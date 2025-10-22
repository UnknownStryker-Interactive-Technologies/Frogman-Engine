CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0...4.1.2)
# Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

PROJECT(2DShooterDemo
        VERSION 0.0
        DESCRIPTION "Frogman Engine based game application"
        LANGUAGES CXX)
GET_FILENAME_COMPONENT(FE_CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)

INCLUDE("${FE_CMAKE_CURRENT_LIST_DIR}/../../../../CMake/predefined-settings.cmake")	   
INCLUDE("${FE_CMAKE_CURRENT_LIST_DIR}/../../../../CMake/frogman-engine.cmake")








# INCLUDE_DIRECTORIES(folder/path/to/your/favorite/library)
INCLUDE_DIRECTORIES(${FE_CMAKE_CURRENT_LIST_DIR}/../Include)

FILE(GLOB_RECURSE HEADER_FILES "${FE_CMAKE_CURRENT_LIST_DIR}/../Include/*.h" "${FE_CMAKE_CURRENT_LIST_DIR}/../Include/*.hpp" "${FE_CMAKE_CURRENT_LIST_DIR}/../Include/*.hxx")
FILE(GLOB_RECURSE SOURCE_FILES "${FE_CMAKE_CURRENT_LIST_DIR}/../Source/*.cpp")

# You can use the FHT to check the presence of a copyright notice in your header files; the FHT will abort the build if the copyright notice is missing.
# -fno-reflection-helper: disables the generation of reflection helper code.
# -fno-copyright-notice: disables the copyright notice check.
# -max-concurrency=8: sets the maximum number of concurrent threads to 8 for processing the files.
# -path-to-copyright-notice=PATH: specifies the path to the copyright notice file to be used for checking.
# -fno-op: does not perform any operation.
# -fno-write: does not write any output files.
RUN_FROGMAN_HEADER_TOOL(${HEADER_FILES} -fno-copyright-notice -max-concurrency=8 )

ADD_FROGMAN_EXECUTABLE(${CMAKE_PROJECT_NAME} ${HEADER_FILES}
    ${SOURCE_FILES}
)








SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FE_CMAKE_CURRENT_LIST_DIR}/../Binaries"
VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${FROGMAN_ENGINE_SDK})
