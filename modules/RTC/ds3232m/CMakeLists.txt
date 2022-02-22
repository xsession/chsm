
cmake_minimum_required(VERSION 3.19)

add_library(ds3232m STATIC 
        src/ds3232m.c
        src/ds3232m_functions.c
)

add_library(chsm::ds3232m ALIAS ds3232m)

target_compile_definitions(ds3232m PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(ds3232m
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ds3232m>
)

target_link_libraries(ds3232m PUBLIC
)

install_lib(ds3232m chsm 11)
diagnostic(ds3232m)
