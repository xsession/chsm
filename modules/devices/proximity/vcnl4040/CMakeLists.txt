
cmake_minimum_required(VERSION 3.19)

add_library(vcnl4040 STATIC 
        src/vcnl4040.c
        src/vcnl4040_functions.c
)

add_library(chsm::vcnl4040 ALIAS vcnl4040)

target_compile_definitions(vcnl4040 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(vcnl4040
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vcnl4040>
)

target_link_libraries(vcnl4040 PUBLIC
)

install_lib(vcnl4040 chsm 11)
diagnostic(vcnl4040)
