

add_executable(vcnl4040_test 
        src/main.c
        src/ut_vcnl4040_test.c
        ../src/vcnl4040.c
        ../src/vcnl4040_functions.c
)

target_include_directories(vcnl4040_test PRIVATE
        inc
        ../inc
)

target_compile_definitions(vcnl4040_test PRIVATE 
        NDEBUG
        SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_classes.h"
)

target_compile_options(vcnl4040_test PRIVATE 
        -Wall 
        -Wextra 
        -pedantic 
        # -Werror
)
target_link_options(vcnl4040_test PRIVATE
        -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(vcnl4040_test PRIVATE
        unity
        crf 
        crf_interfaces
        )

enable_testing()

add_test(NAME vcnl4040_test
        COMMAND vcnl4040_test
       )
    