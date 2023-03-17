
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
                # app_signal_classes
        DEFINES 
                NDEBUG
                # SIGNAL_CLASSES_H=$<TARGET_PROPERTY:SIGNAL_CLASSES_H>
        STANDARD
                11
)

# add_dependencies(crf_interfaces i2c_master_test)
# get_property(test_targets DIRECTORY "${CMAKE_PROJECT_SOURCE_DIR}" PROPERTY TESTS)
# message("test_targets: ----> ${test_targets}")
# # Create a custom target for all tests
# add_custom_target(tests)

message("----------------> COMPILE_OPTIONS: ${COMPILE_OPTIONS}")
