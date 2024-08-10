CMAKE_MINIMUM_REQUIRED(VERSION 3.26.0...3.30.3)
# Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FE_FUZZ_TEST
        VERSION 0.0
        DESCRIPTION "A group of executable fuzz test code binaries"
        LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../../CMake/predefined-settings.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../../CMake/frogman-engine.cmake")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/X86-64/Google-Test/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Third-Party/Tools/X86-64/Benchmark/include)


#https://llvm.org/docs/LibFuzzer.html?ref=jumpcrypto.com
#-fsanitize=address
#-fsanitize=memory
#-fsanitize=undefined
#-fsanitize=fuzzer
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=fuzzer,undefined,address")




ADD_EXECUTABLE(FE.core.algorithm.string.count_chars
${FE_CORE_SOURCES}
tests/FE.core.algorithm.string.count_chars.cpp
)
SET_TARGET_PROPERTIES(FE.core.algorithm.string.count_chars PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
TARGET_LINK_LIBRARIES(FE.core.algorithm.string.count_chars ${BOOST_STACKTRACE} ${BOOST_CHRONO})


ADD_EXECUTABLE(FE.core.algorithm.string.capitalize
${FE_CORE_SOURCES}
tests/FE.core.algorithm.string.capitalize.cpp
)
SET_TARGET_PROPERTIES(FE.core.algorithm.string.capitalize PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
TARGET_LINK_LIBRARIES(FE.core.algorithm.string.capitalize ${BOOST_STACKTRACE} ${BOOST_CHRONO})


ADD_EXECUTABLE(FE.core.algorithm.string.to_lowercase
${FE_CORE_SOURCES}
tests/FE.core.algorithm.string.to_lowercase.cpp
)
SET_TARGET_PROPERTIES(FE.core.algorithm.string.to_lowercase PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
TARGET_LINK_LIBRARIES(FE.core.algorithm.string.to_lowercase ${BOOST_STACKTRACE} ${BOOST_CHRONO})


ADD_EXECUTABLE(FE.core.algorithm.string.capitalize_every_first_letter_of_words
${FE_CORE_SOURCES}
tests/FE.core.algorithm.string.capitalize_every_first_letter_of_words.cpp
)
SET_TARGET_PROPERTIES(FE.core.algorithm.string.capitalize_every_first_letter_of_words PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
TARGET_LINK_LIBRARIES(FE.core.algorithm.string.capitalize_every_first_letter_of_words ${BOOST_STACKTRACE} ${BOOST_CHRONO})


ADD_EXECUTABLE(FE.core.algorithm.string.length
${FE_CORE_SOURCES}
tests/FE.core.algorithm.string.length.cpp
)
SET_TARGET_PROPERTIES(FE.core.algorithm.string.length PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
TARGET_LINK_LIBRARIES(FE.core.algorithm.string.length ${BOOST_STACKTRACE} ${BOOST_CHRONO})