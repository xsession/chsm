# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitclone-lastrun.txt" AND EXISTS "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitinfo.txt" AND
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitclone-lastrun.txt" IS_NEWER_THAN "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/ThrowTheSwitch/Unity.git" "unity"
    WORKING_DIRECTORY "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/ThrowTheSwitch/Unity.git'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe" 
          checkout "67ca5c57c9c50cc1580518cc6f822558c0009b1e" --
  WORKING_DIRECTORY "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '67ca5c57c9c50cc1580518cc6f822558c0009b1e'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitinfo.txt" "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/unity-populate-gitclone-lastrun.txt'")
endif()
