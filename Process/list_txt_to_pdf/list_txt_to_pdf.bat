@echo off

rem ===================================================================
rem Script Name   : list_txt_to_pdf.bat
rem                 text list to pdf converter batch file
rem Description   : Process fixed '%list_file%' with relative paths,
rem                 auto-installs reportlab & pygments, and merges PDFs
rem Location      : Must be placed in the same directory as %list_file%
rem Usage         : Double-click to run in Windows environment
rem
rem Author        : Gemini (Google AI)
rem Verifier      : Anatoly Sharapov
rem Email         : anatoly.a.sharapov@gmail.com
rem Repository    : https://github.com
rem License       : MIT
rem
rem Date Created  : August 15, 2026
rem Last Modified : August 15, 2026
rem Version       : 1.4.0
rem
rem Change History:
rem -------------------------------------------------------------------
rem Version 1.4.0 | Aug 15, 2026 | Refactored logs and template creation to dynamically use %%list_file%% variable.
rem Version 1.3.0 | Aug 15, 2026 | Added automatic pygments syntax highlighting library installation.
rem Version 1.2.1 | Aug 15, 2026 | Fixed explicit path to python.exe.
rem Version 1.2.0 | Aug 15, 2026 | Added automatic reportlab dependency installation.
rem Version 1.1.0 | Aug 15, 2026 | Changed logic to target "%%list_file%%" in local dir.
rem Version 1.0.0 | Aug 15, 2026 | Initial release of the list converter batch tool.
rem ===================================================================

setlocal enabledelayedexpansion

:: 1. Safe print of the script header metadata by searching lines starting with 'rem '
echo -------------------------------------------------------------------
findstr /b /c:"rem " "%~f0"
echo -------------------------------------------------------------------
echo.

echo %~n0 started at %time% %date%
echo.

:: 2. Set code page to UTF-8 for proper Cyrillic and international character output
chcp 65001 > nul

:: 3. Define absolute paths to Python executable, companion script and target list file
set "py_exe=C:\Work\Prog\Python\3.14.3\python.exe"
echo [Info] py_exe = "%py_exe%"

set "script_path=%~dp0list_txt_to_pdf.py"
echo [Info] script_path = "%script_path%"

set "list_file=%~dp0txt_to_pdf.lst"
echo [Info] list_file = "%list_file%"

:: 4. Automatic dependency check and installation
echo [Info] Checking required Python dependencies...

"%py_exe%" -c "import reportlab" 2>nul
if %errorlevel% neq 0 (
  echo [Info] 'reportlab' library is missing. Attempting automatic installation...
  "%py_exe%" -m pip install reportlab
  if !errorlevel! neq 0 (
    echo [Error] Failed to install 'reportlab' via pip. Please install it manually.
    goto :end_script
  )
  echo [Success] 'reportlab' installed successfully.
  echo.
)

"%py_exe%" -c "import pygments" 2>nul
if %errorlevel% neq 0 (
  echo [Info] 'pygments' library is missing. Attempting automatic installation...
  "%py_exe%" -m pip install pygments
  if !errorlevel! neq 0 (
    echo [Error] Failed to install 'pygments' via pip. Please install it manually.
    goto :end_script
  )
  echo [Success] 'pygments' installed successfully.
  echo.
)

echo [Info] All dependencies are satisfied.
echo.

:: 5. Check if the target list file "%list_file%" exists
if not exist "%list_file%" (
  echo [Warning] Target list file not found: "%list_file%"
  echo Creating an empty template "%list_file%" in the script directory...
  (
    echo # Fill this file with relative paths to text files, one per line.
    echo # Example: subfolder\document.txt
    echo # Example: notes.txt
  ) > "%list_file%"
  echo [Attention] Please populate the created file with paths and run the batch file again.
  goto :end_script
)

:: 6. Execute Python script passing the absolute path to the list file
"%py_exe%" "%script_path%" "%list_file%"

:: 7. Check the Python script execution exit code
if %errorlevel% neq 0 (
  echo.
  echo [Error] An error occurred during Python script execution.
) else (
  echo.
  echo [Success] Text conversion and merging completed successfully.
)

:end_script
echo.
pause
