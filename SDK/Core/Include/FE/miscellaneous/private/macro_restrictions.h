#ifndef _FE_MACRO_RESTRICTIONS_H_
#define _FE_MACRO_RESTRICTIONS_H_
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
#include <FE/types.hxx>


//#if !defined(_CLANG_CL_) && !defined(_CLANG_)
//#error Frogman Engine requires LLVM Clang to compile.
//#endif

#if !defined(_FE_ON_WINDOWS_X86_64_) && !defined(_FE_ON_LINUX_X86_64_)
#error An Incompatible System Environment Detected: Frogman Engine supports Windows x86-64 and Linux(Ubuntu) x86-64. Please use one of them.
#endif


#ifdef FE
#error Frogman Engine Prohibits macroizing the keyword "FE".
#endif
#ifdef var
#error Frogman Engine Prohibits macroizing the keyword "var".
#endif
#ifdef framework
#error Frogman Engine Prohibits macroizing the keyword "framework".
#endif
#ifdef internal
#error Frogman Engine Prohibits macroizing the keyword "internal".
#endif


#ifdef null
#error Frogman Engine Prohibits macroizing the keyword "null".
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

#ifdef ASCII
#error Frogman Engine Prohibits macroizing the keyword "ASCII".
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

#ifdef size
#error Frogman Engine Prohibits macroizing the keyword "size".
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
#ifdef capacity_t
#error Frogman Engine Prohibits macroizing the keyword "capacity_t".
#endif

#ifdef ptrdiff
#error Frogman Engine Prohibits macroizing the keyword "ptrdiff".
#endif
#ifdef intptr
#error Frogman Engine Prohibits macroizing the keyword "intptr".
#endif
#ifdef uintptr
#error Frogman Engine Prohibits macroizing the keyword "uintptr".
#endif

#ifdef directory_char_t
#error Frogman Engine Prohibits macroizing the keyword "directory_char_t".
#endif


#ifdef lazy_const
#error Frogman Engine Prohibits macroizing the keyword "lazy_const".
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

#endif