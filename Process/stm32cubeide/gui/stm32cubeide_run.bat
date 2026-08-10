@echo off

rem ===================================================================
rem Script Name   : stm32cubeide_run.bat
rem                 STM32CubeIDE project launcher batch file
rem
rem Description   : Automatically detects the STM32CubeIDE project,
rem                 and opens it in STM32CubeIDE using a temporary workspace.
rem
rem Location      : Project root directory or its "scripts" subdirectory.
rem
rem Usage         : Edit STM32CUBEIDEC below and run the script.
rem
rem Author        : ChatGPT (OpenAI)
rem Verifier      : Anatoly Sharapov
rem Email         : anatoly.a.sharapov@gmail.com
rem Repository    : https://github.com
rem License       : MIT
rem
rem Date Created  : August 7, 2026
rem Last Modified : August 7, 2026
rem Version       : 1.0.0
rem ===================================================================

setlocal EnableExtensions EnableDelayedExpansion

set "HEADER=1"
set "SEP=0"
for /f "usebackq delims=" %%L in ("%~f0") do (
  if defined HEADER (
    echo %%L
    if "%%L"=="rem ===================================================================" (
      set /a SEP+=1
      if !SEP! EQU 2 set "HEADER="
    )
  )
)

echo.
echo %~n0 started at %TIME% %DATE%
echo.
rem ===================================================================
rem Configuration
rem ===================================================================

set "STM32CUBEIDE=c:\Work\MC\ST\STM32Cube\STM32CubeIDE\2.2.0\STM32CubeIDE\stm32cubeide.exe"
echo STM32CUBEIDE = "%STM32CUBEIDE%"

rem ===================================================================
rem Automatically determine project directory
rem ===================================================================

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
if exist "%SCRIPT_DIR%\.project" (
  set "PROJECT_DIR=%SCRIPT_DIR%"
) else (
  if exist "%SCRIPT_DIR%\..\..\..\.project" (
    for %%I in ("%SCRIPT_DIR%\..\..\..") do set "PROJECT_DIR=%%~fI"
  ) 
)
if not defined PROJECT_DIR (
echo ERROR: STM32CubeIDE project not found.
echo.
echo Expected:
echo %SCRIPT_DIR%\.project
echo or
echo %SCRIPT_DIR%\..\..\..\.project
pause
exit /b 2
)
echo PROJECT_DIR = "%PROJECT_DIR%"

rem ===================================================================
rem Validation
rem ===================================================================
if not exist "%STM32CUBEIDE%" (
  echo ERROR: File not found:
  echo "%STM32CUBEIDE%"
  pause
  exit /b 1
)
rem ===================================================================
rem Run STM32CubeIDE and Import Project
rem ===================================================================
echo ===============================================================
echo Launching STM32CubeIDE...
echo ===============================================================
echo.

"%STM32CUBEIDE%" ^
  -import "%PROJECT_DIR%"

set "EXITCODE=%ERRORLEVEL%"
echo.
if %EXITCODE% EQU 0 (
  echo ===============================================================
  echo STM32CubeIDE closed successfully.
  echo ===============================================================
) else (
  echo ===============================================================
  echo STM32CubeIDE closed with error. Exit code: %EXITCODE%
  echo ===============================================================
)
echo.
echo Exit code = %EXITCODE%
pause
exit /b %EXITCODE%
