
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
        CHSM_BUILD_TESTS
    STANDARD
        11
) 

set_property(TARGET lm73_test PROPERTY ECLIPSE_GENERATE_SOURCE_PROJECT ON)
set_property(TARGET lm73_test PROPERTY ECLIPSE_GENERATE_LINKED_RESOURCES ON)