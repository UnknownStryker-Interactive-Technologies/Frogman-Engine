# Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
SET(LIST_DIRECTORIES true)
GET_FILENAME_COMPONENT(FROGMAN_ENGINE_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)


INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash)
FILE(GLOB THIRD_PARTY_CITY_HASH "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash/*.cc" "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/City-Hash/*.h")


INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/ufbx-0.23.0)
FILE(GLOB THIRD_PARTY_UFBX "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/ufbx-0.23.0/*.c" "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/ufbx-0.23.0/*.h")


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




INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/dr_libs)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/entt-4.0.0/single_include) 
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/entt-4.0.0/src)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glm-1.0.3)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/hat-trie-0.7.1/include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/backends)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/lib)
# INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/simdjson-4.2.1/include) # Removed SIMDJSON include to avoid the MSVC C++ compiler bug: https://developercommunity.visualstudio.com/t/The-MSVC-C-v143-compiler-is-unable-to-/10994292
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/stb)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/taskflow-4.0.0/taskflow)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Audio/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include)
INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Renderer/Include)




IF (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")

    IF (${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL "v143")
        SET(BOOST_CHRONO
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_CONTAINER 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_FILESYSTEM
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_LOCALE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_JSON 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_STACKTRACE 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_windbg-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_windbg-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_noop-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_noop-vc143-mt-s-x64-1_91.lib>
        )

        SET(BOOST_THREAD 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc143-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc143-mt-s-x64-1_91.lib>
        )

    ELSEIF (${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL "v145")
        SET(BOOST_CHRONO
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_chrono-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_CONTAINER 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_container-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_FILESYSTEM
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_filesystem-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_LOCALE
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_locale-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_JSON 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_json-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_STACKTRACE 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_windbg-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_windbg-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_noop-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_stacktrace_noop-vc145-mt-s-x64-1_91.lib>
        )

        SET(BOOST_THREAD 
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc145-mt-sgd-x64-1_91.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc145-mt-s-x64-1_91.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.91.0/stage/lib/libboost_thread-vc145-mt-s-x64-1_91.lib>
        )

    ENDIF()

    
    ADD_COMPILE_DEFINITIONS(GLM_FORCE_DEPTH_ZERO_TO_ONE)
    SET(RENDERER_BACKEND d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib uuid.lib version.lib )


    IF(SIMD STREQUAL AVX512F)
        FILE(GLOB ABSL_DEBUG            "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX512F/Debug/*.lib")
        FILE(GLOB ABSL_MINSIZEREL       "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX512F/MinSizeRel/*.lib")
        FILE(GLOB ABSL_RELEASE          "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX512F/Release/*.lib")
        FILE(GLOB ABSL_RELWITHDEBINFO   "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/*.lib")
        SET(ABSL_LIBRARIES
            $<$<CONFIG:DEBUG>:			${ABSL_DEBUG}>
            $<$<CONFIG:RELWITHDEBINFO>: ${ABSL_RELWITHDEBINFO}>
            $<$<CONFIG:RELEASE>:		${ABSL_RELEASE}>
            $<$<CONFIG:MINSIZEREL>:		${ABSL_MINSIZEREL}>
        )

        SET(GLFW
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX512F/Debug/glfw3.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/glfw3.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX512F/Release/glfw3.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX512F/MinSizeRel/glfw3.lib>
        )

        SET(IMGUI
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/Debug/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/Release/ImGUI-1.91.6.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX512F/MinSizeRel/ImGUI-1.91.6.lib>
        )

        SET(LZ4
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX512F/Debug/lz4.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX512F/RelWithDebInfo/lz4.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX512F/Release/lz4.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX512F/MinSizeRel/lz4.lib>
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
        FILE(GLOB ABSL_DEBUG            "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX-SSE2/Debug/*.lib")
        FILE(GLOB ABSL_MINSIZEREL       "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/*.lib")
        FILE(GLOB ABSL_RELEASE          "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX-SSE2/Release/*.lib")
        FILE(GLOB ABSL_RELWITHDEBINFO   "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/abseil-cpp-20260107.1/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/*.lib")
        SET(ABSL_LIBRARIES
            $<$<CONFIG:DEBUG>:			${ABSL_DEBUG}>
            $<$<CONFIG:RELWITHDEBINFO>: ${ABSL_RELWITHDEBINFO}>
            $<$<CONFIG:RELEASE>:		${ABSL_RELEASE}>
            $<$<CONFIG:MINSIZEREL>:		${ABSL_MINSIZEREL}>
        )

        SET(GLFW
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX-SSE2/Debug/glfw3.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/glfw3.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX-SSE2/Release/glfw3.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/glfw-3.4/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/glfw3.lib>
        )

        SET(IMGUI
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/Debug/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/ImGUI-1.91.6.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/Release/ImGUI-1.91.6.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/imgui-1.91.6/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/ImGUI-1.91.6.lib>
        )

        SET(LZ4
            $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVAVX-SSE2X512F/Debug/lz4.lib>
            $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX-SSE2/RelWithDebInfo/lz4.lib>
            $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX-SSE2/Release/lz4.lib>
            $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/lz4-1.10.0/Binaries/X86-64/Windows/AVX-SSE2/MinSizeRel/lz4.lib>
        )

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
        SET(PATH_TO_HEADER_TOOL ${FROGMAN_ENGINE_CMAKE_DIR}/../Header-Tool/Binaries/RelWithDebInfo/FE.HeaderTool.exe)

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
            ARGS ${HEADER_TOOL_PROGRAM_OPTIONS} "-path-to-project=${CMAKE_CURRENT_SOURCE_DIR}" "${HEADER_FILES_PATHS}"
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

	IF( SHIPPING_BUILD STREQUAL "ON" )
        ADD_COMPILE_DEFINITIONS(_FE_SHIPPING_BUILD_)
		ADD_LINK_OPTIONS(/SUBSYSTEM:WINDOWS)
	ENDIF()


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


    FILE(GLOB_RECURSE SHADER_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.hlsli")
    SOURCE_GROUP("Shader Headers" FILES ${SHADER_HEADER_FILES})

    FILE(GLOB_RECURSE VERTEX_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.vertex.hlsl")
    FILE(GLOB_RECURSE PIXEL_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.pixel.hlsl")
    FILE(GLOB_RECURSE GEOMETRY_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.geometry.hlsl")
    FILE(GLOB_RECURSE HULL_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.hull.hlsl")
    FILE(GLOB_RECURSE DOMAIN_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.domain.hlsl")
    FILE(GLOB_RECURSE COMPUTE_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.compute.hlsl")

    SET(SHADER_SOURCE_FILES
    ${VERTEX_SHADER_FILES}
    ${PIXEL_SHADER_FILES}
    ${GEOMETRY_SHADER_FILES}
    ${HULL_SHADER_FILES}
    ${DOMAIN_SHADER_FILES}
    ${COMPUTE_SHADER_FILES}
    )
    SOURCE_GROUP("Shader Sources" FILES ${SHADER_SOURCE_FILES})
    ADD_LIBRARY("Shaders" INTERFACE ${SHADER_HEADER_FILES} ${SHADER_SOURCE_FILES})


    ADD_LINK_OPTIONS(/MANIFEST:EMBED /MANIFESTUAC:level='requireAdministrator')

    ADD_EXECUTABLE(${TARGET_PROJECT_NAME} ${ARGN} ${PRIVATE})

ENDFUNCTION()




FUNCTION(SET_SHADERS)

    FILE(GLOB_RECURSE SHADER_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.hlsli")

    FILE(GLOB_RECURSE VERTEX_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.vertex.hlsl")
    FILE(GLOB_RECURSE PIXEL_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.pixel.hlsl")
    FILE(GLOB_RECURSE GEOMETRY_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.geometry.hlsl")
    FILE(GLOB_RECURSE HULL_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.hull.hlsl")
    FILE(GLOB_RECURSE DOMAIN_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.domain.hlsl")
    FILE(GLOB_RECURSE COMPUTE_SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/*.compute.hlsl")

    MESSAGE(STATUS "Found ${SHADER_HEADER_FILES} shader header files.")    
    MESSAGE(STATUS "Found ${VERTEX_SHADER_FILES} vertex shader files.")
    MESSAGE(STATUS "Found ${PIXEL_SHADER_FILES} pixel shader files.")
    MESSAGE(STATUS "Found ${GEOMETRY_SHADER_FILES} geometry shader files.")
    MESSAGE(STATUS "Found ${HULL_SHADER_FILES} hull shader files.")
    MESSAGE(STATUS "Found ${DOMAIN_SHADER_FILES} domain shader files.")
    MESSAGE(STATUS "Found ${COMPUTE_SHADER_FILES} compute shader files.")

    SET_SOURCE_FILES_PROPERTIES(${VERTEX_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Vertex
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT vs_main
        VS_SHADER_FLAGS /WX
    )

    SET_SOURCE_FILES_PROPERTIES(${PIXEL_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Pixel
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT ps_main
        VS_SHADER_FLAGS /WX
    )

    SET_SOURCE_FILES_PROPERTIES(${GEOMETRY_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Geometry
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT gs_main
        VS_SHADER_FLAGS /WX
    )

    SET_SOURCE_FILES_PROPERTIES(${HULL_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Hull
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT hs_main
        VS_SHADER_FLAGS /WX
    )

    SET_SOURCE_FILES_PROPERTIES(${DOMAIN_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Domain
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT ds_main
        VS_SHADER_FLAGS /WX
    )

    SET_SOURCE_FILES_PROPERTIES(${COMPUTE_SHADER_FILES} PROPERTIES
        VS_SHADER_TYPE  Compute
        VS_SHADER_MODEL 5.0
        VS_SHADER_ENTRYPOINT cs_main
        VS_SHADER_FLAGS /WX
    )

    FILE(GLOB FROGGY "${CMAKE_CURRENT_SOURCE_DIR}/../*.froggy")
    LIST(LENGTH FROGGY N)
    IF(NOT N EQUAL 1)
        MESSAGE(FATAL_ERROR "There should be exactly one .froggy file in the root directory of the project.")
    ENDIF()
    FILE(READ "${FROGGY}" FROGGY_BUFFER)

    # Snapshot before clearing Shaders, so user-edited fields can be carried over.
    SET(ORIGINAL_FROGGY_BUFFER "${FROGGY_BUFFER}")

    # STRING(JSON <out-var> [ERROR_VARIABLE <error-var>] SET <json-string> <member|index> [<member|index> ...] <value>)
    STRING(JSON FROGGY_BUFFER SET "${FROGGY_BUFFER}" "Shaders" "[]")

    SET(SHADER_INDEX 0)

    MACRO(APPEND_SHADERS SHADER_FILES SHADER_TARGET MAIN_FUNCTION)
        FOREACH(SF ${SHADER_FILES})
            FILE(TO_NATIVE_PATH "${SF}" SF)
            STRING(REPLACE "\\" "\\\\" SF_ESC "${SF}")

            # Start OBJ from the matching original entry (by Source path).
            # This carries over Defines / PermutationBlacklist and any other
            # user-added fields without per-key copy hazards.
            SET(OBJ "{}")
            STRING(JSON ORIG_COUNT ERROR_VARIABLE ORIG_ERR LENGTH "${ORIGINAL_FROGGY_BUFFER}" "Shaders")
            IF(NOT ORIG_ERR AND ORIG_COUNT GREATER 0)
                MATH(EXPR LAST_IDX "${ORIG_COUNT} - 1")
                FOREACH(I RANGE ${LAST_IDX})
                    STRING(JSON ORIG_SOURCE ERROR_VARIABLE ORIG_ERR GET "${ORIGINAL_FROGGY_BUFFER}" "Shaders" ${I} "Source")
                    IF(NOT ORIG_ERR AND ORIG_SOURCE STREQUAL "${SF}")
                        STRING(JSON ORIG_OBJ ERROR_VARIABLE ORIG_ERR GET "${ORIGINAL_FROGGY_BUFFER}" "Shaders" ${I})
                        IF(NOT ORIG_ERR)
                            SET(OBJ "${ORIG_OBJ}")
                        ENDIF()
                        BREAK()
                    ENDIF()
                ENDFOREACH()
            ENDIF()

            STRING(JSON OBJ SET "${OBJ}" "Source"       "\"${SF_ESC}\"")
            STRING(JSON OBJ SET "${OBJ}" "MainFunction" "\"${MAIN_FUNCTION}\"")
            STRING(JSON OBJ SET "${OBJ}" "ShaderTarget" "\"${SHADER_TARGET}\"")

            STRING(JSON TYPE_INFO ERROR_VARIABLE ERROR TYPE "${OBJ}" "Defines")
            IF(ERROR OR NOT TYPE_INFO STREQUAL "ARRAY")
                STRING(JSON OBJ SET "${OBJ}" "Defines" "[]")
            ENDIF()

            STRING(JSON TYPE_INFO ERROR_VARIABLE ERROR TYPE "${OBJ}" "PermutationBlacklist")
            IF(ERROR OR NOT TYPE_INFO STREQUAL "ARRAY")
                STRING(JSON OBJ SET "${OBJ}" "PermutationBlacklist" "[]")
            ENDIF()

            STRING(JSON FROGGY_BUFFER SET "${FROGGY_BUFFER}" "Shaders" ${SHADER_INDEX} "${OBJ}")
            MATH(EXPR SHADER_INDEX "${SHADER_INDEX} + 1")
        ENDFOREACH()
    ENDMACRO()

    APPEND_SHADERS("${VERTEX_SHADER_FILES}"   "vs_5_0" "vs_main")
    APPEND_SHADERS("${PIXEL_SHADER_FILES}"    "ps_5_0" "ps_main")
    APPEND_SHADERS("${GEOMETRY_SHADER_FILES}" "gs_5_0" "gs_main")
    APPEND_SHADERS("${HULL_SHADER_FILES}"     "hs_5_0" "hs_main")
    APPEND_SHADERS("${DOMAIN_SHADER_FILES}"   "ds_5_0" "ds_main")
    APPEND_SHADERS("${COMPUTE_SHADER_FILES}"  "cs_5_0" "cs_main")

    STRING(JSON FROGGY_BUFFER SET "${FROGGY_BUFFER}" "ShaderHeaders" "[]")
    SET(HEADER_INDEX 0)
    FOREACH(H ${SHADER_HEADER_FILES})
        FILE(TO_NATIVE_PATH "${H}" H)
        STRING(REPLACE "\\" "\\\\" H_ESC "${H}")
        STRING(JSON FROGGY_BUFFER SET "${FROGGY_BUFFER}" "ShaderHeaders" ${HEADER_INDEX} "\"${H_ESC}\"")
        MATH(EXPR HEADER_INDEX "${HEADER_INDEX} + 1")
    ENDFOREACH()

    FILE(WRITE "${FROGGY}" "${FROGGY_BUFFER}")

ENDFUNCTION()


SET(FROGMAN_FRAMEWORK_SDK ${ABSL_LIBRARIES} ${BOOST_CHRONO} ${BOOST_CONTAINER} ${BOOST_FILESYSTEM} ${BOOST_JSON} ${BOOST_LOCALE} ${BOOST_STACKTRACE} ${BOOST_THREAD} ${FE_CORE} ${FE_FRAMEWORK} )
SET(FROGMAN_ENGINE_SDK ${FROGMAN_FRAMEWORK_SDK} ${FE_ENGINE} ${FE_RENDERER} ${RENDERER_BACKEND} ${GLFW} ${IMGUI} )
