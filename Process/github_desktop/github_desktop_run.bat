@echo off

rem ===================================================================
rem Script Name   : github_desktop_run.bat
rem                 GitHub Desktop launch batch file
rem
rem Description   : Automatically verifies the existence of GitHub Desktop,
rem                 logs the process to the console, and launches the app.
rem
rem Location      : Any directory or Desktop.
rem
rem Usage         : Double-click the script to run GitHub Desktop.
rem
rem Author        : ChatGPT (OpenAI)
rem Verifier      : Anatoly Sharapov
rem Email         : anatoly.a.sharapov@gmail.com
rem Repository    : https://github.com
rem License       : MIT
rem
rem Date Created  : August 7, 2026
rem Last Modified : August 7, 2026
rem Version       : 1.1.0
rem
rem Change History:
rem -------------------------------------------------------------------
rem Version 1.1.0 | Aug 7, 2026 | Added file existence check and
rem                             | detailed console logging.
rem Version 1.0.0 | Aug 7, 2026 | Initial script creation for 
rem                             | GitHub Desktop launching.
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
echo %~n0 [INFO] Starting GitHub Desktop script at %TIME% %DATE%
echo.

set "APP_PATH=C:\Users\Anatoly\AppData\Local\GitHubDesktop\GitHubDesktop.exe"
echo APP_PATH = "%APP_PATH%"

rem Проверка существования файла
if not exist "%APP_PATH%" (
  echo [%DATE% %TIME%] [ERROR] GitHub Desktop executable not found at:
  echo           "%APP_PATH%"
  echo [%DATE% %TIME%] [ERROR] Please check the path and try again.
  echo.
  pause
  endlocal
  exit /b 1
)

echo [%DATE% %TIME%] [INFO] Executable found successfully.
echo [%DATE% %TIME%] [INFO] Launching GitHub Desktop...

rem Запуск приложения
start "" "%APP_PATH%"

echo [%DATE% %TIME%] [SUCCESS] Application launched successfully.
echo [%DATE% %TIME%] [INFO] Closing script in 3 seconds...

rem Небольшая пауза, чтобы успеть прочитать логи перед закрытием
timeout /t 3 >nul

endlocal
pause
exit /b 0
