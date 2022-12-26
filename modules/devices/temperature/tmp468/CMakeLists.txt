cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

add_library(tmp468 STATIC
    src/tmp468.c
    src/tmp468_functions.c
)

target_compile_definitions(tmp468 PUBLIC 
    SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(tmp468 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../inc>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/tmp468>
)

target_link_libraries(tmp468 PUBLIC
    i2c_master
)

diagnostic(tmp468)

if(APP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(APP_BUILD_TESTS)