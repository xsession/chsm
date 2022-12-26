cmake_minimum_required(VERSION 3.19)

add_executable(tmp468_test 
    tsrc/main.c
    tsrc/ut_tmp468_test.c
    ../src/tmp468.c
    ../src/tmp468_functions.c
    ../../../../../../chsm/modules/interfaces/src/sys_if.c
    ../../../../../../chsm/modules/interfaces/test/src/ut_i2c_driver_mock.c
    ../../../../../../chsm/modules/i2c_master/src/i2c_master.c
    ../../../../../../chsm/modules/i2c_master/src/i2c_master_functions.c
)

target_include_directories(tmp468_test PRIVATE
    tinc
    ../inc
    ../../../../../../chsm/modules/interfaces/inc
    ../../../../../../chsm/modules/interfaces/test/inc
    ../../../../../../chsm/modules/i2c_master/inc
)

target_compile_definitions(tmp468_test PRIVATE 
    NDEBUG
    SIGNAL_CLASSES_H="${CMAKE_CURRENT_SOURCE_DIR}/tinc/signal_classes.h")

target_compile_options(tmp468_test PRIVATE 
    -std=c11
    -Wall 
    -Wextra 
    -pedantic 
    -Wno-unused-parameter
    -Wno-unused-but-set-variable
    # -Werror
)
target_link_options(tmp468_test PRIVATE
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(tmp468_test PRIVATE 
    crf 
    unity
)

diagnostic(tmp468_test)

enable_testing()

add_test(NAME tmp468_test
        COMMAND tmp468_test
       )