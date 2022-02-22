
cmake_minimum_required(VERSION 3.19)

add_library(lps22hb STATIC 
        src/lps22hb.c
        src/lps22hb_functions.c
)

add_library(chsm::lps22hb ALIAS lps22hb)

target_compile_definitions(lps22hb PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(lps22hb
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lps22hb>
)

target_link_libraries(lps22hb PUBLIC
)

install_lib(lps22hb chsm 11)
diagnostic(lps22hb)
