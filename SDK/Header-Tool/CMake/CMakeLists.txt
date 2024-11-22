CMAKE_MINIMUM_REQUIRED(VERSION 3.26.0...3.31.0)
# Copyright © 2024 to present UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FE.HeaderTool
        VERSION 0.0
        DESCRIPTION "A group of executable unit test code binaries"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/frogman-engine.cmake")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/include)




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(FE_CORE
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Binaries/X86-64/Windows/Debug/FE.Core.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Binaries/X86-64/Windows//RelWithDebInfo/FE.Core.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Binaries/X86-64/Windows/Release/FE.Core.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Binaries/X86-64/Windows/MinSizeRel/FE.Core.lib>
    )

    SET(FE_FRAMEWORK
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Framework/Binaries/X86-64/Windows/Debug/FE.Framework.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Framework/Binaries/X86-64/Windows//RelWithDebInfo/FE.Framework.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Framework/Binaries/X86-64/Windows/Release/FE.Framework.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Framework/Binaries/X86-64/Windows/MinSizeRel/FE.Framework.lib>
    )

ENDIF()


ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Source/main.cpp
)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../Binaries")
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${BOOST_STACKTRACE} ${BOOST_CHRONO} ${BOOST_THREAD} ${FE_CORE} ${FE_FRAMEWORK})