@echo off

cd ../build-mingw-test

cmake --build . -t i2c_master_test
cd bin
i2c_master_test.exe
