CMAKE_MINIMUM_REQUIRED(VERSION 3.27.0...3.29.6)
# Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
PROJECT(FE-asm
        VERSION 0.0
        DESCRIPTION "This library is a part of the Frogman Engine, and it includes memcpy, memset, memmove implementation in assembly language."
        LANGUAGES CXX)
        
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CMakeLists/frogman-engine.cmake")


ADD_LIBRARY(${CMAKE_PROJECT_NAME} STATIC ${FE_ASM})

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES 
<<<<<<< HEAD
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/x86-64/linux/
=======
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
)


ADD_SUBDIRECTORY(tests)