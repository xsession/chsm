
cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake/utils")

include(Fetch_unity)

add_executable(ds3232m_test 
        src/main.c
        src/ut_ds3232m_test.c
        ../src/ds3232m.c
        ../src/ds3232m_functions.c
)

target_include_directories(ds3232m_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(ds3232m_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(ds3232m_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(ds3232m_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(ds3232m_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME ds3232m_test
        COMMAND ds3232m_test
       )
    