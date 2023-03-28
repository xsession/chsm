
add_module_test(
    NAME
        fram_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_fram_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        fram
    DEFINES
        NDEBUG 
    STANDARD
        11
) 
