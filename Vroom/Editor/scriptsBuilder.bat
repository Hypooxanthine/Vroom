@echo off
setlocal

if "%~1"=="" (
    echo Error: three arguments are needed.
    echo Usage: %~nx0 ^<cmake_path^> ^<build_path^> ^<script_library_target_name^>
    exit /b 1
)
if "%~2"=="" (
    echo Error: three arguments are needed.
    echo Usage: %~nx0 ^<cmake_path^> ^<build_path^> ^<script_library_target_name^>
    exit /b 1
)
if "%~3"=="" (
    echo Error: three arguments are needed.
    echo Usage: %~nx0 ^<cmake_path^> ^<build_path^> ^<script_library_target_name^>
    exit /b 1
)

"%~1" --build "%~2" --target "%~3%"
