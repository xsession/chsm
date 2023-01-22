cmake_minimum_required(VERSION 3.19)

add_executable(sht30_test 
    tsrc/main.c
    tsrc/ut_sht30_test.c
    ../src/sht30.c
    ../src/sht30_functions.c
    # ../../../../../../chsm/modules/interfaces/test/src/ut_i2c_driver_mock.c
    # ../../../../../../chsm/modules/i2c_master/src/i2c_master.c
    # ../../../../../../chsm/modules/i2c_master/src/i2c_master_functions.c
)

target_include_directories(sht30_test PRIVATE
    tinc
    ../inc
    # ../../../../../../chsm/modules/interfaces/test/inc
    # ../../../../../../chsm/modules/i2c_master/inc
)

target_compile_definitions(sht30_test PUBLIC 
    NDEBUG
    SIGNAL_CLASSES_H=\"signal_classes.h\"
    )

target_compile_options(sht30_test PRIVATE 
    -std=c11
    -Wall 
    -Wextra 
    -pedantic 
    # -Werror
)
target_link_options(sht30_test PRIVATE
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map   
)

target_link_libraries(sht30_test 
    PUBLIC 
        unity
        chsm::crf 
        chsm::crf_interfaces
        chsm::test_utils
        chsm::i2c_master
    )



diagnostic(sht30_test)

enable_testing()

add_test(NAME sht30_test
        COMMAND sht30_test
       )