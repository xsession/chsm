@echo off

set python_path=%PYTHON_PATH%

if exist "%PYTHON_PATH%" (
    echo Environment variable PYTHON_PATH exists!
    echo %python_path%
    %python_path%\python.exe .\chsm\sm_gen\chsm_backend.py
) else (
    echo Environment variable PYTHON_PATH doesen't exists please create it!
)