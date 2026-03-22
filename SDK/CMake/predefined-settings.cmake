# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release;MinSizeRel")
GET_FILENAME_COMPONENT(FROGMAN_ENGINE_PREDEFINED_SETTINGS_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)




MESSAGE("
Define a cmake macro if you want to make changes to the settings.
Available -D macro options:
-DSHIPPING_BUILD=ON/OFF (Default: OFF)

Frogman Engine SIMD Extension Requirements:
an x86-64 cpu with AVX and SSE2 (AVX-512F is optional). Please Check if the x86-64 cpu has ymm and xmm vector registers.

This project uses AVX, AVX2, and SSE2 on x86-64 CPUs by the default.
The intrinsics option can be added by -DSIMD=
The available x86-64 SIMD option for this project:
-DSIMD=AVX512F
")

IF(NOT ((CMAKE_CXX_STANDARD EQUAL 20) OR (CMAKE_CXX_STANDARD EQUAL 23)))
    MESSAGE("Frogman Engine supports C++ 20 and C++ 23.")
	MESSAGE(WARNING "No C++ standard version has been specified: this project will use C++20 as the standard.")
	SET(CMAKE_CXX_STANDARD 20)
ENDIF()

IF(NOT ((CMAKE_SYSTEM_PROCESSOR STREQUAL "x64") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")))
	MESSAGE("Frogman Engine supports x86-64.")
	MESSAGE(WARNING "No CPU architecture has been specified: this project will set the target cpu architecture to x86-64.")
	SET(CMAKE_SYSTEM_PROCESSOR "x64")
ENDIF()

FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR)




IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
	MESSAGE(STATUS "Configurating The Build Environment for Windows X86-64.")
	STRING(REPLACE "\\" "\\\\" OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR "${OS_NATIVE_CMAKE_CURRENT_SOURCE_DIR}")
	ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/D_FE_ON_WINDOWS_X86_64_;/D_ALLOWED_DIRECTORY_LENGTH_=256>")

	ENABLE_LANGUAGE(ASM_MASM)
	
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
	# /Gv: Enable calling convention vectorcall
	# /GR: Enable RTTI
	# /dynamicdeopt: as it reads; dynamic deoptimization for debugging release build binaries.

	# Common Compile Options. The ABSL prevents compile when /Gv is enabled.
	ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/D_HAS_EXCEPTIONS=0;/DNOMINMAX;/D_V143_=1;/std:c17;/Zc:__cplusplus;/WX;/W4;/MP;/GF;/Gy;/Oi;/Gr;/GR;/utf-8>") # /utf-8 /Gv

	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:DEBUG>>:/D_DEBUG_;/D_ENABLE_ASSERT_;/D_ENABLE_NEGATIVE_ASSERT_;/D_ENABLE_EXIT_;/D_ENABLE_LOG_;/D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELWITHDEBINFO>>:/D_RELWITHDEBINFO_;/D_ENABLE_ASSERT_;/D_ENABLE_NEGATIVE_ASSERT_;/D_ENABLE_EXIT_;/D_ENABLE_LOG_;/D_ENABLE_LOG_IF_>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELEASE>>:/D_RELEASE_;/D_ENABLE_EXIT_>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:MINSIZEREL>>:/D_MINSIZEREL_;/D_ENABLE_EXIT_>")

	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:DEBUG>>:/Od;/Oy-;/MTd>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELWITHDEBINFO>>:/O2;/Oy-;/MT;/dynamicdeopt>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELEASE>>:/Ox;/MT>")
	ADD_COMPILE_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:MINSIZEREL>>:/O1;/MT>")


	# Common Linker Options
	ADD_LINK_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/MACHINE:X64;dbghelp.lib>")

	ADD_LINK_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:DEBUG>>:/NODEFAULTLIB:libcmt.lib>")
	ADD_LINK_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELWITHDEBINFO>>:/NODEFAULTLIB:libcmtd.lib>")
	ADD_LINK_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RELEASE>>:/NODEFAULTLIB:libcmtd.lib>")
	ADD_LINK_OPTIONS("$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:MINSIZEREL>>:/NODEFAULTLIB:libcmtd.lib>")


	IF(SIMD STREQUAL AVX512F)
		ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/arch:AVX512>")
		MESSAGE(STATUS "AVX-512F has been selected.")

	ELSE()
		ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/arch:AVX2>")
		MESSAGE(STATUS "AVX2 has been selected.")
	ENDIF()


	IF(CMAKE_CXX_STANDARD EQUAL 20)
		ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/std:c++20>")
		MESSAGE(STATUS "C++20 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 23)
		ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C,CXX>:/std:c++23>")
		MESSAGE(STATUS "C++23 has been selected.")
	ENDIF()




# Not maintained anymore. The linux development is canceled.
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
	MESSAGE(STATUS "Configurating The Build Environment for Linux X86-64 Distributions.")
	ADD_COMPILE_OPTIONS(-D_FE_ON_LINUX_X86_64_ -D_ALLOWED_DIRECTORY_LENGTH_=4096)
	MESSAGE(STATUS "CMake detected a C++ compiler at: ${CMAKE_CXX_COMPILER}.")
	STRING(FIND "${CMAKE_CXX_COMPILER}" "clang" CLANG_COMPILER)

    IF(CLANG_COMPILER GREATER -1)
		MESSAGE(STATUS "The detected C++ compiler is clang++.")
    ELSE()
        MESSAGE(FATAL_ERROR "Could Not Find Any of Executable Clang C++ compilers.")
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
	

	IF(SIMD STREQUAL AVX512F)
		ADD_COMPILE_OPTIONS(-mavx512f )
		MESSAGE(STATUS "AVX-512F has been added to the SIMD intrinsic extension list.")

	ELSE()
		ADD_COMPILE_OPTIONS(-mavx -mavx2)
		MESSAGE(STATUS "AVX and AVX2 have been added to the SIMD intrinsic extension list.")
	ENDIF()


	IF(CMAKE_CXX_STANDARD EQUAL 20)
		ADD_COMPILE_OPTIONS(-std=c++20)
		MESSAGE(STATUS "C++20 has been selected.")

	ELSEIF(CMAKE_CXX_STANDARD EQUAL 23)
		ADD_COMPILE_OPTIONS(-std=c++23)
		MESSAGE(STATUS "C++23 has been selected.")
	ENDIF()




ELSE()
	MESSAGE(FATAL_ERROR "System not selected or incompatible.")
ENDIF()