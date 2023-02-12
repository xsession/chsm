
add_module_lib(
        NAME 
                vcnl4040
        PACKAGE
                chsm
        SOURCE 
                src/vcnl4040.c
                src/vcnl4040_functions.c
        INCLUDE 
                inc
        LINK 
                chsm::crf
        DEFINES 
        STANDARD
                11
)