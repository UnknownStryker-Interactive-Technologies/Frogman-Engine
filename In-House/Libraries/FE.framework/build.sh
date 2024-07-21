WORKING_DIRECTORY=$(dirname "$0")
if [ "$WORKING_DIRECTORY" = '.' ];
    then
    WORKING_DIRECTORY=$(pwd)
fi


BUILD_FILES_FOLDER_NAME=build

if [ -d "$WORKING_DIRECTORY/$BUILD_FILES_FOLDER_NAME" ]; 
    then
    rm -r "$WORKING_DIRECTORY/$BUILD_FILES_FOLDER_NAME"
    echo "Frogman Engine Build: re-building libraries"
fi
mkdir "$WORKING_DIRECTORY/$BUILD_FILES_FOLDER_NAME"


PWD=$(pwd)
cd "$WORKING_DIRECTORY/$BUILD_FILES_FOLDER_NAME"
cmake -DTARGET_ARCHITECTURE="x86-64" -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_CXX_COMPILER=clang-12 -DCMAKE_CXX_COMPILER=clang++-12 -DCMAKE_CXX_STANDARD=20 .. -G "Ninja Multi-Config"
cmake --build . --config Debug -j4
cmake --build . --config MinSizeRel -j4
cmake --build . --config Release -j4
cmake --build . --config RelWithDebInfo -j4
cd $PWD