#ifndef _FE_CORE_MACROS_MACRO_DEFINITIONS_H_
#define _FE_CORE_MACROS_MACRO_DEFINITIONS_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.




#ifdef _MSC_VER
#define _MSVC_
#include <vcruntime.h>
#endif

#ifdef __clang__
#define _CLANG_ 
#endif

#ifdef __GNUC__
#define _GNUC_ 
#endif




#ifdef __x86_64
#define _X64_ 
#elif defined(_M_X64)
#define _X64_ 
#elif defined(_M_AMD64)
#define _X64_ 
#endif




#if defined(_CLANG_) && defined(_LINUX_X86_64_) || defined(_GNUC_) && defined(_LINUX_X86_64_)
#define _FORCE_INLINE_ inline
#define _STDCALL_ 
#define _FASTCALL_

#elif defined(_MSVC_)
#define _FORCE_INLINE_ __forceinline
#define _STDCALL_ __stdcall
#define _FASTCALL_ __fastcall
#endif

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




#define BEGIN_NAMESPACE(NAME_P) namespace NAME_P {
#define END_NAMESPACE }
#define CLASS_FORWARD_DECLARATION(namespace_p, class_p) namespace namespace_p { class class_p; }
#define STRUCT_FORWARD_DECLARATION(namespace_p, struct_p) namespace namespace_p { struct struct_p; }
#define ENUM_STRUCT_FORWARD_DECLARATION(namespace_p, enum_struct_p) namespace namespace_p { enum struct enum_struct_p; }

#define _SOURCE_CODE_LOCATION_ __FILE__, __func__, __LINE__


#ifdef _WINDOWS_X86_64_
#define STRING(s) L#s
#elif defined(_LINUX_64BIT_OS_)
#define STRING(s) s
#endif


#endif