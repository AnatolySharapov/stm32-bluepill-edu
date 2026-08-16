@echo off
rem ===================================================================
rem Script Name   : cli_test.bat
rem                 STM32 CLI automated test suite runner batch file
rem Description   : Validates explicit Python environment path, auto-installs
rem                 pyserial dependency, and executes the CLI serial test tool
rem Location      : Process/cli_test/ folder
rem Usage         : Double-click to run in Windows environment
rem
rem Author        : Gemini (Google AI)
rem Verifier      : Anatoly Sharapov
rem Email         : anatoly.a.sharapov@gmail.com
rem Repository    : https://github.com
rem License       : MIT
rem
rem Date Created  : August 16, 2026
rem Last Modified : August 16, 2026
rem Version       : 1.3.1
rem
rem Change History:
rem -------------------------------------------------------------------
rem Version 1.3.1 | Aug 16, 2026 | Bumped version to sync with python core string correction.
rem Version 1.3.0 | Aug 16, 2026 | Added support for automated USB-to-UART COM port discovery.
rem Version 1.2.0 | Aug 16, 2026 | Refactored to use absolute python.exe path and native CMD dependency checks.
rem Version 1.1.0 | Aug 16, 2026 | Added automatic pip check and setup installation for pyserial dependency.
rem Version 1.0.0 | Aug 16, 2026 | Initial release of the automated CLI test runner batch tool.
rem ===================================================================

setlocal enabledelayedexpansion

:: Set console codepage to Windows-1251 for consistent output parsing
chcp 1251 >nul

:: Safe print of the script header metadata by searching lines starting with 'rem '
echo -------------------------------------------------------------------
findstr /b /c:"rem " "%~f0"
echo -------------------------------------------------------------------
echo.

:: PATH SETUP: %cd% dynamically captures the current active directory
set "target_path=%cd%"
echo [Info] Target path set to Current Directory: %target_path%

:: 3. Define absolute paths to Python executable, companion script and target list file
set "py_exe=C:\Work\Prog\Python\3.14.3\python.exe"
echo [Info] py_exe = "%py_exe%"

set "py_test_file=%~dp0cli_test.py"
echo [Info] py_test_file = "%py_test_file%"

:: Check if the core Python test execution script exists
if not exist "%py_test_file%" (
    echo [Error] Companion logic script missing: cli_test.py
    echo [Error] Please ensure both files are placed in the same folder.
    goto :end_script
)

:: Validate if the specific Python interpreter executable exists
if not exist "%py_exe%" (
    echo [Error] Python executable not found at specified path: %py_exe%
    echo [Error] Please check your Python installation directory configuration.
    goto :end_script
)

:: 4. Automatic dependency check and installation
echo [Info] Checking required Python dependencies...

"%py_exe%" -c "import serial" 2>nul
if %errorlevel% neq 0 (
    echo [Info] 'pyserial' library is missing. Attempting automatic installation...
    "%py_exe%" -m pip install pyserial
    if !errorlevel! neq 0 (
        echo [Error] Failed to install 'pyserial' via pip. Please install it manually.
        goto :end_script
    )
    echo [Success] 'pyserial' installed successfully.
    echo.
)

:: Execute the automated testing logic runner
echo [Info] Environment ready. Launching automated test suite runtime...
echo.

"%py_exe%" "%py_test_file%"
if %errorlevel% neq 0 (
    echo.
    echo [Error] Test suite execution terminated with failures.
) else (
    echo.
    echo [SUCCESS] Automated test suite execution completed successfully!
)

:end_script
echo.
echo [Notification] Process complete!
pause
exit /b
