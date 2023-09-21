
add_module_test(
    NAME
    {{cookiecutter.module_name}}_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_{{cookiecutter.module_name}}_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        {{cookiecutter.module_name}}
    DEFINES
        NDEBUG 
        CHSM_BUILD_TESTS
    STANDARD
        11
) 

set_property(TARGET {{cookiecutter.module_name}}_test PROPERTY ECLIPSE_GENERATE_SOURCE_PROJECT ON)
set_property(TARGET {{cookiecutter.module_name}}_test PROPERTY ECLIPSE_GENERATE_LINKED_RESOURCES ON)