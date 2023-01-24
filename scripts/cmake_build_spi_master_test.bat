@echo off

cd ../build-mingw-test

cmake --build . -t spi_master_test
cd bin
spi_master_test.exe
