
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(mpr121_test 
        src/main.c
        src/ut_mpr121_test.c
        ../src/mpr121.c
        ../src/mpr121_functions.c
)

target_include_directories(mpr121_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(mpr121_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(mpr121_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(mpr121_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(mpr121_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME mpr121_test
        COMMAND mpr121_test
       )
    