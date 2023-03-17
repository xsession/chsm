
target_include_directories(i2c_master PRIVATE tinc)

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
    STANDARD
        11
) 

# set_target_properties(i2c_master_test PROPERTIES SIGNAL_CLASSES "${CMAKE_CURRENT_SOURCE_DIR}/tinc")

add_dependencies(i2c_master_test i2c_master)
# define the target and include directories
# set(I2C_MASTER_SIG_CLASSES_PATH "<${CMAKE_CURRENT_SOURCE_DIR}/tinc/signal_classes.h>")

# target_sources(app_signal_classes $<TARGET_PROPERTIES:i2c_master_test,SIGNAL_CLASSES>)

# create the template file
# target_include_directories(app_signal_classes $<TARGET_PROPERTIES:i2c_master_test,SIGNAL_CLASSES>) \n 
# file(WRITE "i2c_master_test_include_dirs.cmake.in" "message($<TARGET_PROPERTIES:i2c_master_test,SIGNAL_CLASSES>) \n")

# configure the include_dirs.cmake file
# configure_file(i2c_master_test_include_dirs.cmake.in i2c_master_test_include_dirs.cmake @ONLY)

# add the custom command
# add_custom_command(
#     PRE_BUILD i2c_master_test
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/i2c_master_test_include_dirs.cmake
#     COMMAND ${CMAKE_COMMAND} -E echo "configuring i2c_master_test_include_dirs.cmake"
#     DEPENDS i2c_master_test_include_dirs.cmake.in
# )

# include the generated file
# include(${CMAKE_CURRENT_BINARY_DIR}/i2c_master_test_include_dirs.cmake)