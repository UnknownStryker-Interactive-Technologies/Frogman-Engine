setlocal

SET "SCRIPT_DIRECTORY=%~dp0"

cd /d "%SCRIPT_DIRECTORY%"
mkdir Solution_X64_AVX
cd Solution_X64_AVX

cmake -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_CXX_STANDARD=20 .. -G %1 -T %2 -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" -DBUILD_SHARED_LIBS=OFF -DJSONCPP_STATIC_WINDOWS_RUNTIME=ON -DBUILD_OBJECT_LIBS=OFF -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DJSONCPP_WITH_PKGCONFIG_SUPPORT=OFF -DJSONCPP_WITH_CMAKE_PACKAGE=OFF -DJSONCPP_WITH_INSTALL=OFF

cmake --build . --config Debug --parallel
cmake --build . --config RelWithDebInfo --parallel
cmake --build . --config MinSizeRel --parallel
cmake --build . --config Release --parallel

endlocal
