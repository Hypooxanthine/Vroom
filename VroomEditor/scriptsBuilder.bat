@echo off
setlocal

if "%~1"=="" (
    echo Error: two arguments are needed.
    echo Usage: %~nx0 ^<cmake_path^> ^<build_path^>
    exit /b 1
)
if "%~2"=="" (
    echo Error: two arguments are needed.
    echo Usage: %~nx0 ^<cmake_path^> ^<build_path^>
    exit /b 1
)

"%~1" --build "%~2" --target ScriptsLibrary

echo.
echo Press any key to continue...
pause >nul
