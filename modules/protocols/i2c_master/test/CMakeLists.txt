
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
        NDEBUG 
        SIGNAL_CLASSES_H=$<TARGET_PROPERTY:i2c_master_test,"${CMAKE_CURRENT_SOURCE_DIR}/tinc/signal_classes.h">
    STANDARD
        11
) 


