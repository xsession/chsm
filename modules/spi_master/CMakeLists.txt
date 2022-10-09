
add_module_interface_lib(
        NAME 
                spi_master
        PACKAGE
                chsm
        SOURCE
                src/spi_master.c
                src/spi_master_functions.c 
        INCLUDE 
                inc
        LINK 
                chsm::chsm_interfaces
        DEFINES 
                NDEBUG
        STANDARD
                11
)
