@echo off

rem ===================================================================
rem Script Name   : stm32cubeidec_cleanBuild_Release.bat
rem                 STM32CubeIDE headless Release build batch file
rem
rem Description   : Automatically detects the STM32CubeIDE project,
rem                 imports it into a temporary workspace and performs
rem                 a clean Release build using stm32cubeidec.exe.
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
rem Date Created  : August 4, 2026
rem Last Modified : August 4, 2026
rem Version       : 2.1.0
rem
rem Change History:
rem -------------------------------------------------------------------
rem Version 2.1.0 | Aug 4, 2026 | Switched to unique temporary workspace.
rem                             | Automatic workspace cleanup.
rem                             | Added workspace creation validation.
rem Version 2.0.0 | Aug 4, 2026 | Automatic project detection.
rem                             | Project name is read from .project.
rem                             | Added configuration validation.
rem                             | Added variable display.
rem                             | Added proper exit code.
rem Version 1.1.0 | Aug 4, 2026 | Added configuration validation,
rem                             | variable echo and proper exit code.
rem Version 1.0.0 | Aug 4, 2026 | Initial release.
rem ===================================================================

@echo off
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

set "STM32CUBEIDEC=c:\Work\MC\ST\STM32Cube\STM32CubeIDE\2.2.0\STM32CubeIDE\stm32cubeidec.exe"
echo STM32CUBEIDEC = "%STM32CUBEIDEC%"

rem ===================================================================
rem Automatically determine project directory
rem ===================================================================

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

if exist "%SCRIPT_DIR%\.project" (
    set "PROJECT_DIR=%SCRIPT_DIR%"
) else (
    if exist "%SCRIPT_DIR%\..\.project" (
        for %%I in ("%SCRIPT_DIR%\..") do set "PROJECT_DIR=%%~fI"
    )
)

if not defined PROJECT_DIR (
    echo ERROR: STM32CubeIDE project not found.
    echo.
    echo Expected:
    echo    %SCRIPT_DIR%\.project
    echo or
    echo    %SCRIPT_DIR%\..\.project
    pause
    exit /b 2
)

echo PROJECT_DIR   = "%PROJECT_DIR%"

rem ===================================================================
rem Read project name from .project
rem ===================================================================

set "PROJECT_NAME="

for /f "usebackq delims=" %%A in (`
powershell -NoProfile -Command ^
 "[xml]$xml = Get-Content -LiteralPath '%PROJECT_DIR%\.project'; $xml.projectDescription.name"
`) do (
    set "PROJECT_NAME=%%A"
)

if not defined PROJECT_NAME (
    echo ERROR: Cannot read project name from:
    echo        "%PROJECT_DIR%\.project"
    pause
    exit /b 3
)

echo PROJECT_NAME  = "%PROJECT_NAME%"

rem ===================================================================
rem Create temporary workspace
rem ===================================================================

set "TS=%DATE:~-4%%DATE:~3,2%%DATE:~0,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%"
set "TS=%TS: =0%"
set "TS=%TS::=%"

set "WORKSPACE=%TEMP%\STM32CubeIDE_%TS%"
echo WORKSPACE     = "%WORKSPACE%"
if exist "%WORKSPACE%" rd /s /q "%WORKSPACE%"

mkdir "%WORKSPACE%"

if errorlevel 1 (
    echo ERROR: Cannot create workspace:
    echo        "%WORKSPACE%"
    pause
    exit /b 4
)

echo Created.
echo.

rem ===================================================================
rem Validation
rem ===================================================================

if not exist "%STM32CUBEIDEC%" (
    echo ERROR: File not found:
    echo        "%STM32CUBEIDEC%"
    pause
    exit /b 1
)

rem ===================================================================
rem Clean Release Build
rem ===================================================================

echo ===============================================================
echo Building Release configuration...
echo ===============================================================
echo.

"%STM32CUBEIDEC%" ^
    --launcher.suppressErrors ^
    -nosplash ^
    -application org.eclipse.cdt.managedbuilder.core.headlessbuild ^
    -data "%WORKSPACE%" ^
    -import "%PROJECT_DIR%" ^
    -cleanBuild "%PROJECT_NAME%/Release"

set "EXITCODE=%ERRORLEVEL%"

echo.

if %EXITCODE% EQU 0 (
    echo ===============================================================
    echo Build completed successfully.
    echo ===============================================================
) else (
    echo ===============================================================
    echo Build FAILED. Exit code: %EXITCODE%
    echo ===============================================================
)

echo.

if exist "%WORKSPACE%" (
    echo Removing temporary workspace...
    rd /s /q "%WORKSPACE%"
)

echo.
echo Exit code = %EXITCODE%

pause
exit /b %EXITCODE%