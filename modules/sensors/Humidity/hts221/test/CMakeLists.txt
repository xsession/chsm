
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(hts221_test 
        src/main.c
        src/ut_hts221_test.c
        ../src/hts221.c
        ../src/hts221_functions.c
)

target_include_directories(hts221_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(hts221_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(hts221_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(hts221_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(hts221_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME hts221_test
        COMMAND hts221_test
       )
    