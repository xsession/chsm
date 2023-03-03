
add_module_test(
    NAME
        i2c_master_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_i2c_master_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        i2c_master
    DEFINES
        NDEBUG 
        "SIGNAL_CLASSES_H=$<JOIN:SIGNAL_CLASSES_H,$CACHE{SIGNAL_CLASSES_H_PATH}>"
    STANDARD
        11
) 

# # define the target and include directories
set(I2C_MASTER_SIG_CLASSES_PATH "<${CMAKE_CURRENT_SOURCE_DIR}/tinc/signal_classes.h>")

# create the template file
file(WRITE "include_dirs.cmake.in" "set(SIGNAL_CLASSES_H_PATH @I2C_MASTER_SIG_CLASSES_PATH@ CACHE STRING "i2c_master_test" FORCE)\n")

# configure the include_dirs.cmake file
configure_file(include_dirs.cmake.in include_dirs.cmake)

# add the custom command
add_custom_command(
    PRE_BUILD i2c_master_test
    OUTPUT include_dirs.cmake
    COMMAND ${CMAKE_COMMAND} -E echo "configuring include_dirs.cmake"
    DEPENDS include_dirs.cmake.in
)

# include the generated file
include(${CMAKE_CURRENT_BINARY_DIR}/include_dirs.cmake)