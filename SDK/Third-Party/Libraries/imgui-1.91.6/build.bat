setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64
cd Solution_X64

cmake -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G "Visual Studio 17 2022" -T "v143"
cmake --build . --config Debug -j8
cmake --build . --config RelWithDebInfo -j8
cmake --build . --config MinSizeRel -j8
cmake --build . --config Release -j8

endlocal