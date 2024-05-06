CMAKE_MINIMUM_REQUIRED(VERSION 3.24.0...3.27.7)
SET(FE_CORE_AVX "FE_CORE_AVX")




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


ADD_LIBRARY(${FE_CORE_AVX} STATIC
	${_THIRD_PARTY_CITY_HASH_}
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
)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	MESSAGE("Configurating The Build Environment for Linux X86-64 Distributions")


	if(GNU_CPP)
		TARGET_COMPILE_OPTIONS(${FE_CORE_AVX} PRIVATE
			$<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:RELWITHDEBINFO>: ${RELWITHDEBINFO_COMPILE_OPTIONS} 	-mavx>
			$<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		-mavx>
		)
	elseif(CLANG_CPP)
		TARGET_COMPILE_OPTIONS(${FE_CORE_AVX} PRIVATE
			$<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:RELWITHDEBINFO>: ${RELWITHDEBINFO_COMPILE_OPTIONS} 	-mavx>
			$<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			-mavx>
			$<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		-mavx>
		)
	endif()


	SET_TARGET_PROPERTIES(${FE_CORE_AVX} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG 			${CMAKE_CURRENT_SOURCE_DIR}/Linux/debug/a
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Linux/release_with_debug_info/a
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		${CMAKE_CURRENT_SOURCE_DIR}/Linux/release/a
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL 	${CMAKE_CURRENT_SOURCE_DIR}/Linux/minimally_sized_release/a
    )


	TARGET_LINK_LIBRARIES(${FE_CORE_AVX}
		stdc++fs
		${BOOST_STACKTRACE_LINUX}
	)




elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	MESSAGE("Configurating The Build Environment for Windows X86-64 Distributions")

	TARGET_COMPILE_OPTIONS(${FE_CORE_AVX} PRIVATE
        $<$<CONFIG:DEBUG>:			${DEBUG_COMPILE_OPTIONS} 			/arch:AVX>
        $<$<CONFIG:RELWITHDEBINFO>:	${RELWITHDEBINFO_COMPILE_OPTIONS} 	/arch:AVX>
        $<$<CONFIG:RELEASE>:		${RELEASE_COMPILE_OPTIONS} 			/arch:AVX>
        $<$<CONFIG:MINSIZEREL>:		${MINSIZEREL_COMPILE_OPTIONS} 		/arch:AVX>
    )


	SET_TARGET_PROPERTIES(${FE_CORE_AVX} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG 			${CMAKE_CURRENT_SOURCE_DIR}/Windows/debug/lib
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Windows/release_with_debug_info/lib
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		${CMAKE_CURRENT_SOURCE_DIR}/Windows/release/lib
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL 	${CMAKE_CURRENT_SOURCE_DIR}/Windows/minimally_sized_release/lib
    )


	TARGET_LINK_LIBRARIES(${FE_CORE_AVX} 
		${BOOST_STACKTRACE_WINDOWS}
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


TARGET_COMPILE_DEFINITIONS(${FE_CORE_AVX} PRIVATE
	$<$<CONFIG:DEBUG>:			_DEBUG_					_ENABLE_ASSERT_ _ENABLE_ABORT_IF_ _ENABLE_LOG_ _ENABLE_EXIT_ >
	$<$<CONFIG:RELWITHDEBINFO>:	_RELWITHDEBINFO_		_ENABLE_ASSERT_ _ENABLE_ABORT_IF_ _ENABLE_LOG_ _ENABLE_EXIT_ >
	$<$<CONFIG:RELEASE>:		_RELEASE_				_ENABLE_EXIT_ >
	$<$<CONFIG:MINSIZEREL>:		_RELEASE_ _MINSIZEREL_	_ENABLE_EXIT_ >
)