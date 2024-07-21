#ifndef _FE_CORE_MACRO_DEFINITIONS_H_
#define _FE_CORE_MACRO_DEFINITIONS_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.


#ifdef _MSC_VER
    #define _MSVC_
    #include <vcruntime.h>
#elif defined( __GLIBCXX__ ) || defined( __GLIBCPP__ )
    #include <cxxabi.h>
    #define _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_

    #ifdef __GABIXX_CXXABI_H__ // This code has referenced Boost.demangle.hpp
    #undef _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_
    #endif
#endif


#ifdef _MSVC_
    #define _FORCE_INLINE_ __forceinline
<<<<<<< HEAD
#else
    #define _FORCE_INLINE_ inline __attribute__((always_inline))
#endif

#define _VECTOR_CALL_ __vectorcall
#define _REGISTER_CALL_ __regcall
=======
    #define _STDCALL_ __stdcall
    #define _FASTCALL_ __fastcall
#else
    #define _FORCE_INLINE_ inline __attribute__((always_inline))
    #define _STDCALL_ 
    #define _FASTCALL_
#endif


>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
#define _CDECL_ __cdecl


#if __cplusplus >= 202004L
    #define _HAS_CXX23_
    #define _HAS_CXX20_
    #define _HAS_CXX17_
#elif __cplusplus >= 202002L
    #define _HAS_CXX20_
    #define _HAS_CXX17_
#elif __cplusplus >= 201703L
    #define _HAS_CXX17_
#else
// please add /Zc:__cplusplus to the aditional options. [ C/C++ -> Command Line -> Aditional Options ]
    #error Archaic C++ version detected. Use C++ 17 or later versions. Please add /Zc:__cplusplus to the aditional options. [ C/C++ -> Command Line -> Aditional Options ]
#endif


#ifdef _HAS_CXX17_
    #define _CONSTEXPR17_ constexpr
#else
    #define _CONSTEXPR17_
#endif

#ifdef _HAS_CXX20_
    #define _CONSTEXPR20_ constexpr
#else
    #define _CONSTEXPR20_
#endif

#ifdef _HAS_CXX23_
    #define _CONSTEXPR23_ constexpr
#else
    #define _CONSTEXPR23_
#endif


#ifdef _WINDOWS_X86_64_
    #define STRING(s) L#s
#elif defined(_LINUX_X86_64_)
    #define STRING(s) #s
#endif


#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
    #define _X86_64_
#elif defined(__aarch64__)
    #define _ARM64_
#else
#error platform macro not defined.
#endif


#define BEGIN_NAMESPACE(NAME_P) namespace NAME_P {
#define END_NAMESPACE }
#define CLASS_FORWARD_DECLARATION(namespace_p, class_p) namespace namespace_p { class class_p; }
#define STRUCT_FORWARD_DECLARATION(namespace_p, struct_p) namespace namespace_p { struct struct_p; }
#define ENUM_STRUCT_FORWARD_DECLARATION(namespace_p, enum_struct_p) namespace namespace_p { enum struct enum_struct_p; }
#define _SOURCE_CODE_LOCATION_ __FILE__, __func__, __LINE__


#endif