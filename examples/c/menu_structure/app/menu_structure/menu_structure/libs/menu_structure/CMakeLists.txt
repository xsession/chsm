
add_module_lib(
    NAME 
            menu_structure
    PACKAGE
            app_libs
    SOURCE
            src/menu_structure.c
            src/menu_structure_functions.c
    INCLUDE 
            inc
    LINK 
            chsm::crf
    DEFINES 
            NDEBUG
    STANDARD
            11
)