
add_module_test(
    NAME
        ltc2471_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_ltc2471_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        ltc2471
    DEFINES
        NDEBUG 
    STANDARD
        11
) 

