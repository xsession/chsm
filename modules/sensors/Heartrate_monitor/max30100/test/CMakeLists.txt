
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(max30100_test 
        src/main.c
        src/ut_max30100_test.c
        ../src/max30100.c
        ../src/max30100_functions.c
)

target_include_directories(max30100_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(max30100_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(max30100_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(max30100_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(max30100_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME max30100_test
        COMMAND max30100_test
       )
    