
cmake_minimum_required(VERSION 3.19)

add_library(lsm9ds1 STATIC 
        src/lsm9ds1.c
        src/lsm9ds1_functions.c
)

add_library(chsm::lsm9ds1 ALIAS lsm9ds1)

target_compile_definitions(lsm9ds1 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(lsm9ds1
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lsm9ds1>
)

target_link_libraries(lsm9ds1 PUBLIC
)

install_lib(lsm9ds1 chsm 11)
diagnostic(lsm9ds1)
