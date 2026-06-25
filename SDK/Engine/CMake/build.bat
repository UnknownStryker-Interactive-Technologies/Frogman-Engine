setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX
cd Solution_X64_AVX

cmake -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G %1 -T %2
cmake --build . --config Debug -j8
cmake --build . --config RelWithDebInfo -j8
cmake --build . --config MinSizeRel -j8
cmake --build . --config Release -j8




cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX512F
cd Solution_X64_AVX512F

cmake -DCMAKE_SYSTEM_PROCESSOR=x64 -DSIMD=AVX512F -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G %1 -T %2
cmake --build . --config Debug -j8
cmake --build . --config RelWithDebInfo -j8
cmake --build . --config MinSizeRel -j8
cmake --build . --config Release -j8

endlocal
