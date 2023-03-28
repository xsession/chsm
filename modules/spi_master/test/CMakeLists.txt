
add_module_test(
    NAME
        spi_master_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_spi_master_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        spi_drv_mock
        spi_master
    DEFINES
        NDEBUG
    STANDARD
        11
) 
