                
add_module_test(
    NAME
        intf_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_i2c_driver_mock_test.c
        tsrc/ut_spi_driver_mock_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        crf
        i2c_drv_mock
        spi_drv_mock
    DEFINES
        # NDEBUG 
    STANDARD
        11
) 


add_module_lib(
    NAME 
        i2c_drv_mock
    SOURCE
        tsrc/ut_i2c_driver_mock.c
    INCLUDE
        tinc
    LINK
        crf_interfaces
    DEFINES
        SIGNAL_CLASSES_H=$CACHE{SIGNAL_CLASSES_H_PATH}
    STANDARD
        11
)

add_module_lib(
    NAME 
        spi_drv_mock
    SOURCE
        tsrc/ut_spi_driver_mock.c
    INCLUDE
        tinc
    LINK
        crf_interfaces
    DEFINES
        SIGNAL_CLASSES_H=$CACHE{SIGNAL_CLASSES_H_PATH}
    STANDARD
        11
)

