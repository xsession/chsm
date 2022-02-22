
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(lps22hb_test 
        src/main.c
        src/ut_lps22hb_test.c
        ../src/lps22hb.c
        ../src/lps22hb_functions.c
)

target_include_directories(lps22hb_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(lps22hb_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(lps22hb_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(lps22hb_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(lps22hb_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME lps22hb_test
        COMMAND lps22hb_test
       )
    