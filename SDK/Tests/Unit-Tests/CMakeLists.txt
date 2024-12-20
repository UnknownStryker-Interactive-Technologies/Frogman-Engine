CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0...3.31.2)
# Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FE_UNIT_TEST
        VERSION 0.0
        DESCRIPTION "A group of executable unit test code binaries"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/frogman-engine.cmake")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/include)




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(UNIT_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/windows/Debug/gtest.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/windows/RelWithDebInfo/gtest.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/windows/Release/gtest.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Google-Test/windows/MinSizeRel/gtest.lib>
    )

    ADD_COMPILE_OPTIONS(/DBENCHMARK_STATIC_DEFINE)
    LINK_LIBRARIES(advapi32.lib shlwapi.lib)
    SET(BENCHMARK_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/windows/Debug/benchmark.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/windows/RelWithDebInfo/benchmark.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/windows/Release/benchmark.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party/Tools/X86-64/Benchmark/windows/MinSizeRel/benchmark.lib>
    )

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




ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.algorithm.math.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.algorithm.string.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.algorithm.utility.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.fcontainer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.framework.managed.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.framework.reflection.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.hash.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.iterator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.log.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.memory.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.pool.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FE.task.cpp
)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Test-App")
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${BOOST_STACKTRACE} ${BOOST_CHRONO} ${BOOST_THREAD} ${UNIT_TEST} ${BENCHMARK_TEST} ${FE_CORE} ${FE_FRAMEWORK}) 