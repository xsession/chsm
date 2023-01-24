
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
        # NDEBUG 
    STANDARD
        11
) 


# define the target and include directories
set(TARGET "app_signal_classes")
set(INCLUDE_DIRS "tinc")

# create the template file
file(WRITE "include_dirs.cmake.in" "target_include_directories(@TARGET@ INTERFACE @INCLUDE_DIRS@)\n")

# configure the include_dirs.cmake file
configure_file(include_dirs.cmake.in include_dirs.cmake @ONLY)

# add the custom command
add_custom_command(
    OUTPUT include_dirs.cmake
    COMMAND ${CMAKE_COMMAND} -E echo "configuring include_dirs.cmake"
    DEPENDS include_dirs.cmake.in
    PRE_BUILD i2c_master_test
)

# include the generated file
include(${CMAKE_CURRENT_BINARY_DIR}/include_dirs.cmake)