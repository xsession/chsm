
add_module_test(
    NAME
        dac8565_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_dac8565_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        dac8565
    DEFINES
        NDEBUG 
    STANDARD
        11
) 
