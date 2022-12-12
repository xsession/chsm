cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(
  "menu_structure"
  VERSION 1.0.0
  DESCRIPTION
    "chsm based test project."
    HOMEPAGE_URL ""
  LANGUAGES C CXX ASM)

include(GNUInstallDirs)

set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/.project/cmake_build/build")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})  

message(STATUS "CMAKE_GENERATOR: -> ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_TOOLCHAIN_FILE: -> ${CMAKE_TOOLCHAIN_FILE}")

message(STATUS "PROJECT_SOURCE_DIR: -> ${PROJECT_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/.project") # itt van a fetch_dependency.cmake, hogy project specifikus legyen.

set(debug ON)

add_subdirectory(menu_structure)