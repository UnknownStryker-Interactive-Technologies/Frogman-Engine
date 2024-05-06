#ifndef _FE_MACRO_RESTRICTIONS_H_
#define _FE_MACRO_RESTRICTIONS_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>


#if !defined(_MSVC_) && !defined(_GNUC_)&& !defined(_CLANG_)
#error Frogman Engine requires Microsoft Visual Studio 2022 C++ v143, LLVM Clang, g++ 11 ,or latest g++ to compile. Please use one of these compilers.
#endif
#if !defined(_WINDOWS_X86_64_) && !defined(_LINUX_64BIT_OS_)
#error An Incompatible System Environment Detected: Frogman Engine supports Windows x86-64 and Linux(Ubuntu) x86-64. Please use one of them.
#endif
#ifdef __APPLE__
#error Apple platforms are not supported. Please use Windows x86-64 or Linux(Ubuntu) x84-64 Operating System.
#endif
#ifndef _X64_
#error Frogman Engine is only works on X86-64 environment.
#endif


#ifdef FE
#error Frogman Engine Prohibits macroizing the keyword "FE".
#endif
#ifdef var
#error Frogman Engine Prohibits macroizing the keyword "var".
#endif

#ifdef const
#error Frogman Engine Prohibits macroizing the keyword "const".
#endif
#ifdef constexpr
#error Frogman Engine Prohibits macroizing the keyword "constexpr".
#endif

#ifdef nodiscard
#error Frogman Engine Prohibits macroizing the keyword "nodiscard".
#endif
#ifdef likely
#error Frogman Engine Prohibits macroizing the keyword "likely".
#endif
#ifdef unlikely
#error Frogman Engine Prohibits macroizing the keyword "unlikely".
#endif
#ifdef fallthrough
#error Frogman Engine Prohibits macroizing the keyword "fallthrough".
#endif
#ifdef maybe_unused
#error Frogman Engine Prohibits macroizing the keyword "maybe_unused".
#endif
#ifdef noreturn
#error Frogman Engine Prohibits macroizing the keyword "noreturn".
#endif
#ifdef deprecated
#error Frogman Engine Prohibits macroizing the keyword "deprecated".
#endif


#ifdef max_value
#error Frogman Engine Prohibits macroizing the keyword "max_value".
#endif
#ifdef min_value
#error Frogman Engine Prohibits macroizing the keyword "min_value".
#endif


#ifdef boolean
#error Frogman Engine Prohibits macroizing the keyword "boolean".
#endif

#ifdef schar
#error Frogman Engine Prohibits macroizing the keyword "schar".
#endif
#ifdef uchar
#error Frogman Engine Prohibits macroizing the keyword "uchar".
#endif
#ifdef wchar
#error Frogman Engine Prohibits macroizing the keyword "wchar".
#endif
#ifdef UTF8
#error Frogman Engine Prohibits macroizing the keyword "char8".
#endif
#ifdef UTF16
#error Frogman Engine Prohibits macroizing the keyword "char16".
#endif
#ifdef UTF32
#error Frogman Engine Prohibits macroizing the keyword "char32".
#endif

#ifdef float32
#error Frogman Engine Prohibits macroizing the keyword "float32".
#endif
#ifdef float64
#error Frogman Engine Prohibits macroizing the keyword "float64".
#endif

#ifdef binary
#error Frogman Engine Prohibits macroizing the keyword "binary".
#endif
#ifdef byte
#error Frogman Engine Prohibits macroizing the keyword "sbyte".
#endif
#ifdef ubyte
#error Frogman Engine Prohibits macroizing the keyword "ubyte".
#endif
#ifdef int8
#error Frogman Engine Prohibits macroizing the keyword "int8".
#endif
#ifdef uint8
#error Frogman Engine Prohibits macroizing the keyword "uint8".
#endif
#ifdef int16
#error Frogman Engine Prohibits macroizing the keyword "int16".
#endif
#ifdef uint16
#error Frogman Engine Prohibits macroizing the keyword "uint16".
#endif
#ifdef int32
#error Frogman Engine Prohibits macroizing the keyword "int32".
#endif
#ifdef uint32
#error Frogman Engine Prohibits macroizing the keyword "uint32".
#endif
#ifdef int64
#error Frogman Engine Prohibits macroizing the keyword "int64".
#endif
#ifdef uint64
#error Frogman Engine Prohibits macroizing the keyword "uint64".
#endif

#ifdef size_t
#error Frogman Engine Prohibits macroizing the keyword "size_t".
#endif
#ifdef length_t
#error Frogman Engine Prohibits macroizing the keyword "length_t".
#endif
#ifdef index_t
#error Frogman Engine Prohibits macroizing the keyword "index_t".
#endif
#ifdef count_t
#error Frogman Engine Prohibits macroizing the keyword "count_t".
#endif


#ifdef T
#error Frogman Engine Prohibits macroizing the keyword "T".
#endif
#ifdef U
#error Frogman Engine Prohibits macroizing the keyword "U".
#endif
#ifdef IntT
#error Frogman Engine Prohibits macroizing the keyword "IntT".
#endif
#ifdef CharT
#error Frogman Engine Prohibits macroizing the keyword "CharT".
#endif


#ifdef ATOMIC_BYTE_PTR
#error Frogman Engine Prohibits macroizing the keyword "ATOMIC_BYTE_PTR".
#endif
#ifdef BYTE_PTR
#error Frogman Engine Prohibits macroizing the keyword "BYTE_PTR".
#endif
#ifdef WORD_PTR
#error Frogman Engine Prohibits macroizing the keyword "WORD_PTR".
#endif
#ifdef DWORD_PTR
#error Frogman Engine Prohibits macroizing the keyword "DWORD_PTR".
#endif
#ifdef QWORD_PTR
#error Frogman Engine Prohibits macroizing the keyword "QWORD_PTR".
#endif


#endif