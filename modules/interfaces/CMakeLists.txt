
add_module_interface_lib(
        NAME
                chsm_interfaces
        PACKAGE 
                chsm
        SOURCES
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

