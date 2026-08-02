setlocal


SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX
cd Solution_X64_AVX

cmake -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G %1 -T %2
cmake --build . --config Debug --parallel
cmake --build . --config RelWithDebInfo --parallel
cmake --build . --config MinSizeRel --parallel
cmake --build . --config Release --parallel


endlocal
