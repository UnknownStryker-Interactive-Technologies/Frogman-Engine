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

FILE(GLOB_RECURSE FE_AUDIO_HEADERS "${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Include/*.h" "${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Include/*.hpp" "${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Include/*.hxx")
FILE(GLOB_RECURSE FE_AUDIO_SOURCES "${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Source/*.cpp")




INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.87.0)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glm-1.0.1)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/hat-trie-0.6.0/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/backends)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/robin-hood-hash)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/robin-map-1.4.0/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/taskflow-3.8.0/taskflow)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/unordered_dense-4.7.0/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Include)




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
    FILE(GLOB ABSL_DEBUG            "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX-SSE2/Debug/*.lib")
    FILE(GLOB ABSL_MINSIZEREL       "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/*.lib")
    FILE(GLOB ABSL_RELEASE          "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX-SSE2/Release/*.lib")
    FILE(GLOB ABSL_RELWITHDEBINFO   "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/*.lib")
    SET(ABSL_LIBRARIES
        $<$<CONFIG:DEBUG>:			${ABSL_DEBUG}>
        $<$<CONFIG:RELWITHDEBINFO>: ${ABSL_RELWITHDEBINFO}>
        $<$<CONFIG:RELEASE>:		${ABSL_RELEASE}>
        $<$<CONFIG:MINSIZEREL>:		${ABSL_MINSIZEREL}>
    )

    SET(ASSIMP
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX-SSE2/Debug/assimp-vc143-mtd.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/assimp-vc143-mt.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX-SSE2/Release/assimp-vc143-mt.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/assimp-vc143-mt.lib>
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
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/Debug/ImGUI-1.91.6.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/ImGUI-1.91.6.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/Release/ImGUI-1.91.6.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/ImGUI-1.91.6.lib>
    )

    SET(RENDERER_BACKEND d3d11.lib dxgi.lib d3dcompiler.lib)

    IF(SIMD STREQUAL AVX512F)
        FILE(GLOB ABSL_DEBUG            "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX512F/Debug/*.lib")
        FILE(GLOB ABSL_MINSIZEREL       "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX512F/MinSizeRel/*.lib")
        FILE(GLOB ABSL_RELEASE          "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX512F/Release/*.lib")
        FILE(GLOB ABSL_RELWITHDEBINFO   "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20250814.1/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/*.lib")
        SET(ABSL_LIBRARIES
            $<$<CONFIG:DEBUG>:			${ABSL_DEBUG}>
            $<$<CONFIG:RELWITHDEBINFO>: ${ABSL_RELWITHDEBINFO}>
            $<$<CONFIG:RELEASE>:		${ABSL_RELEASE}>
            $<$<CONFIG:MINSIZEREL>:		${ABSL_MINSIZEREL}>
        )

        SET(ASSIMP
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX512F/Debug/assimp-vc143-mtd.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/assimp-vc143-mt.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX512F/Release/assimp-vc143-mt.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/assimp-6.0.2/Binaries/X86-64/Windows/AVX512F/MinSizeRel/assimp-vc143-mt.lib>
        )

        SET(IMGUI
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/Debug/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/Release/ImGUI-1.91.6.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/MinSizeRel/ImGUI-1.91.6.lib>
        )

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

        SET(FE_RENDERER
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX512F/Debug/FE.Renderer.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/FE.Renderer.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX512F/Release/FE.Renderer.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX512F/MinSizeRel/FE.Renderer.lib>
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

        SET(FE_RENDERER
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX-SSE2/Debug/FE.Renderer.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/FE.Renderer.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX-SSE2/Release/FE.Renderer.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/FE.Renderer.lib>
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

SET(FROGMAN_ENGINE
${FE_CORE_HEADERS}
${FE_CORE_SOURCES}
${FE_FRAMEWORK_HEADERS}
${FE_FRAMEWORK_SOURCES}
${FE_ENGINE_HEADERS}
${FE_ENGINE_SOURCES}
${FE_RENDERER_HEADERS}
${FE_RENDERER_SOURCES}
)
ADD_LIBRARY("Engine" INTERFACE ${FROGMAN_ENGINE})


SET(PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
SOURCE_GROUP("Private" FILES ${PRIVATE})
ADD_LINK_OPTIONS(/MANIFEST:EMBED /MANIFESTUAC:level='requireAdministrator')
ADD_EXECUTABLE(${TARGET_PROJECT_NAME} ${ARGN} ${PRIVATE})

ENDFUNCTION()




SET(FROGMAN_FRAMEWORK_SDK ${ABSL_LIBRARIES} ${ASSIMP} ${BOOST_CHRONO} ${BOOST_CONTEXT} ${BOOST_FIBER} ${BOOST_LOCALE} ${BOOST_STACKTRACE} ${BOOST_THREAD} ${FE_CORE} ${FE_FRAMEWORK} ${FE_RENDERER} ${RENDERER_BACKEND} ${GLFW} ${IMGUI})
SET(FROGMAN_ENGINE_SDK ${FROGMAN_FRAMEWORK_SDK} ${FE_ENGINE})
