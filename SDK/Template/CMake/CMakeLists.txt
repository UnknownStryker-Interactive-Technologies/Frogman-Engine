CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0...3.31.2)
# Copyright © 2024 to present UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FrogmanEngineTemplate
        VERSION 0.0
        DESCRIPTION "Frogman Engine Project Template"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/frogman-engine.cmake")




SET(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../Tests/FE-HT-Test/HeaderWithCopyright.hpp)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Source/main.cpp
)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../Binaries")

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} 
${BOOST_STACKTRACE} ${BOOST_CHRONO} ${BOOST_THREAD} 
${GLM} ${GLFW} ${IMGUI}
# the order of library arrangement is important here!
${FE_FRAMEWORK} ${FE_CORE}
#                Depends on
# ${FE_FRAMEWORK}    ->    ${FE_CORE}
)


SET(HT_OPTIONS "-fno-reflection-helper -path-to-copyright-notice=${CMAKE_CURRENT_SOURCE_DIR}/../../Tests/FE-HT-Test/LICENSE.txt")
RUN_HEADER_TOOL(${HEADERS} ${HT_OPTIONS})