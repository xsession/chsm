
add_module_lib(
        NAME
                crf_interfaces
        PACKAGE 
                chsm
        SOURCE
                src/sys_if.c
        INCLUDE 
                inc
        LINK 
                chsm::crf
        DEFINES 
                NDEBUG
        STANDARD
                11
)
