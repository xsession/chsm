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

cmake -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_DEBUG_MSG=ON -DCHSM_BUILD_TESTS=ON ../

