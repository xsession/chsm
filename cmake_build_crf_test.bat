@echo off

cd build-mingw

cmake --build . -t crf_test 
ctest -VV -R crf_test