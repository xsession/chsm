cmake_minimum_required(VERSION 3.14...3.18 FATAL_ERROR)

add_executable(menu
    libs/menu_structure/src/menu_structure.c
    libs/menu_structure/src/menu_structure_functions.c
    src/main.c
)

target_include_directories(menu PUBLIC
    inc
    libs/menu_structure/inc
)

message("CMAKE_CURRENT_SOURCE_DIR: -> ${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(menu PROPERTIES SUFFIX ".exe")

target_compile_definitions(menu PUBLIC
    NDEBUG
)

target_compile_options(menu PRIVATE
    -std=c11
)

target_link_options(menu PRIVATE
)

target_link_directories(menu PRIVATE
)

target_link_libraries(menu PUBLIC
    chsm::crf
    chsm::crf_interfaces
)

include(diagnostic)
diagnostic(menu)
