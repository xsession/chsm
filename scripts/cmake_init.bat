@echo off

if exist "%~dp0/../build-mingw" (
    echo directory exist
    cd ../
    cd build-mingw
) else (
    echo directory doesen't exist
    cd ..
    mkdir build-mingw
    cd build-mingw
)

cmake -G "MinGW Makefiles" ../

