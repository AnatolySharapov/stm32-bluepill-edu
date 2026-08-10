@echo off

rem ===================================================================
rem Script Name   : remove_comments.bat 
rem                 C source code comment remover batch file
rem Description   : Automates the removal of single-line and multi-line 
rem                 comments from C source code files using an external 
rem                 PowerShell script block for absolute stability.
rem Location      : Can be placed in any directory
rem Usage         : Double-click to run inside your current working directory
rem
rem Author        : Gemini (Google AI)
rem Verifier      : Anatoly Sharapov
rem Email         : anatoly.a.sharapov@gmail.com
rem Repository    : https://github.com
rem License       : MIT
rem
rem Date Created  : August 2, 2026
rem Last Modified : August 4, 2026
rem Version       : 1.7.0
rem
rem Change History:
rem -------------------------------------------------------------------
rem Version 1.7.0 | Aug 4, 2026 | Split the batch script into a hybrid bat/ps1 
rem                             | architecture to achieve 100% processing stability 
rem                             | and completely eliminate CMD string escape bugs.
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
set "script_dir=%~dp0"
set "ps_logic_file=%script_dir%strip_logic.ps1"

echo [Info] Target path set to Current Directory: %target_path%

:: Check if the structural companion script exists
if not exist "%ps_logic_file%" (
    echo [Error] Companion logic script missing: strip_logic.ps1
    echo [Error] Please ensure both files are placed in the same folder.
    goto end
)

:: Validate path types using native CMD file system checks
if exist "%target_path%\" (
    goto process_directory
) else if exist "%target_path%" (
    goto process_single_file
) else (
    echo [Error] Target path '%target_path%' does not exist.
    goto end
)

:process_single_file
echo [Info] Target identified as a Single File.
call :clean_file "%target_path%"
goto finish

:process_directory
echo [Info] Target identified as a Directory. Recursively scanning...
set "files_found=0"

for /r "%target_path%" %%f in (*.c *.h) do (
    if exist "%%f" (
        set /a "files_found+=1"
        call :clean_file "%%f"
    )
)

if "!files_found!"=="0" (
    echo [Info] No .c or .h files found in the specified directory.
) else (
    echo [Success] Total files processed: !files_found!
)
goto finish


:: ===================================================================
:: Core processing logic runner
:: ===================================================================
:clean_file
set "file_to_clean=%~1"
echo [Processing] !file_to_clean!

:: Create an automated backup file copy (.bak) before modifying data
copy /y "!file_to_clean!" "!file_to_clean!.bak" >nul
if %errorlevel% neq 0 (
    echo [Warning] Backup creation failed for: !file_to_clean!
) else (
    echo [Backup] Created backup copy: !file_to_clean!.bak
)

:: Run isolated companion script with ExecutionPolicy bypass to avoid Windows blocks
powershell -NoProfile -ExecutionPolicy Bypass -File "%ps_logic_file%" -filePath "!file_to_clean!"

if %errorlevel% neq 0 (
    echo [Error] Failed to process: !file_to_clean!
) else (
    echo [SUCCESS] Stripped comments from: !file_to_clean!
)
exit /b


:finish
echo.
echo [Notification] Process complete!

:end
  pause
  exit /b
