
cmake_minimum_required(VERSION 3.19)

add_library(hts221 STATIC 
        src/hts221.c
        src/hts221_functions.c
)

add_library(chsm::hts221 ALIAS hts221)

target_compile_definitions(hts221 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(hts221
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/hts221>
)

target_link_libraries(hts221 PUBLIC
)

install_lib(hts221 chsm 11)
diagnostic(hts221)
