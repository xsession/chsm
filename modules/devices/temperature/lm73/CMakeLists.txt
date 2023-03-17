
add_module_lib(
        NAME 
                lm73
        PACKAGE
                chsm
        SOURCE
                src/lm73.c
                src/lm73_functions.c
        INCLUDE 
                inc
        LINK 
                chsm::i2c_master
        DEFINES 
                NDEBUG
        STANDARD
                11
)