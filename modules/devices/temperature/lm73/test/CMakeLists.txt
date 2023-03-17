
add_module_test(
    NAME
        lm73_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_lm73_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        lm73
    DEFINES
        NDEBUG 
    STANDARD
        11
) 

