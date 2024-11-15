# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
SET(LIST_DIRECTORIES true)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/City-Hash)
FILE(GLOB THIRD_PARTY_CITY_HASH "${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/City-Hash/*.cc" "${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/City-Hash/*.h")


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/Robin-Hood-Hash)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Core/Include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Framework/Include)




SET(FE_FRAMEWORK
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/framework.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/game_thread.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/vulkan_renderer.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/input_manager.hpp
)
SOURCE_GROUP("Framework" FILES ${FE_FRAMEWORK})

SET(FE_FRAMEWORK_REFLECTION
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/reflection.h
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/method.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/property.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/type_info.hpp
)
SOURCE_GROUP("Reflection" FILES ${FE_FRAMEWORK_REFLECTION})

SET(FE_FRAMEWORK_OBJECT_HIERARCHY 
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/object_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/pawn_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/character_base.hpp
)
SOURCE_GROUP("Object" FILES ${FE_FRAMEWORK_OBJECT_HIERARCHY})

SET(FE_FRAMEWORK_CONTROLLER_HIERARCHY 
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/controller_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/player_controller_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/AI_controller_base.hpp
)
SOURCE_GROUP("Controller" FILES ${FE_FRAMEWORK_CONTROLLER_HIERARCHY})




SET(FE_FRAMEWORK_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/framework.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/game_thread.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/vulkan_renderer.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/input_manager.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/platform_information.h

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/reflection.h
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/method.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/property.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/reflection/type_info.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/atom_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/object_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/pawn_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/character_base.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/firearm_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/projectile_base.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/game_instance.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/game_mode_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/game_state_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/world.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/controller_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/player_controller_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/AI_controller_base.hpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/object_component_base.hpp
)




SET(FE_FRAMEWORK_SOURCES 
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/framework.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/game_thread.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/vulkan_renderer.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/input_manager.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/platform_information.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/property.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/type_info.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/atom_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/object_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/pawn_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/character_base.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/firearm_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/projectile_base.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/game_instance.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/game_mode_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/game_state_base.cpp 
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/world.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/controller_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/player_controller_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/AI_controller_base.cpp

${CMAKE_CURRENT_LIST_DIR}/../Framework/Source/object_component_base.cpp
)




SET(FE_ALGORITHM_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/algorithm/string.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/algorithm/utility.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/algorithm/math.hpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/math.cpp
)




SET(FE_LOG_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/log/format_string.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/log/log.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/log/logger.hpp
)

SET(FE_LOG_SOURCES
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/format_string.cpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/logger.cpp
)




SET(FE_FRAMEWORK_MANAGED_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/managed/ptr.hxx
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/managed/trackable.hxx
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/managed/unique_ptr.hxx
${CMAKE_CURRENT_LIST_DIR}/../Framework/Include/FE/framework/managed/private/ref_block.hxx
)




SET(FE_POOL_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pool/block_pool.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pool/block_pool_allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pool/scalable_pool.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pool/scalable_pool_allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pool/private/pool_common.hxx
)




SET(FE_CORE_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/definitions.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/prerequisites.h

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/memory.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/iterator.hxx

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/farray.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/fqueue.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/fstack.hxx

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/function.hxx

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/type_traits.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/types.hxx

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/do_once.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/clock.hpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/fstream_guard.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/hash.hpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/pair.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/random.hxx

${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/private/allocator_base.hpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/private/memory_traits.hxx
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/private/strlen.hxx
)

SET(FE_CORE_SOURCES
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/allocator_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/clock.cpp
${CMAKE_CURRENT_LIST_DIR}/../Core/Source/hash.cpp
)




SET(FE_MISC_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/miscellaneous/private/macro_restrictions.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/miscellaneous/define_max_min.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/miscellaneous/suppress_warnings.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/miscellaneous/forward_declare_property.h
${CMAKE_CURRENT_LIST_DIR}/../Core/Include/FE/miscellaneous/undefine_max_min.h
)




IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(BOOST_STACKTRACE 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_stacktrace_addr2line.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_addr2line.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_noop.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_noop.a>
    )

    SET(BOOST_CHRONO 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_chrono.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    )

    SET(BOOST_THREAD 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_thread.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_thread.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_thread.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_thread.a>
    )

    SET(GLM
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/Debug/libglm.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/Release/libglm.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/Release/libglm.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/RelWithDebInfo/libglm.a>
    )

    SET(GLFW
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/linux/clang++-12/Debug/libglfw3.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/linux/clang++-12/Release/libglfw3.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/linux/clang++-12/Release/libglfw3.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/linux/clang++-12/RelWithDebInfo/libglfw3.a>
    )

    SET(FE_CORE
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Linux/Debug/libFE.Core.a>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Linux/RelWithDebInfo/libFE.Core.a>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Linux/Release/libFE.Core.a>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Linux/MinSizeRel/libFE.Core.a>
    )

    SET(FE_FRAMEWORK
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Linux/Debug/libFE.Framework.a>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Linux/RelWithDebInfo/libFE.Framework.a>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Linux/Release/libFE.Framework.a>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Linux/MinSizeRel/libFE.Framework.a>
    )

    
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(BOOST_STACKTRACE 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_stacktrace_windbg_cached-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_windbg_cached-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_noop-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_noop-clangw17-mt-s-x64-1_85.lib>
    )

    SET(BOOST_CHRONO 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_chrono-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    )

    SET(BOOST_THREAD 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_thread-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_thread-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_thread-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_thread-clangw17-mt-s-x64-1_85.lib>
    )

    SET(GLM
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/windows/clangw17-mt-x64/Debug/glm.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/windows/clangw17-mt-x64/MinSizeRel/glm.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/windows/clangw17-mt-x64/Release/glm.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glm-1.0.1/lib/windows/clangw17-mt-x64/RelWithDebInfo/glm.lib>
    )

    SET(GLFW
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/windows/clangw17-mt-x64/Debug/glfw3.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/windows/clangw17-mt-x64/MinSizeRel/glfw3.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/windows/clangw17-mt-x64/Release/glfw3.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Third-Party/Libraries/glfw-3.4/lib/windows/clangw17-mt-x64/RelWithDebInfo/glfw3.lib>
    )

    SET(FE_CORE
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Windows/Debug/FE.Core.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Windows/RelWithDebInfo/FE.Core.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Windows/Release/FE.Core.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Core/Binaries/X86-64/Windows/MinSizeRel/FE.Core.lib>
    )

    SET(FE_FRAMEWORK
        $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Windows/Debug/FE.Framework.lib>
        $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Windows/RelWithDebInfo/FE.Framework.lib>
        $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Windows/Release/FE.Framework.lib>
        $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../Framework/Binaries/X86-64/Windows/MinSizeRel/FE.Framework.lib>
    )

ENDIF()