
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(apds9960_test 
        src/main.c
        src/ut_apds9960_test.c
        ../src/apds9960.c
        ../src/apds9960_functions.c
)

target_include_directories(apds9960_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(apds9960_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(apds9960_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(apds9960_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(apds9960_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME apds9960_test
        COMMAND apds9960_test
       )
    