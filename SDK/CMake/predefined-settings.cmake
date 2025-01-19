# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release;MinSizeRel")
GET_FILENAME_COMPONENT(FROGMAN_ENGINE_PREDEFINED_SETTINGS_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)

MESSAGE("
Define a cmake macro if you want to make changes on the settings.
Available -D macro options:
-DENABLE_MEMORY_TRACKER=1

Frogman Engine SIMD Extension Requirements:
an x86-64 cpu with AVX and SSE2 (AVX-512F is optional). Please Check if the x86-64 cpu has ymm and xmm vector registers.

This project uses AVX and SSE2 as the default SIMD options on x86-64 CPUs.
The intrinsics option can be added by -D
Available x86-64 -D macro SIMD options:
-DAVX2=1
-DAVX512F=1

")

IF(NOT ((CMAKE_CXX_STANDARD EQUAL 17) OR (CMAKE_CXX_STANDARD EQUAL 20) OR (CMAKE_CXX_STANDARD EQUAL 23)))
    MESSAGE("Frogman Engine supports C++ 17, C++ 20, and C++ 23.")
	MESSAGE(WARNING "No C++ standard version has been specified: this project will use C++17 as the standard.")
	SET(CMAKE_CXX_STANDARD 17)
ENDIF()

IF(NOT ((TARGET_CPU_ARCHITECTURE STREQUAL "x86-64") OR (TARGET_CPU_ARCHITECTURE STREQUAL "arm64")))
	MESSAGE("Frogman Engine supports x86-64.")
	MESSAGE(WARNING "No CPU architecture has been specified: this project will set the target cpu architecture as x86-64.")
	SET(TARGET_CPU_ARCHITECTURE "x86-64")
ENDIF()

FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR)


IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")
	MESSAGE(STATUS "Configurating The Build Environment for Windows X86-64.")
	STRING(REPLACE "\\" "\\\\" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR "${OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR}")
	ADD_COMPILE_OPTIONS(/D_FE_ON_WINDOWS_X86_64_ /D_ALLOWED_DIRECTORY_LENGTH_=256)


	IF(ENABLE_MEMORY_TRACKER STREQUAL "yes")
		MESSAGE(STATUS "Enabled the option: ENABLE_MEMORY_TRACKER")
		ADD_COMPILE_OPTIONS(/D_ENABLE_MEMORY_TRACKER_)
	ENDIF()


	# /Zc:__cplusplus: C++ version macro option
	# /WX: Treat warnings as errors 
	# /W4: Warning level 4
	# /MP: Multi-processor compilation4
	# /GF: Eliminate duplicate strings
	# /GT: Enable fiber-safe Thread Local Storage 
	# /MTd: Multi-threaded Debug Static std C++ library
	# /Gy: Enable function-level linking 
	# /GL: Whole program optimization
	# /Od: Disable optimization 
	# /Ot: Favor fast code 
	# /Os: Favor small code
	# /Ob0: Disable inline function expansion 
	# /Oi: Generate intrinsic functions
	# /Oy-: Disable frame pointer omission  
	# /Gr: Enable calling convention fastcall
	# /GR: Enable RTTI

	# Common Compile Options
	ADD_COMPILE_OPTIONS(/D_HAS_EXCEPTIONS=0 /DNOMINMAX /D_V143_=1 /std:c17 /Zc:__cplusplus /WX /W4 /MP /GF /Gy /Oi /Gr /GR) #  /utf-8

	ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:/D_DEBUG_;/D_ENABLE_ASSERT_;/D_ENABLE_NEGATIVE_ASSERT_;/D_ENABLE_EXIT_;/D_ENABLE_LOG_;/D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:/D_RELWITHDEBINFO_;/D_ENABLE_ASSERT_;/D_ENABLE_NEGATIVE_ASSERT_;/D_ENABLE_EXIT_;/D_ENABLE_LOG_;/D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:/D_RELEASE_;/D_ENABLE_EXIT_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:MINSIZEREL>:/D_MINSIZEREL_;/D_ENABLE_EXIT_>")

	ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:/Od;/Ob0;/Oy-;/MTd>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:/O2;/Ob2;/Ot;/Oy-;/MT>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:/Ox;/Ob2;/Ot;/Oy;/MT>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:MINSIZEREL>:/Os;/Ob2;/Oy;/MT>")


	# Common Linker Options
	ADD_LINK_OPTIONS(/MACHINE:X64 dbghelp.lib)

	ADD_LINK_OPTIONS("$<$<CONFIG:DEBUG>:/NODEFAULTLIB:libcmt.lib>")
	ADD_LINK_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:/NODEFAULTLIB:libcmtd.lib>")
	ADD_LINK_OPTIONS("$<$<CONFIG:RELEASE>:/NODEFAULTLIB:libcmtd.lib>")
	ADD_LINK_OPTIONS("$<$<CONFIG:MINSIZEREL>:/NODEFAULTLIB:libcmtd.lib>")


	IF(DEFINED AVX2)
		ADD_COMPILE_OPTIONS(/arch:AVX2)
		MESSAGE(STATUS "AVX-2 has been selected.")

	ELSEIF(DEFINED AVX512F)
		ADD_COMPILE_OPTIONS(/arch:AVX512)
		MESSAGE(STATUS "AVX-512F has been selected.")

	ELSE()
		ADD_COMPILE_OPTIONS(/arch:AVX)
		MESSAGE(STATUS "AVX has been selected.")
	ENDIF()


	IF(CMAKE_CXX_STANDARD EQUAL 17)
		ADD_COMPILE_OPTIONS(/std:c++17)
		MESSAGE(STATUS "C++17 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 20)
		ADD_COMPILE_OPTIONS(/std:c++20)
		MESSAGE(STATUS "C++20 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 23)
		ADD_COMPILE_OPTIONS(/std:c++23)
		MESSAGE(STATUS "C++23 has been selected.")
	ENDIF()



# Not maintained anymore. The linux development is cancelled.
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND TARGET_CPU_ARCHITECTURE STREQUAL "x86-64")
	MESSAGE(STATUS "Configurating The Build Environment for Linux X86-64 Distributions.")
	ADD_COMPILE_OPTIONS(-D_FE_ON_LINUX_X86_64_ -D_ALLOWED_DIRECTORY_LENGTH_=4096)
	MESSAGE(STATUS "CMake detected a C++ compiler at: ${CMAKE_CXX_COMPILER}.")
	STRING(FIND "${CMAKE_CXX_COMPILER}" "clang" CLANG_COMPILER)

    IF(CLANG_COMPILER GREATER -1)
		MESSAGE(STATUS "The detected C++ compiler is clang++.")
    ELSE()
        MESSAGE(FATAL_ERROR "Could Not Find Any of Executable Clang C++ compilers.")
    ENDIF()


	IF(DEFINED ENABLE_MEMORY_TRACKER)
		MESSAGE(STATUS "Enabled the option: ENABLE_MEMORY_TRACKER")
		ADD_COMPILE_OPTIONS(-D_ENABLE_MEMORY_TRACKER_)
	ENDIF()


	# Common Compile Options.
	ADD_COMPILE_OPTIONS(-D_CLANG_=1 -march=x86-64 -msse2 -mavx -frtti -ffunction-sections -finput-charset=UTF-8 -fexec-charset=UTF-8)

	# Options for compile-time checks
	ADD_COMPILE_OPTIONS(-Werror -Wpedantic -Wall -Wextra -Wdivision-by-zero -Wstack-exhausted -Wbuiltin-memcpy-chk-size -Wfree-nonheap-object -Wnull-dereference -Wnull-pointer-arithmetic)
	ADD_COMPILE_OPTIONS(-Wnull-character -Wbuiltin-macro-redefined -Wswitch -Wimplicit-fallthrough -Wswitch-default -Wdangling -Wthread-safety -Wcoroutine -Wconversion -Wno-sign-conversion )	
	ADD_COMPILE_OPTIONS(-Wbitwise-conditional-parentheses -Wbitwise-op-parentheses -Wbitfield-width -Wbitfield-constant-conversion -Wbitfield-enum-conversion)
	ADD_COMPILE_OPTIONS(-Wno-format-pedantic -Wformat-security -Wno-unknown-pragmas -Wno-extra-semi )
	
	ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:-D_DEBUG_;-D_ENABLE_ASSERT_;-D_ENABLE_NEGATIVE_ASSERT_;-D_ENABLE_LOG_;-D_ENABLE_EXIT_;-D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:-D_RELWITHDEBINFO_;-D_DEBUG_;-D_ENABLE_ASSERT_;-D_ENABLE_NEGATIVE_ASSERT_;-D_ENABLE_LOG_;-D_ENABLE_EXIT_;-D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:-D_RELEASE_;-D_ENABLE_EXIT_>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:MINSIZEREL>:-D_RELEASE_;-D_MINSIZEREL_;-D_ENABLE_EXIT_>")

	ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:-O1;-fno-inline-functions;-fno-unroll-loops;-fno-omit-frame-pointer;-g>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:-O3;-funroll-loops;-g>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:-O3;-funroll-loops;-fomit-frame-pointer>")
	ADD_COMPILE_OPTIONS("$<$<CONFIG:MINSIZEREL>:-Os;-funroll-loops;-fomit-frame-pointer>")
	
    
	ADD_LINK_OPTIONS(-pthread -ldl)
	

	IF(DEFINED AVX2)
		ADD_COMPILE_OPTIONS(-mavx2)
		MESSAGE(STATUS "AVX-2 has been added to the SIMD intrinsic extension list.")

	ELSEIF(DEFINED AVX512F)
		ADD_COMPILE_OPTIONS(-mavx512f)
		MESSAGE(STATUS "AVX-512F has been added to the SIMD intrinsic extension list.")
	ENDIF()


	IF(CMAKE_CXX_STANDARD EQUAL 17)
		ADD_COMPILE_OPTIONS(-std=c++17)
		MESSAGE(STATUS "C++17 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 20)
		ADD_COMPILE_OPTIONS(-std=c++20)
		MESSAGE(STATUS "C++20 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 23)
		ADD_COMPILE_OPTIONS(-std=c++23)
		MESSAGE(STATUS "C++23 has been selected.")
	ENDIF()


ELSE()
	MESSAGE(FATAL_ERROR "System not selected.")
ENDIF()