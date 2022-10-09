
add_module_interface_lib(
    NAME 
            fram
    PACKAGE
            chsm
    SOURCE
            src/fram.c
            src/fram_functions.c
    INCLUDE 
            inc
    LINK 
            chsm::i2c_master
    DEFINES 
            NDEBUG
    STANDARD
            11
)