
cmake_minimum_required(VERSION 3.19)

add_library(ccs811 STATIC 
        src/ccs811.c
        src/ccs811_functions.c
)

add_library(chsm::ccs811 ALIAS ccs811)

target_compile_definitions(ccs811 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(ccs811
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ccs811>
)

target_link_libraries(ccs811 PUBLIC
)

install_lib(ccs811 chsm 11)
diagnostic(ccs811)
