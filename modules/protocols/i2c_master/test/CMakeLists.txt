
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

add_module_interface_lib(
    NAME
        app_signal_classes
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/tinc
    LINK
    DEFINES
    STANDARD
        )