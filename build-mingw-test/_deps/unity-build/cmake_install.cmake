# Install script for directory: C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/chsm")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/MinGW/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/lib/libunity.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "library" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/unity" TYPE FILE FILES
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/src/unity.h"
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/src/unity_internals.h"
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/extras/fixture/src/unity_fixture.h"
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/extras/fixture/src/unity_fixture_internals.h"
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/extras/memory/src/unity_memory.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/unity/unityTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/unity/unityTargets.cmake"
         "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build/CMakeFiles/Export/80da942aa44c821c5569bae18496db2c/unityTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/unity/unityTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/unity/unityTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/unity" TYPE FILE FILES "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build/CMakeFiles/Export/80da942aa44c821c5569bae18496db2c/unityTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/unity" TYPE FILE FILES "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build/CMakeFiles/Export/80da942aa44c821c5569bae18496db2c/unityTargets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/unity" TYPE FILE FILES
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity/unityConfig.cmake"
    "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build/unityConfigVersion.cmake"
    )
endif()

