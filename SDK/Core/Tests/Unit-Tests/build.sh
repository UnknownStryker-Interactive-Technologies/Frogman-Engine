#!/bin/bash
WORKING_DIRECTORY=$(dirname "$(realpath "$0")")
#CMAKE_DIRECTORY=$WORKING_DIRECTORY/../../Third-Party/Tools/X86-64/CMake/cmake-3.29.3-linux-x86_64/bin/cmake

PWD=$(pwd)
cd $WORKING_DIRECTORY
mkdir Solution
cd Solution

cmake -DTARGET_CPU_ARCHITECTURE="x86-64" -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_CXX_COMPILER=clang-12 -DCMAKE_CXX_COMPILER=clang++-12 -DCMAKE_CXX_STANDARD=20 .. -G "Ninja Multi-Config"
cmake --build . --config Debug -j8
cmake --build . --config RelWithDebInfo -j8
cmake --build . --config MinSizeRel -j8
cmake --build . --config Release -j8

cd $PWD