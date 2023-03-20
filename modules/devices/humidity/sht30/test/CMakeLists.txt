
add_module_test(
    NAME
        sht30_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_sht30_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        sht30
    DEFINES
        NDEBUG 
    STANDARD
        11
) 

