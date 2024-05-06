CMAKE_MINIMUM_REQUIRED(VERSION 3.24.0...3.27.7)
PROJECT(FE)
SET(CMAKE_CXX_STANDARD 20)

FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	ADD_COMPILE_DEFINITIONS(_WINDOWS_X86_64_)

	STRING(REPLACE "\\" "\\\\" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR "${OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR}")

#	SET(ENV{VULKAN_SDK} "C:/VulkanSDK/1.3.280.0")
#	SET(GLSL_COMPILER_WINDOWS $ENV{VULKAN_SDK}/Bin/glslc.exe)
#	MESSAGE("\n\n\nDetected GLSL compiler at: " ${GLSL_COMPILER_WINDOWS})
	
#	SET(_SHADER_COMPILER_SCRIPT_
#	${CMAKE_CURRENT_SOURCE_DIR}/../shaders/bin/compile-glsl.bat
#	)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	ADD_COMPILE_DEFINITIONS(_LINUX_X86_64_)
endif()

#FIND_PACKAGE(Vulkan REQUIRED)
#INCLUDE_DIRECTORIES(${Vulkan_INCLUDE_DIRS})
ADD_COMPILE_DEFINITIONS(_CMAKE_PROJECT_DIRECTORY_="${OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR}"
						_HAS_EXCEPTIONS=0 
						#_ENABLE_MEMORY_TRACKER_
						_MEMORY_POOL_FE_STRINGS_
						_MEMORY_POOL_FE_UNIQUE_PTR_PROPERTIES_
						_MEMORY_POOL_FE_EXCLUSIVE_PTR_PROPERTIES_
						)
						# Additonal Options: _ENABLE_MEMORY_TRACKER_ _MEMORY_POOL_FE_STRINGS_ _MEMORY_POOL_FE_UNIQUE_PTR_PROPERTIES_ _MEMORY_POOL_FE_EXCLUSIVE_PTR_PROPERTIES_
						# Expertimental features: _MEMORY_POOL_FE_ARRAYS_



INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Third-Party/City-Hash)


SET(_THIRD_PARTY_CITY_HASH_
	${CMAKE_CURRENT_SOURCE_DIR}/../Third-Party/City-Hash/city.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../Third-Party/City-Hash/city.h
	${CMAKE_CURRENT_SOURCE_DIR}/../Third-Party/City-Hash/citycrc.h
	)


SET(_ALGORITHM_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/algorithm/math.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/algorithm/string.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/algorithm/utility.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/algorithm/heap.hxx 
	)


SET(_ALGORITHM_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/algorithm/private/math.cpp
	)


SET(_LOG_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/log/logger.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/log/format_string.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/log/macros.h
	)


SET(_LOG_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/log/private/logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/log/private/format_string.cpp
	)


SET(_CONTAINERS_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/containers/array.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/containers/queue.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/containers/stack.hxx
	)


SET(_CONTAINERS_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/containers/private/array_impl.hxx
	)


SET(_MACROS_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/macros/attributes.h 	
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/macros/macro_definitions.h
	)


SET(_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/allocator_base.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/allocator_base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/clock.cpp 	
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/hash.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/memory_metrics.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/pool_allocator_base.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/pool_common.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/strlen.h 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/private/thread.cpp
	)


SET(_SMART_POINTERS_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_pointers/exclusive_ptr.hxx 	
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_pointers/ptr.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_pointers/unique_ptr.hxx
	)


SET(_SMART_POINTERS_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_pointers/private/smart_ptr_variants_base.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_pointers/private/smart_ptr_variants_base.cpp	
	)


SET(_SMART_REFERENCES_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_references/object.hxx 	
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_references/ref.hxx
	)


SET(_SMART_REFERENCES_PRIVATE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_references/private/smart_ref_base.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_references/private/smart_ref_base.cpp	
	)


SET(_CORE_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/allocator.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/block_pool.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/block_pool_allocator.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/char_traits.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/clock.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/fstream_guard.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/fstring.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/function.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/hash.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/iterator.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/memory.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/memory_block.hxx 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/memory_metrics.h 
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/pair.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/prerequisites.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/pool.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/pool_allocator.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/random.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/smart_ptrs.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/singleton.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/string.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/string_view.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/thread.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/types.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/core/type_traits.hxx
	)

	
SET(_MISCELLANEOUS_PRIVATE_ ${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/private/macro_restrictions.h)


SET(_MISCELLANEOUS_
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/misc.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/configuration.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/define_max_min.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/google_test_extension.h
	${CMAKE_CURRENT_SOURCE_DIR}/../FE/miscellaneous/undefine_max_min.h
	)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	MESSAGE("Configurating The Build Environment for Linux X86-64 Distributions")

	FIND_PROGRAM(GNU_C NAMES gcc)
	FIND_PROGRAM(CLANG_C NAMES clang)

	if(GNU_C)
		SET(CMAKE_C_COMPILER ${GNU_C})

	elseif(CLANG_C)
		SET(CMAKE_C_COMPILER ${CLANG_C})

	else()
		MESSAGE(FATAL_ERROR "Could Not Find Any of Executable GNU C compilers nor Clang C compilers")
	endif()


	FIND_PROGRAM(GNU_CPP NAMES g++-13 g++-12 g++-11 g++)
	FIND_PROGRAM(CLANG_CPP NAMES clang++-16 clang++-15 clang++-14 clang++-13 clang++-12 clang++)

	if(GNU_CPP)
		SET(CMAKE_CXX_COMPILER ${GNU_CPP})

	elseif(CLANG_CPP)
		SET(CMAKE_CXX_COMPILER ${CLANG_CPP})

	else()
		MESSAGE(FATAL_ERROR "Could Not Find Any of Executable GNU C++ compilers nor Clang C++ compilers")
	endif()

	
	if(GNU_CPP)
		SET(DEBUG_COMPILE_OPTIONS			 -Wall -Wextra -g	-pthread -fno-inline-functions	-fno-unroll-loops	-fno-omit-frame-pointer -fno-exceptions -march=x86-64 -O0)
		SET(RELWITHDEBINFO_COMPILE_OPTIONS	 -Wall -Wextra 		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
		SET(RELEASE_COMPILE_OPTIONS			 -Wall -Wextra		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
		SET(MINSIZEREL_COMPILE_OPTIONS		 -Wall -Wextra		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
		
	elseif(CLANG_CPP)
		SET(DEBUG_COMPILE_OPTIONS			-Wall -Wextra -g	-pthread -fno-inline-functions	-fno-unroll-loops	-fno-omit-frame-pointer -fno-exceptions -march=x86-64 -O0)
		SET(RELWITHDEBINFO_COMPILE_OPTIONS	-Wall -Wextra 		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
		SET(RELEASE_COMPILE_OPTIONS			-Wall -Wextra		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
		SET(MINSIZEREL_COMPILE_OPTIONS		-Wall -Wextra		-pthread -finline-functions		-funroll-loops		-fomit-frame-pointer	-fno-exceptions -march=x86-64 -O3)
	endif()


	SET(BOOST_STACKTRACE_LINUX 
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Linux/gcc-11.4/debug/libboost_stacktrace_basic.a>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Linux/gcc-11.4/release/libboost_stacktrace_basic.a>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Linux/gcc-11.4/release/libboost_stacktrace_basic.a>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Linux/gcc-11.4/release/libboost_stacktrace_basic.a>
	)

	SET(FE_CORE_FRAMEWORK_LINUX 
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Linux/debug/lib/libFE_CORE_FRAMEWORK.a>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Linux/release_with_debug_info/lib/libFE_CORE_FRAMEWORK.a>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Linux/release/lib/libFE_CORE_FRAMEWORK.a>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Linux/minimally_sized_release/lib/libFE_CORE_FRAMEWORK.a>
	)


	ADD_COMPILE_DEFINITIONS(_LINUX_X86_64_)


elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	MESSAGE("Configurating The Build Environment for Windows X86-64")

	SET(DEBUG_COMPILE_OPTIONS			/Zc:__cplusplus /WX /W4 /MP /Od /Ob0 /Oi /Ot /Oy- /GT /MTd /Gy /GF )
	SET(RELWITHDEBINFO_COMPILE_OPTIONS	/Zc:__cplusplus /WX /W4 /MP /Ox /Ob2 /Oi /Ot /Oy- /GT /MT /Gy /GL /GF )
	SET(RELEASE_COMPILE_OPTIONS			/Zc:__cplusplus /WX /W4 /MP /Ox /Ob2 /Oi /Ot /Oy /GT /MT /Gy /GL /GF )
	SET(MINSIZEREL_COMPILE_OPTIONS		/Zc:__cplusplus /WX /W4 /MP /Ox /Ob2 /Oi /Os /Oy /GT /MT /Gy /GL /GF )

	
	SET(BOOST_STACKTRACE_WINDOWS 
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Windows/VS2022-v143/debug/libboost_stacktrace_windbg_cached-vc143-mt-gd-x64-1_85.lib>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Windows/VS2022-v143/release/libboost_stacktrace_basic-vc143-mt-x64-1_85.lib>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Windows/VS2022-v143/release/libboost_stacktrace_noop-vc143-mt-x64-1_85.lib>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/../../Third-Party-Libraries/boost-1.85.0/lib/Windows/VS2022-v143/release/libboost_stacktrace_noop-vc143-mt-x64-1_85.lib>
	)

	SET(FE_CORE_FRAMEWORK_WINDOWS
		$<$<CONFIG:DEBUG>:			${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Windows/debug/lib/FE_CORE_FRAMEWORK.lib>
		$<$<CONFIG:RELWITHDEBINFO>: ${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Windows/release_with_debug_info/lib/FE_CORE_FRAMEWORK.lib>
		$<$<CONFIG:RELEASE>:		${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Windows/release/lib/FE_CORE_FRAMEWORK.lib>
		$<$<CONFIG:MINSIZEREL>:		${CMAKE_CURRENT_SOURCE_DIR}/FE_CORE_FRAMEWORK/Windows/minimally_sized_release/lib/FE_CORE_FRAMEWORK.lib>
	)
	

	ADD_COMPILE_DEFINITIONS(_WINDOWS_X86_64_)

endif()




ADD_SUBDIRECTORY(FE_CORE_AVX)
ADD_SUBDIRECTORY(FE_CORE_FRAMEWORK)

ADD_SUBDIRECTORY(FE_CORE_TEST)
ADD_DEPENDENCIES("FE_CORE_TEST" "FE_CORE_FRAMEWORK")
SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "FE_CORE_TEST")
