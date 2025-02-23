cmake -E make_directory "build"
cd build

cmake ..
cmake --build . --config Release
cmake --build . --config Debug 