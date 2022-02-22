
cmake_minimum_required(VERSION 3.19)

add_library(apds9960 STATIC 
        src/apds9960.c
        src/apds9960_functions.c
)

add_library(chsm::apds9960 ALIAS apds9960)

target_compile_definitions(apds9960 PUBLIC 
        NDEBUG
        SIGNAL_CLASSES_H=${CHSM_SIGNAL_CLASSES_H}
)

target_include_directories(apds9960
PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/apds9960>
)

target_link_libraries(apds9960 PUBLIC
)

install_lib(apds9960 chsm 11)
diagnostic(apds9960)
