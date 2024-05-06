CMAKE_MINIMUM_REQUIRED(VERSION 3.24.0...3.27.7)
SET(FE_CORE_TEST "FE_CORE_TEST")




INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/include)


SOURCE_GROUP("Third-Party\\City-Hash" FILES ${_THIRD_PARTY_CITY_HASH_})
SOURCE_GROUP("FE\\core\\algorithm" FILES ${_ALGORITHM_})
SOURCE_GROUP("FE\\core\\algorithm\\private" FILES ${_ALGORITHM_PRIVATE_})
SOURCE_GROUP("FE\\core\\log" FILES ${_LOG_})
SOURCE_GROUP("FE\\core\\log\\private" FILES ${_LOG_PRIVATE_})
SOURCE_GROUP("FE\\core\\containers" FILES ${_CONTAINERS_})
SOURCE_GROUP("FE\\core\\containers\\private" FILES ${_CONTAINERS_PRIVATE_})
SOURCE_GROUP("FE\\core\\macros" FILES ${_MACROS_})
SOURCE_GROUP("FE\\core\\private" FILES ${_PRIVATE_})
SOURCE_GROUP("FE\\core\\smart_pointers" FILES ${_SMART_POINTERS_})
SOURCE_GROUP("FE\\core\\smart_pointers\\private" FILES ${_SMART_POINTERS_PRIVATE_})
SOURCE_GROUP("FE\\core\\smart_references" FILES ${_SMART_REFERENCES_})
SOURCE_GROUP("FE\\core\\smart_references\\private" FILES ${_SMART_REFERENCES_PRIVATE_})
SOURCE_GROUP("FE\\core" FILES ${_CORE_})
SOURCE_GROUP("FE\\miscellaneous\\private" FILES ${_MISCELLANEOUS_PRIVATE_})
SOURCE_GROUP("FE\\miscellaneous" FILES ${_MISCELLANEOUS_})


SET(_TEST_CODE_
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/allocator-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/array-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/fstring-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/function_table-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/function-thread-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/hash-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/heap-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/iterator-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/math_algorithm-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/never-dangling_test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/pool-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/private-memory-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/queue-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/singleton_templates-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/smart_ptr_variants-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/stack-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/string_algorithm-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/string_view-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/string-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/unique_ptr-test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/FE-Core-Test-Cases/utility_algorithm-test.cpp
	)
SOURCE_GROUP("Tests" FILES ${_TEST_CODE_})


ADD_EXECUTABLE(	${FE_CORE_TEST} 
				${_THIRD_PARTY_CITY_HASH_}
				main.cpp
				${_ALGORITHM_}
				${_ALGORITHM_PRIVATE_}
				${_LOG_}
				${_LOG_PRIVATE_}
				${_CONCURRENT_CONTAINERS_}
				${_CONTAINERS_}
				${_CONTAINERS_PRIVATE_}
				${_MACROS_}
				${_MEMORY_POOL_} 
				${_PRIVATE_}
				${_SMART_POINTERS_}
				${_SMART_POINTERS_PRIVATE_}
				${_SMART_REFERENCES_}
				${_SMART_REFERENCES_PRIVATE_}
				${_CORE_}
				${_MISCELLANEOUS_PRIVATE_}
				${_MISCELLANEOUS_}
				${_TEST_CODE_}
				)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	MESSAGE("Configurating The Build Environment for Linux X86-64 Distributions")


	if(GNU_CPP)
		TARGET_COMPILE_OPTIONS(${FE_CORE_TEST} PRIVATE
			$<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:RELWITHDEBINFO>: ${RELWITHDEBINFO_COMPILE_OPTIONS}	-mavx>
			$<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		-mavx>
		)
	elseif(CLANG_CPP)
		TARGET_COMPILE_OPTIONS(${FE_CORE_TEST} PRIVATE
			$<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:RELWITHDEBINFO>: ${RELWITHDEBINFO_COMPILE_OPTIONS} 	-mavx>
			$<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		-mavx>
		)
	endif()


	SET_TARGET_PROPERTIES(${FE_CORE_TEST} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG 			${CMAKE_CURRENT_SOURCE_DIR}/Linux/debug/a
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Linux/release_with_debug_info/a
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		${CMAKE_CURRENT_SOURCE_DIR}/Linux/release/a
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL 	${CMAKE_CURRENT_SOURCE_DIR}/Linux/minimally_sized_release/a
    )

	SET(UNIT_TEST_LINUX			
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/linux/libgtest.a>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/linux/libgtest.a>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/linux/libgtest.a>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/linux/libgtest.a>
	)

#	SET(BENCHMARK_TEST_LINUX			
#		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/linux/libbenchmark.a>
#		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/linux/libbenchmark.a>
#		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/linux/libbenchmark.a>
#		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/linux/libbenchmark.a>
#	)

	TARGET_LINK_LIBRARIES(${FE_CORE_TEST} 
		stdc++fs 
		${BOOST_STACKTRACE_LINUX}
		${UNIT_TEST_LINUX}
#		${BENCHMARK_TEST_LINUX}
		${FE_CORE_FRAMEWORK_LINUX}
	)




elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	MESSAGE("Configurating The Build Environment for Windows X86-64 Distributions")

	TARGET_COMPILE_OPTIONS(${FE_CORE_TEST} PRIVATE
        $<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			/arch:AVX>
        $<$<CONFIG:RELWITHDEBINFO>:	${RELWITHDEBINFO_COMPILE_OPTIONS} 	/arch:AVX>
        $<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			/arch:AVX>
        $<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		/arch:AVX>
    )


	SET_TARGET_PROPERTIES(${FE_CORE_TEST} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG 			${CMAKE_CURRENT_SOURCE_DIR}/Windows/debug
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Windows/release_with_debug_info
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		${CMAKE_CURRENT_SOURCE_DIR}/Windows/release
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL 	${CMAKE_CURRENT_SOURCE_DIR}/Windows/minimally_sized_release
    )


	SET(UNIT_TEST_WINDOWS			
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/windows/Debug/gtest.lib>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/windows/RelWithDebInfo/gtest.lib>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/windows/Release/gtest.lib>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Google-Test/windows/MinSizeRel/gtest.lib>
	)

	SET(BENCHMARK_TEST_WINDOWS			
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/windows/Debug/benchmark.lib>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/windows/RelWithDebInfo/benchmark.lib>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/windows/Release/benchmark.lib>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../../Frogman-Engine-Tests/Benchmark/windows/MinSizeRel/benchmark.lib>
	)


	TARGET_LINK_LIBRARIES(${FE_CORE_TEST} 
		${BOOST_STACKTRACE_WINDOWS}
		${FE_CORE_FRAMEWORK_WINDOWS}
		${UNIT_TEST_WINDOWS}
		${BENCHMARK_TEST_WINDOWS}
		shlwapi.lib
	)


endif()




#// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#// To enable or disable a certain feature, remove the macro.
#// 
#// Modifying the macro will not immediately affect your project.
#// The binary files e.g. (.lib, .a, .dll, or .so) need to be rebuilt to apply changes that have been made.

#// Frogman Engine Memory Tracker generates a memory utilization report over frames at the end of an app process.(Planned)
#// The output file can be visualized as a graph image via Frogman Engine Memory Utilization Analyzer.
#// Memory tracking routines involve atomic integral sum and add operations, and its tracking frequency can be dynamically adjusted without recompiling the Engine binary.
#// Excluding this feature can improve an app performance.

#// FE_ASSERT can be disabled by removing the corresponding macro to eliminate error-checking overhead.
#// Please NOTE that FE_ASSERT aborts when circumstances are unrecoverable and critical.

ADD_COMPILE_DEFINITIONS(BENCHMARK_STATIC_DEFINE)


TARGET_COMPILE_DEFINITIONS(${FE_CORE_TEST} PRIVATE
	$<$<CONFIG:DEBUG>:			_DEBUG_					_ENABLE_ASSERT_ _ENABLE_ABORT_IF_ _ENABLE_LOG_ _ENABLE_EXIT_ > 
	$<$<CONFIG:RELWITHDEBINFO>:	_RELWITHDEBINFO_		_ENABLE_ASSERT_ _ENABLE_ABORT_IF_ _ENABLE_LOG_ _ENABLE_EXIT_ >
	$<$<CONFIG:RELEASE>:		_RELEASE_				_ENABLE_EXIT_ >
	$<$<CONFIG:MINSIZEREL>:		_RELEASE_ _MINSIZEREL_	_ENABLE_EXIT_ >
)