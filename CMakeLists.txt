cmake_minimum_required(VERSION 3.19)

project(chsm 
VERSION 1.0.0
LANGUAGES C CXX ASM)

########################  External .cmake files includes  ############################ 

include(cmake_utils)

########################  Global settings  ############################ 
set(DEBUG ON)
option(CHSM_BUILD_TESTS OFF)
set(OUTSIDE_PYTHON_PATH ${$ENV{PYTHON_PATH}})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})  

message(STATUS "
----------------------> Chsm <-----------------------------------
")

add_subdirectory(modules)
add_subdirectory(reactive_frameworks/c_rf)

add_module_interface_lib(
    NAME 
        ${PROJECT_NAME}
    LINK
        chsm::chsm_interfaces
        chsm::test_utils
        chsm::crf 
        chsm::i2c_master 
        chsm::spi_master
        chsm::canopen 
        chsm::dac8565 
        chsm::fram 
        chsm::lm73 
        chsm::ltc2471    
    STANDARD
        11
)
    
if(CHSM_BUILD_TESTS)
    include(FindUnity)
    message(STATUS "----------> Enabled the tests! <-------------------")
    enable_testing()
endif()
# Install ------------------------------------------------------------------------

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion 
)

## Target installation
install(TARGETS   ${PROJECT_NAME} 
    EXPORT        ${PROJECT_NAME}Targets
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT     library
)

# Installing headers
install(
    DIRECTORY "${${PROJECT_NAME}_PUBLIC_HEADERS}"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include
)

## Target's cmake files: targets export
install(EXPORT  ${PROJECT_NAME}Targets
    NAMESPACE   ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

## Target's cmake files: config and version config for find_package()
install(FILES   ${PROJECT_NAME}Config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE ${PROJECT_NAME})

# ------------------ Test and diagnostic ----------------------------------------------------

diagnostic(${PROJECT_NAME})


# add_custom_target(chsm_start ALL
#     COMMAND ${OUTSIDE_PYTHON_PATH}\python.exe .\cgen\chsm_backend.py
#     COMMAND dot -Tpng graphviz/project_dependency.dot -o graphviz/project_dependency.png
#     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#     COMMENT "Graphviz generated dependency map in ${CMAKE_BINARY_DIR}/graphviz/project_dependency.png"
# )