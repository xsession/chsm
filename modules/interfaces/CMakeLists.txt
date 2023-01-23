
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

add_dependencies(crf_interfaces app_signal_classes)

if(TARGET app_signal_classes )
        message("-----> app_signal_classes is alive!!!")
else()
        message("-----> app_signal_classes is dead!!!")
endif()
