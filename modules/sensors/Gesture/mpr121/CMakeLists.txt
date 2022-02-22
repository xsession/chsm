
cmake_minimum_required(VERSION 3.19)

add_library(mpr121 STATIC 
        src/mpr121.c
        src/mpr121_functions.c
)

add_library(chsm::mpr121 ALIAS mpr121)

target_compile_definitions(mpr121 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(mpr121
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mpr121>
)

target_link_libraries(mpr121 PUBLIC
)

install_lib(mpr121 chsm 11)
diagnostic(mpr121)
