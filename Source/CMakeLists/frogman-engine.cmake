# Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
SET(LIST_DIRECTORIES true)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../Third-Party/City-Hash)
FILE(GLOB THIRD_PARTY_CITY_HASH "${CMAKE_CURRENT_LIST_DIR}/../Third-Party/City-Hash/*.cc" "${CMAKE_CURRENT_LIST_DIR}/../Third-Party/City-Hash/*.h")


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/glm-1.0.1)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/Robin-Hood-Hash)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../)


SET(FE_CORE_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../FE/core/algorithm/heap.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/algorithm/math.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/algorithm/string.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/algorithm/utility.hxx

${CMAKE_CURRENT_LIST_DIR}/../FE/core/log/format_string.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/log/logger.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/log/log.h

${CMAKE_CURRENT_LIST_DIR}/../FE/core/macros/attributes.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/macros/definitions.h

${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed/exclusive_ptr.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed/ptr.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed/trackable.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed/unique_ptr.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed/private/ref_block.hxx

${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/allocator_base.hpp

${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/debug.h

${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/memory_traits.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/pool_common.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/strlen.hxx

${CMAKE_CURRENT_LIST_DIR}/../FE/core/adjacency_graph.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/array.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/block_pool_allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/block_pool.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/char_traits.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/clock.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/do_once.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/fstream_guard.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/fstring.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/function.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/hash.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/iterator.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/managed.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/memory.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/pair.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/pool_allocator.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/pool.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/prerequisites.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/queue.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/random.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/stack.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/string_view.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/string.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/thread.h
${CMAKE_CURRENT_LIST_DIR}/../FE/core/type_traits.hxx
${CMAKE_CURRENT_LIST_DIR}/../FE/core/types.hxx
)

SET(FE_CORE_SOURCES
${CMAKE_CURRENT_LIST_DIR}/../FE/core/algorithm/private/math.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/vector.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/log/private/format_string.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/log/private/logger.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/allocator_base.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/clock.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/core/private/hash.cpp
)


SET(FE_FRAMEWORK_HEADERS
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/game/object.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/function.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/reflection.h
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/property.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/type_info.hpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/framework.hpp
)

SET(FE_FRAMEWORK_SOURCES
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/game/private/object.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/private/function.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/private/property.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/reflection/private/type_info.cpp
${CMAKE_CURRENT_LIST_DIR}/../FE/framework/private/framework.cpp
)




IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(BOOST_STACKTRACE 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_stacktrace_addr2line.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_addr2line.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_noop.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_stacktrace_noop.a>
    )

    SET(BOOST_CHRONO 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_chrono.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_chrono.a>
    )

    SET(BOOST_FILESYSTEM 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_filesystem.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_filesystem.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_filesystem.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_filesystem.a>
    )

    SET(BOOST_SERIALIZATION
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/debug/libboost_serialization.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_serialization.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_serialization.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/linux/clang-10/release/libboost_serialization.a>
    )

    SET(GLM
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/libglm.a>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/libglm.a>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/libglm.a>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/glm-1.0.1/lib/linux/clang++-12/libglm.a>
    )
    
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")

    SET(BOOST_STACKTRACE 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_stacktrace_windbg_cached-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_windbg_cached-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_noop-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_stacktrace_noop-clangw17-mt-s-x64-1_85.lib>
    )

    SET(BOOST_CHRONO 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_chrono-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_chrono-clangw17-mt-s-x64-1_85.lib>
    )

    SET(BOOST_FILESYSTEM 
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_filesystem-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_filesystem-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_filesystem-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_filesystem-clangw17-mt-s-x64-1_85.lib>
    )

    SET(BOOST_SERIALIZATION
    $<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/debug/libboost_serialization-clangw17-mt-sgd-x64-1_85.lib>
    $<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_serialization-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_serialization-clangw17-mt-s-x64-1_85.lib>
    $<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_LIST_DIR}/../../Third-Party/Libraries/boost-1.85.0/lib/windows/clangw17-mt-x64/release/libboost_serialization-clangw17-mt-s-x64-1_85.lib>
    )

ENDIF()