
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
                SIGNAL_CLASSES_H=${SIGNAL_CLASSES_H_PATH}
        STANDARD
                11
)


message("----------------> COMPILE_OPTIONS: ${COMPILE_OPTIONS}")
