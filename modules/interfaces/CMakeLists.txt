
add_module_interface_lib(
        NAME
                crf_interfaces
        PACKAGE 
                chsm
        SOURCES
                tsrc/sys_if.c
        INCLUDE 
                inc
        LINK 
                chsm::crf
                app_signal_classes
        DEFINES 
                NDEBUG
        STANDARD
                11
)
