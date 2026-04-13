@echo off
setlocal
REM ═══════════════════════════════════════════════════════════════════
REM  CHSM Desktop — Build Script
REM
REM  Builds everything needed for the Electron portable exe:
REM    1. React frontend  (npm run build  → frontend/dist/)
REM    2. Python backend   (PyInstaller   → dist_backend/)
REM    3. Electron app     (electron-builder → release/)
REM
REM  Prerequisites:
REM    - Node.js / npm
REM    - Python 3.10+ with pip
REM    - PyInstaller   (pip install pyinstaller)
REM ═══════════════════════════════════════════════════════════════════

cd /d "%~dp0"

echo.
echo ──────────────────────────────────────────────────
echo  Step 1: Build React frontend
echo ──────────────────────────────────────────────────
pushd frontend
call npm install
if errorlevel 1 goto :error
call npm run build
if errorlevel 1 goto :error
popd

echo.
echo ──────────────────────────────────────────────────
echo  Step 2: Build Python backend (PyInstaller)
echo ──────────────────────────────────────────────────
pip install pyinstaller fastapi "uvicorn[standard]" websockets python-multipart jinja2 2>nul
pyinstaller --noconfirm --distpath dist_backend backend/chsm_backend.spec
if errorlevel 1 goto :error

echo.
echo ──────────────────────────────────────────────────
echo  Step 3: Install Electron dependencies
echo ──────────────────────────────────────────────────
call npm install
if errorlevel 1 goto :error

echo.
echo ──────────────────────────────────────────────────
echo  Step 4: Package Electron app
echo ──────────────────────────────────────────────────
call npx electron-builder --win portable
if errorlevel 1 goto :error

echo.
echo ══════════════════════════════════════════════════
echo  BUILD COMPLETE
echo  Output: release\CHSM-*-portable.exe
echo ══════════════════════════════════════════════════
goto :end

:error
echo.
echo  BUILD FAILED — see errors above.
exit /b 1

:end
endlocal
