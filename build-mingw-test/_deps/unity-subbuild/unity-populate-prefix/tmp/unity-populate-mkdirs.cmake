# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/unity"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-build"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/tmp"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src"
  "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/build-mingw-test/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
