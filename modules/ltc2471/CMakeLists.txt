
add_module_interface_lib(
        NAME 
                ltc2471
        PACKAGE
                chsm
        SOURCE
                src/ltc2471.c
                src/ltc2471_functions.c
        INCLUDE 
                inc
        LINK 
                chsm::i2c_master
        DEFINES 
                NDEBUG
        STANDARD
                11
)