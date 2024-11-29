#ifndef _FE_CORE_MACRO_DEFINITIONS_H_
#define _FE_CORE_MACRO_DEFINITIONS_H_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#ifdef _MSC_VER
    #define _FE_WITH_MSVC_
    #include <vcruntime.h>
#elif defined( __GLIBCXX__ ) || defined( __GLIBCPP__ )
    #include <cxxabi.h>
    #define _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_

    #ifdef __GABIXX_CXXABI_H__ // This code has referenced Boost.demangle.hpp
    #undef _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_
    #endif
#endif


#if defined(_FE_ON_WINDOWS_X86_64_) || defined(_FE_ON_LINUX_X86_64_)
    #define _FE_ON_X86_64_
#elif defined(__aarch64__)
    #define _FE_ON_ARM64_
#else
#error platform macro not defined.
#endif


#if __cplusplus >= 202004L
    #define _FE_HAS_CXX23_
    #define _FE_HAS_CXX20_
    #define _FE_HAS_CXX17_
#elif __cplusplus >= 202002L
    #define _FE_HAS_CXX20_
    #define _FE_HAS_CXX17_
#elif __cplusplus >= 201703L
    #define _FE_HAS_CXX17_
#else
// please add /Zc:__cplusplus to the aditional options. [ C/C++ -> Command Line -> Aditional Options ]
    #error Archaic C++ version detected. Use C++ 17 or later versions. Please add /Zc:__cplusplus to the aditional options. [ C/C++ -> Command Line -> Aditional Options ]
#endif


#ifdef _FE_WITH_MSVC_
    #define _FE_FORCE_INLINE_ __forceinline
#else
    #define _FE_FORCE_INLINE_ inline __attribute__((always_inline))
#endif

#define _FE_VECTOR_CALL_ __vectorcall
#define _FE_CDECL_ __cdecl


#ifdef _FE_HAS_CXX17_
    #define _FE_CONSTEXPR17_ constexpr
#else
    #define _FE_CONSTEXPR17_
#endif

#ifdef _FE_HAS_CXX20_
    #define _FE_CONSTEXPR20_ constexpr
    #define _FE_CONSTEVAL20_ consteval
#else
    #define _FE_CONSTEXPR20_
    #define _FE_CONSTEVAL20_
#endif

#ifdef _FE_HAS_CXX23_
    #define _FE_CONSTEXPR23_ constexpr
    #define _FE_CONSTEVAL23_ consteval
#else
    #define _FE_CONSTEXPR23_
    #define _FE_CONSTEVAL23_
#endif


#ifdef _FE_ON_WINDOWS_X86_64_
    #define FE_TEXT(s) L#s
#elif defined(_FE_ON_LINUX_X86_64_)
    #define FE_TEXT(s) #s
#endif


#define _FE_NODISCARD_ [[nodiscard]]
#define _FE_FALLTHROUGH_ [[fallthrough]]
#define _FE_MAYBE_UNUSED_ [[maybe_unused]]
#define _FE_DISCARD_ _FE_MAYBE_UNUSED_
#define _FE_NORETURN_ [[noreturn]]
#define _FE_DEPRECATED_ [[deprecated]]
#define _FE_IN_DEVELOPMENT_ _FE_DEPRECATED_


#ifdef _FE_HAS_CXX20_
	#define _FE_LIKELY_ [[likely]]
	#define _FE_UNLIKELY_ [[unlikely]]
	#define FE_LIKELY(c) (c)
	#define FE_UNLIKELY(c) (c)
	#define _FE_NO_UNIQUE_ADDRESS_ //[[no_unique_address]]

#else
	#ifdef _FE_ON_LINUX_X86_64_
		#define FE_LIKELY(c) __builtin_expect((c), 1)
		#define FE_UNLIKELY(c) __builtin_expect((c), 0)
	#else
		#define FE_LIKELY(c) (c)
		#define FE_UNLIKELY(c) (c)
	#endif

	#define _FE_LIKELY_ 
	#define _FE_UNLIKELY_
	#define _FE_NO_UNIQUE_ADDRESS_
#endif




#define BEGIN_NAMESPACE(NAME_P) namespace NAME_P {
#define END_NAMESPACE }
#define CLASS_FORWARD_DECLARATION(namespace_p, class_p) namespace namespace_p { class class_p; }
#define STRUCT_FORWARD_DECLARATION(namespace_p, struct_p) namespace namespace_p { struct struct_p; }
#define ENUM_STRUCT_FORWARD_DECLARATION(namespace_p, enum_struct_p) namespace namespace_p { enum struct enum_struct_p; }
#define _SOURCE_CODE_LOCATION_ __FILE__, __func__, __LINE__


#endif