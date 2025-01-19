CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0...3.31.2)
# Copyright © 2024 to present UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FrogmanEngineTemplate
        VERSION 0.0
        DESCRIPTION "Frogman Engine Project Template"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/frogman-engine.cmake")

RUN_FROGMAN_HEADER_TOOL("C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/FE-HT-Test/HeaderWithoutCopyright.hpp" -path-to-copyright-notice=C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/FE-HT-Test/LICENSE.txt)


ADD_FROGMAN_EXECUTABLE(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Source/main.cpp
)

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${FROGMAN_ENGINE_SDK})

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../Binaries")



