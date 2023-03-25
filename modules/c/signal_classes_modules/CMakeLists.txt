add_library(signal_classes_modules INTERFACE)

target_include_directories(
    signal_classes_modules INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>
                     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lib>)