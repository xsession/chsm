
add_module_lib(
        NAME 
                i2c_master
        PACKAGE
                chsm
        SOURCE
                src/i2c_master.c
                src/i2c_master_functions.c
        INCLUDE 
                inc
        LINK 
                crf_interfaces
                chsm::crf
                # app_signal_classes
                default_signal_classes
        DEFINES 
                NDEBUG
        STANDARD
                11
)
