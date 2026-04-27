setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64
cd Solution_X64

cmake -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%SCRIPT_DIRECTORY%/Binaries/X86-64/Windows/AVX-SSE2" -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_STANDARD=17 .. -G "Visual Studio 17 2022" -T "v143" -DGLFW_LIBRARY_TYPE=STATIC -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_INSTALL=OFF
cmake --build . --config Debug --parallel
cmake --build . --config RelWithDebInfo --parallel
cmake --build . --config MinSizeRel --parallel
cmake --build . --config Release --parallel




cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX512F
cd Solution_X64_AVX512F

cmake -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%SCRIPT_DIRECTORY%/Binaries/X86-64/Windows/AVX512F" -DCMAKE_SYSTEM_PROCESSOR=x64 -DSIMD=AVX512F -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_STANDARD=17 .. -G "Visual Studio 17 2022" -T "v143" -DGLFW_LIBRARY_TYPE=STATIC -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_INSTALL=OFF
cmake --build . --config Debug --parallel
cmake --build . --config RelWithDebInfo --parallel
cmake --build . --config MinSizeRel --parallel
cmake --build . --config Release --parallel

endlocal
