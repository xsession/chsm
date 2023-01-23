
add_module_test(
    NAME
        i2c_master_test 
    SOURCE 
        tsrc/main.c
        tsrc/ut_i2c_master_test.c
    INCLUDE 
        tinc
    LINK 
        unity
        i2c_drv_mock
        i2c_master
    DEFINES
        # NDEBUG 
    STANDARD
        11
) 

add_library(app_signal_classes INTERFACE)
target_sources(app_signal_classes INTERFACE tinc/signal_classes.h)
target_link_libraries(app_signal_classes INTERFACE chsm::crf)
target_include_directories(app_signal_classes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tinc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/app_signal_classes>)
# set_target_properties(app_signal_classes
#         PROPERTIES 
#         C_STANDARD          11
#         C_STANDARD_REQUIRED ON)
