
add_module_test(
    NAME
        vcnl4040_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_vcnl4040_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        vcnl4040
    DEFINES
        NDEBUG 
    STANDARD
        11
) 

