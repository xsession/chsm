add_library(default_signal_classes INTERFACE)

target_include_directories(
    default_signal_classes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>
                     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lib>)