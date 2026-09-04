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


# oneTBB is optional, and it is the build system rather than a header probe that decides whether the TBB benchmark
# rows are compiled in: only CMake knows whether tbbmalloc reached the link line. Directory level commands are used
# here on purpose. TARGET_LINK_LIBRARIES below uses the plain signature, and CMake rejects mixing the plain and
# keyword forms on one target, so LINK_LIBRARIES / ADD_COMPILE_DEFINITIONS stay out of that conflict. They only
# apply to targets created after this point, which is why the block sits above ADD_FROGMAN_EXECUTABLE.
# TBB::tbbmalloc carries its own include directory, so no INCLUDE_DIRECTORIES line is needed for it.
FIND_PACKAGE(TBB CONFIG QUIET COMPONENTS tbbmalloc)

IF(TBB_FOUND AND TARGET TBB::tbbmalloc)
    MESSAGE(STATUS "oneTBB found: the TBB benchmark rows are enabled")
    LINK_LIBRARIES(TBB::tbbmalloc)
    ADD_COMPILE_DEFINITIONS(FE_HAS_TBB_SCALABLE_MEMORY_RESOURCE=1)
ELSE()
    MESSAGE(STATUS "oneTBB not found: the TBB benchmark rows are compiled out")
    ADD_COMPILE_DEFINITIONS(FE_HAS_TBB_SCALABLE_MEMORY_RESOURCE=0)
ENDIF()


FILE(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


ADD_LINK_OPTIONS(/NODEFAULTLIB:MSVCRTD)

ADD_FROGMAN_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${FROGMAN_ENGINE_SDK} ${UNIT_TEST} ${BENCHMARK_TEST})


SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Test-App/AVX-SSE")


# tbbmalloc has to be loadable at program start, not just at link time. This has to follow SET_TARGET_PROPERTIES so
# that the DLL lands in Test-App/AVX-SSE rather than the default output directory. Whichever file appears there,
# tbbmalloc.dll or tbbmalloc_debug.dll, also tells you which configuration was selected.
IF(TBB_FOUND AND TARGET TBB::tbbmalloc AND WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:TBB::tbbmalloc> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
ENDIF()