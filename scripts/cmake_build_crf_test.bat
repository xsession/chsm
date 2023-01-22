@echo off

cd ../build-mingw-test

cmake --build . -t crf_test -v
cd bin
crf_test.exe
