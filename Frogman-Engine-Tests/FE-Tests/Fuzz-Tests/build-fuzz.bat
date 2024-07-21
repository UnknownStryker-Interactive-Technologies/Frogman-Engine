@echo off
setlocal

set "WORKING_DIRECTORY=%~dp0"
if "%WORKING_DIRECTORY:~2%" == "" (
    set "WORKING_DIRECTORY=%CD%"
)

echo Working Directory: %WORKING_DIRECTORY%

set "BUILD_FILES_FOLDER_NAME=build"


if exist "%WORKING_DIRECTORY%\%BUILD_FILES_FOLDER_NAME%\" (
    rmdir /s /q "%WORKING_DIRECTORY%\%BUILD_FILES_FOLDER_NAME%"
    echo Frogman Engine Tests: re-building tests
)
mkdir "%WORKING_DIRECTORY%\%BUILD_FILES_FOLDER_NAME%"


if exist "%WORKING_DIRECTORY%\binaries\" (
    rmdir /s /q "%WORKING_DIRECTORY%\binaries"
)
mkdir "%WORKING_DIRECTORY%\binaries"


:: Save the current directory
set "PWD=%CD%"

:: Change the directory to the build folder
cd /d "%WORKING_DIRECTORY%\%BUILD_FILES_FOLDER_NAME%"

:: Run CMake configuration
cmake -DCMAKE_SYSTEM_NAME=Windows .. -G "Visual Studio 17 2022" -T "clangcl"

:: Change back to the original directory
cd /d "%PWD%"
endlocal
