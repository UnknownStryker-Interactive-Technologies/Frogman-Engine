CMAKE_MINIMUM_REQUIRED(VERSION 3.27.0...3.29.6)
# Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FE_UNIT_TEST
        VERSION 0.0
        DESCRIPTION "A group of executable unit test code binaries"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CMakeLists/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CMakeLists/frogman-engine.cmake")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/include)


IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(UNIT_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/linux/libgtest.a>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/linux/libgtest.a>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/linux/libgtest.a>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/linux/libgtest.a>
    )

    ADD_COMPILE_OPTIONS(-DBENCHMARK_STATIC_DEFINE)
    SET(BENCHMARK_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/linux/Debug/libbenchmark.a>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/linux/Release/libbenchmark.a>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/linux/Release/libbenchmark.a>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/linux/Release/libbenchmark.a>
    )
    

ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(UNIT_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/windows/Debug/gtest.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/windows/RelWithDebInfo/gtest.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/windows/Release/gtest.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Google-Test/windows/MinSizeRel/gtest.lib>
    )

    ADD_COMPILE_OPTIONS(/DBENCHMARK_STATIC_DEFINE)
    LINK_LIBRARIES(advapi32.lib shlwapi.lib)
    SET(BENCHMARK_TEST			
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/windows/Debug/benchmark.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/windows/RelWithDebInfo/benchmark.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/windows/Release/benchmark.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/Benchmark/windows/MinSizeRel/benchmark.lib>
    )

ENDIF()


ADD_SUBDIRECTORY(FE.core.prerequisites)
ADD_SUBDIRECTORY(FE.core.type_traits)
ADD_SUBDIRECTORY(FE.core.algorithm.utility)  
ADD_SUBDIRECTORY(FE.core.hash)
ADD_SUBDIRECTORY(FE.core.log)
ADD_SUBDIRECTORY(FE.core.thread)
ADD_SUBDIRECTORY(FE.core.algorithm.math)   
ADD_SUBDIRECTORY(FE.core.iterator)  
ADD_SUBDIRECTORY(FE.core.memory)   
ADD_SUBDIRECTORY(FE.core.algorithm.string)  
ADD_SUBDIRECTORY(FE.core.fstring)
ADD_SUBDIRECTORY(FE.core.pool)
ADD_SUBDIRECTORY(FE.core.managed)
ADD_SUBDIRECTORY(FE.core.string)
ADD_SUBDIRECTORY(FE.core.string_view)
ADD_SUBDIRECTORY(FE.core.containers)
ADD_SUBDIRECTORY(FE.framework.reflection)
ADD_SUBDIRECTORY(FE.framework.predefined_main)
ADD_SUBDIRECTORY(FE.core.adjacency_graph)