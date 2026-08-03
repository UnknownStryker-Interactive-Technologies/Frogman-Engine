setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX
cd Solution_X64_AVX

cmake -A x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G "Visual Studio 18 2026" -T "v145"
cmake --build . --config Debug -j16
cmake --build . --config RelWithDebInfo -j16
cmake --build . --config MinSizeRel -j16
cmake --build . --config Release -j16




cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX512F
cd Solution_X64_AVX512F

cmake -A x64 -DSIMD=AVX512F -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G "Visual Studio 18 2026" -T "v145"
cmake --build . --config Debug -j16
cmake --build . --config RelWithDebInfo -j16
cmake --build . --config MinSizeRel -j16
cmake --build . --config Release -j16

endlocal
