cmake_minimum_required(VERSION 3.19)

add_module_test(
        NAME
                menu_structure_test
        SOURCE 
                tsrc/main.c
                tsrc/ut_menu_structure_test.c
        INCLUDE 
                tinc
        LINK 
                unity
                chsm::crf
                app_libs::menu_structure
        DEFINES 
                NDEBUG
    )