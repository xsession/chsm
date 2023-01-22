# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.25.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(unity-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[C:/Program Files/Git/cmd/git.exe]==])
set(GIT_VERSION_STRING [==[2.24.1.windows.2]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[C:/Program Files/Git/cmd/git.exe;2.24.1.windows.2]==]
)


include(ExternalProject)
ExternalProject_Add(unity-populate
                     "UPDATE_DISCONNECTED" "True" "GIT_REPOSITORY" "https://github.com/ThrowTheSwitch/Unity.git" "GIT_TAG" "67ca5c57c9c50cc1580518cc6f822558c0009b1e"
                    SOURCE_DIR          "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity"
                    BINARY_DIR          "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


