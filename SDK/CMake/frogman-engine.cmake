# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
SET(LIST_DIRECTORIES true)
GET_FILENAME_COMPONENT(FROGMAN_ENGINE_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)


INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash)
FILE(GLOB THIRD_PARTY_CITY_HASH "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash/*.cc" "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash/*.h")


FILE(GLOB_RECURSE FE_CORE_HEADERS "${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/*.h" "${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/*.hpp" "${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/*.hxx")
FILE(GLOB_RECURSE FE_CORE_SOURCES "${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/*.cpp")


FILE(GLOB_RECURSE FE_FRAMEWORK_HEADERS "${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/*.h" "${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/*.hpp" "${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/*.hxx")
FILE(GLOB_RECURSE FE_FRAMEWORK_SOURCES "${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Source/*.cpp")


FILE(GLOB_RECURSE FE_ENGINE_HEADERS "${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/*.h" "${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/*.hpp" "${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/*.hxx")
FILE(GLOB_RECURSE FE_ENGINE_SOURCES "${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/*.cpp")


FILE(GLOB_RECURSE FE_RENDERER_HEADERS "${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Include/*.h" "${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Include/*.hpp" "${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Include/*.hxx")
FILE(GLOB_RECURSE FE_RENDERER_SOURCES "${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Source/*.cpp")




INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glm-1.0.1)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/hat-trie-0.6.0/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/backends)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/robin-hood-hash)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/taskflow-3.8.0/taskflow)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include)




SET(FE_FRAMEWORK_REFLECTION
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/reflection.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/type_info.hpp
)
SOURCE_GROUP("FE.Framework.Reflection" FILES ${FE_FRAMEWORK_REFLECTION})




SET(FE_ALGORITHM_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/string.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/utility.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/math.hpp
)
SOURCE_GROUP("FE.Algorithm" FILES ${FE_ALGORITHM_HEADERS})

SET(FE_LOG_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/log/format_string.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/log/log.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/log/logger.hpp
)
SOURCE_GROUP("FE.Log" FILES ${FE_LOG_HEADERS})

SET(FE_LOG_SOURCES
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/format_string.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/logger.cpp
)

SET(FE_POOL_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/pool/block_pool.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/pool/scalable_pool.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/pool/memory_resource.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/pool/private/pool_common.hxx
)
SOURCE_GROUP("FE.Pool" FILES ${FE_POOL_HEADERS})

SET(FE_MISC_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/private/macro_restrictions.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/define_max_min.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/suppress_warnings.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/undefine_max_min.h
)
SOURCE_GROUP("FE.Misc" FILES ${FE_MISC_HEADERS})




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")

    SET(ASSIMP
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Solution_X64/lib/Debug/assimp-vc143-mtd.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Solution_X64/lib/RelWithDebInfo/assimp-vc143-mt.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Solution_X64/lib/Release/assimp-vc143-mt.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Solution_X64/lib/MinSizeRel/assimp-vc143-mt.lib>
    )

    SET(BOOST_CHRONO 
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_chrono-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_87.lib>
    )

    SET(BOOST_LOCALE
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_locale-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_87.lib>
    )

    SET(BOOST_STACKTRACE 
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_stacktrace_windbg-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_stacktrace_windbg-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_stacktrace_noop-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_stacktrace_noop-vc143-mt-s-x64-1_87.lib>
    )

    SET(BOOST_THREAD 
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_thread-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_87.lib>
    )

    SET(BOOST_FIBER 
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_fiber-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_fiber-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_fiber-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_fiber-vc143-mt-s-x64-1_87.lib>
    )

    SET(BOOST_CONTEXT 
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_context-vc143-mt-sgd-x64-1_87.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_context-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_context-vc143-mt-s-x64-1_87.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0/stage/lib/libboost_context-vc143-mt-s-x64-1_87.lib>
    )

    SET(GLFW
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/lib-vc2022/glfw3_mt.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/lib-vc2022/glfw3_mt.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/lib-vc2022/glfw3_mt.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/lib-vc2022/glfw3_mt.lib>
    )

    SET(IMGUI
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/Debug/ImGUI-1.91.6.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/RelWithDebInfo/ImGUI-1.91.6.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/Release/ImGUI-1.91.6.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/MinSizeRel/ImGUI-1.91.6.lib>
    )

    IF(SIMD STREQUAL AVX512F)
        SET(FE_CORE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX512F/Debug/FE.Core.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/FE.Core.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX512F/Release/FE.Core.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX512F/MinSizeRel/FE.Core.lib>
        )

        SET(FE_FRAMEWORK
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX512F/Debug/FE.Framework.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/FE.Framework.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX512F/Release/FE.Framework.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX512F/MinSizeRel/FE.Framework.lib>
        )

        SET(FE_ENGINE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX512F/Debug/FE.Engine.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/FE.Engine.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX512F/Release/FE.Engine.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX512F/MinSizeRel/FE.Engine.lib>
        )
    ELSE()
        SET(FE_CORE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX-SSE2/Debug/FE.Core.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/FE.Core.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX-SSE2/Release/FE.Core.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/FE.Core.lib>
        )

        SET(FE_FRAMEWORK
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX-SSE2/Debug/FE.Framework.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/FE.Framework.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX-SSE2/Release/FE.Framework.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/FE.Framework.lib>
        )

        SET(FE_ENGINE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX-SSE2/Debug/FE.Engine.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/FE.Engine.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX-SSE2/Release/FE.Engine.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/FE.Engine.lib>
        )
    ENDIF()

ENDIF()




FUNCTION(RUN_FROGMAN_HEADER_TOOL)
    SET(HEADER_TOOL_PROGRAM_OPTIONS)
    SET(HEADER_FILES_PATHS)

    FOREACH(ARGUMENT IN LISTS ARGV)
        IF(ARGUMENT MATCHES "^-")
            LIST(APPEND HEADER_TOOL_PROGRAM_OPTIONS ${ARGUMENT})
        ELSE()
            LIST(APPEND HEADER_FILES_PATHS ${ARGUMENT})
        ENDIF()
    ENDFOREACH()

    SET(RETURN_VALUE_FROM_TOOL)
    SET(TOOL_STDOUT)
    SET(TOOL_STDERR)
        
    MESSAGE("========== Frogman Engine Header Tool ==========")

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")

        # Print paths for debugging
        SET(PATH_TO_HEADER_TOOL ${FROGMAN_ENGINE_CMAKE_DIR}/../Header-Tool/Binaries/Release/FE.HeaderTool.exe)

        MESSAGE(STATUS "The path to Frogman Engine Header Tool is: ${PATH_TO_HEADER_TOOL}")
        MESSAGE(STATUS "The target header files are: ${HEADER_FILES_PATHS}")
        MESSAGE(STATUS "The Header Tool program options are: ${HEADER_TOOL_PROGRAM_OPTIONS}")

        # Verify the existence of the header tool executable
        IF(NOT EXISTS "${PATH_TO_HEADER_TOOL}")
            MESSAGE(FATAL_ERROR "Frogman Engine Header Tool executable not found at ${PATH_TO_HEADER_TOOL}")
        ENDIF()

        # Verify the existence of the target header files
        FOREACH(HEADER_FILE IN LISTS HEADER_FILES_PATHS)
            IF(NOT EXISTS "${HEADER_FILE}")
                MESSAGE(FATAL_ERROR "Target header file not found: ${HEADER_FILE}")
            ENDIF()
        ENDFOREACH()

        # Execute the header tool. 
        EXECUTE_PROCESS(
            COMMAND ${CMAKE_COMMAND} -E env LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 ${PATH_TO_HEADER_TOOL} 
            ARGS ${HEADER_TOOL_PROGRAM_OPTIONS} "-path-to-project=${CMAKE_CURRENT_SOURCE_DIR}" "-fno-code-style-guide" "${HEADER_FILES_PATHS}"
            RESULT_VARIABLE RETURN_VALUE_FROM_TOOL
            OUTPUT_VARIABLE TOOL_STDOUT
            ERROR_VARIABLE TOOL_STDERR
        )

    ENDIF()

    MESSAGE("${TOOL_STDOUT}")
    MESSAGE("${TOOL_STDERR}")

    # Print tool output and error for debugging
    MESSAGE("Frogman Engine Header Tool returned the exit code '${RETURN_VALUE_FROM_TOOL}'.")

    IF(NOT RETURN_VALUE_FROM_TOOL EQUAL 0)
        IF(NOT RETURN_VALUE_FROM_TOOL EQUAL 1006)
            MESSAGE(FATAL_ERROR "------ FE HT: compilation failed! Please, check the messages above! ------")
        ENDIF()
    ENDIF()

    MESSAGE("========== Frogman Engine Header Tool Successfully Processed the Target Header Files ==========")

ENDFUNCTION()


FUNCTION(ADD_FROGMAN_EXECUTABLE TARGET_PROJECT_NAME)
ADD_EXECUTABLE(${TARGET_PROJECT_NAME} ${ARGN} ${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp)
ENDFUNCTION()




SET(FROGMAN_FRAMEWORK_SDK ${ASSIMP} ${BOOST_CHRONO} ${BOOST_CONTEXT} ${BOOST_FIBER} ${BOOST_LOCALE} ${BOOST_STACKTRACE} ${BOOST_THREAD} ${FE_CORE} ${FE_FRAMEWORK} ${GLFW} ${IMGUI})
SET(FROGMAN_ENGINE_SDK ${FROGMAN_FRAMEWORK_SDK} ${FE_ENGINE})
