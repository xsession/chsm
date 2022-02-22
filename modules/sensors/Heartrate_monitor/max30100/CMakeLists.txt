
cmake_minimum_required(VERSION 3.19)

add_library(max30100 STATIC 
        src/max30100.c
        src/max30100_functions.c
)

add_library(chsm::max30100 ALIAS max30100)

target_compile_definitions(max30100 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(max30100
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/max30100>
)

target_link_libraries(max30100 PUBLIC
)

install_lib(max30100 chsm 11)
diagnostic(max30100)
