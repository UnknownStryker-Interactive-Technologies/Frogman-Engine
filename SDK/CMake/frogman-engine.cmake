# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
SET(LIST_DIRECTORIES true)
GET_FILENAME_COMPONENT(FROGMAN_ENGINE_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)


INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/city-hash)
FILE(GLOB THIRD_PARTY_CITY_HASH "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/city-hash/*.cc" "${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/city-hash/*.h")


INCLUDE_DIRECTORIES(${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/include)
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




SET(FE_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/engine.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/ECS.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/renderer.hpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/archetype_base.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/component_base.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/system_base.hpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/game_instance_base.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/game_mode_base.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/game_state_base.hpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/world.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/world_context.hpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Include/FE/smart_ptr.hxx
)

SET(FE_SOURCES
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/engine.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/ECS.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/renderer.cpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/archetype_base.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/component_base.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/system_base.cpp

${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/game_instance_base.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/game_mode_base.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/game_state_base.cpp 
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/world.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Source/world_context.cpp
)




SET(FE_FRAMEWORK_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/framework.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/thread_id.hpp
)
SOURCE_GROUP("FE.Framework" FILES ${FE_FRAMEWORK_HEADER})

SET(FE_FRAMEWORK_REFLECTION
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/reflection.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Include/FE/framework/type_info.hpp
)
SOURCE_GROUP("FE.Framework.Reflection" FILES ${FE_FRAMEWORK_REFLECTION})


SET(FE_FRAMEWORK_SOURCES 
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Source/framework.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Source/thread_id.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Source/type_info.cpp
)




SET(FE_ALGORITHM_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/string.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/utility.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/algorithm/math.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/math.cpp
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


SET(FE_CORE_HEADERS
${FE_ALGORITHM_HEADERS}
${FE_LOG_HEADERS}
${FE_POOL_HEADERS}
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/definitions.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/prerequisites.h

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/memory.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/iterator.hxx

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/farray.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/fqueue.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/fstack.hxx

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/function.hxx

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/type_traits.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/types.hxx

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/do_once.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/clock.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/fstream_guard.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/hash.hpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/pair.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/random.hxx

${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/private/memory_traits.hxx
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/private/strlen.hxx
)

SET(FE_CORE_SOURCES
${FE_LOG_SOURCES}
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/memory.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/memory_resource.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/clock.cpp
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Source/hash.cpp
)


SET(FE_MISC_HEADERS
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/private/macro_restrictions.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/define_max_min.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/suppress_warnings.h
${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Include/FE/miscellaneous/undefine_max_min.h
)
SOURCE_GROUP("FE.Misc" FILES ${FE_MISC_HEADERS})




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(BOOST_CHRONO 
    $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/debug/libboost_chrono-vc143-mt-sgd-x64-1_86.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_chrono-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_chrono-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_chrono-vc143-mt-s-x64-1_86.lib>
    )

    SET(BOOST_LOCALE
    $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/debug/libboost_locale-vc143-mt-sgd-x64-1_86.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_locale-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_locale-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_locale-vc143-mt-s-x64-1_86.lib>
    )

    SET(BOOST_STACKTRACE 
    $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/debug/libboost_stacktrace_windbg_cached-vc143-mt-sgd-x64-1_86.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_stacktrace_windbg_cached-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_stacktrace_noop-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_stacktrace_noop-vc143-mt-s-x64-1_86.lib>
    )

    SET(BOOST_THREAD 
    $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/debug/libboost_thread-vc143-mt-sgd-x64-1_86.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_thread-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_thread-vc143-mt-s-x64-1_86.lib>
    $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Third-Party/Libraries/boost-1.86.0/lib/windows/v143/release/libboost_thread-vc143-mt-s-x64-1_86.lib>
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

    SET(FE_CORE
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/Debug/FE.Core.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/RelWithDebInfo/FE.Core.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/Release/FE.Core.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Core/Binaries/X86-64/Windows/MinSizeRel/FE.Core.lib>
    )

    SET(FE_FRAMEWORK
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/Debug/FE.Framework.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/RelWithDebInfo/FE.Framework.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/Release/FE.Framework.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Framework/Binaries/X86-64/Windows/MinSizeRel/FE.Framework.lib>
    )

    SET(FE_ENGINE
        $<$<CONFIG:DEBUG>:			${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/Debug/FE.Engine.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/RelWithDebInfo/FE.Engine.lib>
        $<$<CONFIG:RELEASE>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/Release/FE.Engine.lib>
        $<$<CONFIG:MINSIZEREL>:		${FROGMAN_ENGINE_CMAKE_DIR}/../Engine/Binaries/X86-64/Windows/MinSizeRel/FE.Engine.lib>
    )

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

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

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
            ARGS ${HEADER_TOOL_PROGRAM_OPTIONS} "-path-to-project=${CMAKE_CURRENT_SOURCE_DIR}" "-fno-code-style-guide" "${HEADER_FILES_PATHS}"
            RESULT_VARIABLE RETURN_VALUE_FROM_TOOL
            OUTPUT_VARIABLE TOOL_STDOUT
            ERROR_VARIABLE TOOL_STDERR
        )

    ENDIF()

    MESSAGE("${TOOL_STDOUT}")

    # Print tool output and error for debugging
    MESSAGE("Frogman Engine Header Tool returned the exit code '${RETURN_VALUE_FROM_TOOL}'.")

    IF(NOT RETURN_VALUE_FROM_TOOL EQUAL 0)

        IF(RETURN_VALUE_FROM_TOOL EQUAL -1)
            MESSAGE(STATUS "Frogman Engine Header Tool: The header files must retain a copy of the specified license text.")
        ENDIF()

        MESSAGE(FATAL_ERROR "------ FE HT: compilation failed! Please, check the messages above! ------")
    ENDIF()

    MESSAGE("========== Frogman Engine Header Tool Successfully Processed the Target Header Files ==========")

ENDFUNCTION()


FUNCTION(ADD_FROGMAN_EXECUTABLE TARGET_PROJECT_NAME)
ADD_EXECUTABLE(${TARGET_PROJECT_NAME} ${ARGN} ${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp)
ENDFUNCTION()


SET(FROGMAN_FRAMEWORK_SDK ${BOOST_CHRONO} ${BOOST_LOCALE} ${BOOST_STACKTRACE} ${BOOST_THREAD} ${FE_CORE} ${FE_FRAMEWORK})
SET(FROGMAN_ENGINE_SDK ${BOOST_CHRONO} ${BOOST_LOCALE} ${BOOST_STACKTRACE} ${BOOST_THREAD} ${GLFW} ${IMGUI} ${FE_CORE} ${FE_FRAMEWORK} ${FE_ENGINE})
