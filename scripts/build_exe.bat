@echo off

set python_path=%PYTHON_PATH%

if exist "%PYTHON_PATH%" (
    echo Environment variable PYTHON_PATH exists!
    echo %python_path%
    %python_path%\python.exe -m pip install pyinstaller
    %python_path%\python.exe -m eel ../chsm/sm_gen/chsm_backend.py ../chsm/sm_gen/web
) else (
    echo Environment variable PYTHON_PATH doesen't exists please create it!
)