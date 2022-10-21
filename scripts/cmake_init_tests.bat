@echo off

set current_path=%cd%
set cpu_architecture=mingw

if exist "%~dp0/../build-%cpu_architecture%" (
    echo directory exist
    cd ..
    cd build-%cpu_architecture%
) else (
    echo directory doesen't exist
    cd ..
    mkdir build-%cpu_architecture%
    cd build-%cpu_architecture%
)

cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%current_path%\cmake_utils\compilers\toolchain-mingw.cmake -DCHSM_BUILD_TESTS=ON ../

