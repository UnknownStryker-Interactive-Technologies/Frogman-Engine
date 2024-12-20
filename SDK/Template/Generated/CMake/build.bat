setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution
cd Solution

cmake -DTARGET_CPU_ARCHITECTURE="x86-64" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 -DFROGMAN_ENGINE_SDK_DIR=C:/Users/leeho/OneDrive/Documents/GitHub/Frogman-Engine/SDK .. -G "Visual Studio 17 2022" -T "v143"
cmake --build . --config Debug -j8
cmake --build . --config RelWithDebInfo -j8
cmake --build . --config MinSizeRel -j8
cmake --build . --config Release -j8

endlocal