
add_module_test(
    NAME
        tmp468_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_tmp468_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        tmp468
    DEFINES
        NDEBUG 
    STANDARD
        11
) 

