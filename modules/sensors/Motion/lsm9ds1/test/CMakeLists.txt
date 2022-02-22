
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(lsm9ds1_test 
        src/main.c
        src/ut_lsm9ds1_test.c
        ../src/lsm9ds1.c
        ../src/lsm9ds1_functions.c
)

target_include_directories(lsm9ds1_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(lsm9ds1_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(lsm9ds1_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(lsm9ds1_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(lsm9ds1_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME lsm9ds1_test
        COMMAND lsm9ds1_test
       )
    