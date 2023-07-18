@echo off

set current_path=%cd%
set cpu_architecture=mingw

if exist "%~dp0/../build-%cpu_architecture%-test" (
    echo directory exist
    cd ..
    cd build-%cpu_architecture%-test
) else (
    echo directory doesen't exist
    cd ..
    mkdir build-%cpu_architecture%-test
    cd build-%cpu_architecture%-test
)

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_MSG=ON -DCHSM_BUILD_TESTS=ON ../

