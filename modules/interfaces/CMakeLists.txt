
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
                app_signal_classes
        DEFINES 
                NDEBUG
        STANDARD
                11
)


# add_module_lib(
#         NAME
#                 app_signal_classes
#         PACKAGE
#                 chsm
#         SOURCE
#                 src/app_signal_classes.c
#         INCLUDE
#         LINK
#                 chsm::crf
#         DEFINES
#         STANDARD
#                 11
# )



message("----------------> COMPILE_OPTIONS: ${COMPILE_OPTIONS}")
