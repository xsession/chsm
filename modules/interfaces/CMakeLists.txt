
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
                # $<CACHE:SIGNAL_CLASSES_H> 
                SIGNAL_CLASSES_H=$CACHE{SIGNAL_CLASSES_H}
        STANDARD
                11
)


message("----------------> COMPILE_OPTIONS: ${COMPILE_OPTIONS}")
