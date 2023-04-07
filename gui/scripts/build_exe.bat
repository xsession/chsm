@echo off

set python_path=%PYTHON_PATH%

if exist "%PYTHON_PATH%" (
    echo Environment variable PYTHON_PATH exists!
    echo %python_path%
    cd ../chsm/sm_gen
    %python_path%\python.exe -m pip install pyinstaller eel docopt 
    %python_path%\python.exe -m  eel ./chsm_backend.py --clean --onefile web -r ./cgen/templates
) else (
    echo Environment variable PYTHON_PATH doesen't exists please create it!
)
