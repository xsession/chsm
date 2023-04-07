cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

add_library(sht30 STATIC
    src/sht30.c
    src/sht30_functions.c
)

target_compile_definitions(sht30 PUBLIC 
    NDEBUG
    SIGNAL_CLASSES_H=${SIGNAL_CLASSES_H}
)

target_include_directories(sht30 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../inc>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/sht30>
)

target_link_libraries(sht30 PUBLIC
    i2c_master
)

diagnostic(sht30)

if(APP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(APP_BUILD_TESTS)
