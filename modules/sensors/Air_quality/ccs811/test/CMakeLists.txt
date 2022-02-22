
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(ccs811_test 
        src/main.c
        src/ut_ccs811_test.c
        ../src/ccs811.c
        ../src/ccs811_functions.c
)

target_include_directories(ccs811_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(ccs811_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(ccs811_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(ccs811_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(ccs811_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME ccs811_test
        COMMAND ccs811_test
       )
    