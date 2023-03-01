
add_module_test(
    NAME
        spi_master_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_spi_master_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        spi_drv_mock
        spi_master
    DEFINES
        NDEBUG
        SIGNAL_CLASSES_H=$CACHE{SIGNAL_CLASSES_H_PATH} 
    STANDARD
        11
) 

# define the target and include directories
set(SPI_MASTER_SIG_CLASSES_PATH "<${CMAKE_CURRENT_SOURCE_DIR}/tinc/signal_classes.h>")

# create the template file
file(WRITE "include_dirs_spi_master.cmake.in" "set(SIGNAL_CLASSES_H_PATH @SPI_MASTER_SIG_CLASSES_PATH@ CACHE STRING "spi_master_test" FORCE)\n")

# configure the include_dirs.cmake file
configure_file(include_dirs_spi_master.cmake.in include_dirs_spi_master.cmake)

# add the custom command
add_custom_command(
    OUTPUT include_dirs_spi_master.cmake
    COMMAND ${CMAKE_COMMAND} -E echo "configuring include_dirs.cmake"
    DEPENDS include_dirs_spi_master.cmake.in
    PRE_BUILD spi_master_test
)

# include the generated file
include(${CMAKE_CURRENT_BINARY_DIR}/include_dirs_spi_master.cmake)