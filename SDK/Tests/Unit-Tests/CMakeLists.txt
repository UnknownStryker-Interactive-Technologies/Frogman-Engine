CMAKE_MINIMUM_REQUIRED(VERSION 3.26.0...4.3.0)
# Copyright © 2023~ UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
PROJECT(FE_UNIT_TEST
        VERSION 0.0
        DESCRIPTION "A group of executable unit test code binaries"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/frogman-engine.cmake")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/benchmark-1.9.1/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/googletest-1.16.0/googletest/include)




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")

    SET(UNIT_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/googletest-1.16.0/build/lib/Debug/gtest.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/googletest-1.16.0/build/lib/Release/gtest.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/googletest-1.16.0/build/lib/Release/gtest.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/googletest-1.16.0/build/lib/Release/gtest.lib>
    )

    ADD_COMPILE_OPTIONS(/DBENCHMARK_STATIC_DEFINE)
    LINK_LIBRARIES(advapi32.lib shlwapi.lib)
    SET(BENCHMARK_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/benchmark-1.9.1/build/src/Debug/benchmark.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/benchmark-1.9.1/build/src/Release/benchmark.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/benchmark-1.9.1/build/src/Release/benchmark.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/benchmark-1.9.1/build/src/Release/benchmark.lib>
    )

ENDIF()


FILE(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


ADD_LINK_OPTIONS(/NODEFAULTLIB:MSVCRTD)
#RUN_FROGMAN_HEADER_TOOL(${CMAKE_CURRENT_SOURCE_DIR}/FE.ECS.hpp -fno-copyright-notice) 
ADD_FROGMAN_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${FROGMAN_ENGINE_SDK} ${UNIT_TEST} ${BENCHMARK_TEST})
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Test-App")