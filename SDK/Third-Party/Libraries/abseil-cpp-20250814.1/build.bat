setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX
cd Solution_X64_AVX

cmake -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%SCRIPT_DIRECTORY%/Binaries/X86-64/Windows/AVX-SSE2" -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G "Visual Studio 17 2022" -T "v143" -DABSL_MSVC_STATIC_RUNTIME=ON
cmake --build . --config Debug 
cmake --build . --config RelWithDebInfo
cmake --build . --config MinSizeRel
cmake --build . --config Release




cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX512F
cd Solution_X64_AVX512F

cmake -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%SCRIPT_DIRECTORY%/Binaries/X86-64/Windows/AVX512F" -DCMAKE_SYSTEM_PROCESSOR=x64 -DSIMD=AVX512F -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G "Visual Studio 17 2022" -T "v143" -DABSL_MSVC_STATIC_RUNTIME=ON
cmake --build . --config Debug
cmake --build . --config RelWithDebInfo
cmake --build . --config MinSizeRel
cmake --build . --config Release

endlocal
