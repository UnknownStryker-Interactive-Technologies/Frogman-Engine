#ifndef _FE_LOG_H_
#define _FE_LOG_H_
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

#include <robin_hood.h>

// std::is_same_v
#include <type_traits>

// std::cerr
#include <iostream>




#ifdef FE_LOG
#error FE_LOG is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_LOG_IF
#error FE_LOG_IF is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_ASSERT
#error FE_ASSERT is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_NEGATIVE_ASSERT
#error FE_NEGATIVE_ASSERT is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_EXIT
#error FE_EXIT is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_DEBUG_BREAK
#error FE_DEBUG_BREAK is a reserved Frogman Engine macro keyword.
#endif

#include <FE/log/logger.hpp>
#include <FE/log/format_string.h>
#include <FE/definitions.h>

// std
#include <cassert>
#include <cstdlib> // std::abort, std::exit

#ifndef __FUNCSIG__
#define __FUNCSIG__ __func__
#endif

#ifdef _MSC_VER
#include <crtdbg.h>
#define FE_DEBUG_BREAK() _CrtDbgBreak()
#else
#define FE_DEBUG_BREAK()
#endif


#ifdef _ENABLE_LOG_
/*
%i8 - int8
%u8 - uint8
%i16 - int16
%u16 - uint16
%d, %i32 - int32
%u, %u32 - uint32
%ld, %i64 - int64
%lu, %u64 - uint64
%f, %f32 - float32
%lf, %f64 - float64
%b - bool
%c - char
%s - string
%p - hexadecimal 64-bit pointer

FE_LOG is a macro that facilitates logging messages by formatting them with a buffered string formatter and including the current file name, function signature, and line number for better traceability in the log output.
*/
#define FE_LOG(...) ::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__)
#else
#define FE_LOG(...)
#endif


#ifdef _ENABLE_LOG_IF_
/*
%i8 - int8
%u8 - uint8
%i16 - int16
%u16 - uint16
%d, %i32 - int32
%u, %u32 - uint32
%ld, %i64 - int64
%lu, %u64 - uint64
%f, %f32 - float32
%lf, %f64 - float64
%b - bool
%c - char
%s - string
%p - hexadecimal 64-bit pointer

FE_LOG_IF is a macro that logs messages conditionally based on a specified condition
utilizing a logger to format and output the message along with the file name and line number where the log was triggered.
*/
#define FE_LOG_IF(condition, ...) \
if(condition) \
{ \
	::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
}
#else
#define FE_LOG_IF(condition, ...)
#endif


#ifdef _ENABLE_NEGATIVE_ASSERT_
/*
%i8 - int8
%u8 - uint8
%i16 - int16
%u16 - uint16
%d, %i32 - int32
%u, %u32 - uint32
%ld, %i64 - int64
%lu, %u64 - uint64
%f, %f32 - float32
%lf, %f64 - float64
%b - bool
%c - char
%s - string
%p - hexadecimal 64-bit pointer
Negative Assertion Macro

FE_NEGATIVE_ASSERT is a macro that triggers a fatal error log and aborts the program if the given expression evaluates to true
logging a formatted message along with the file name and line number.
*/
#define FE_NEGATIVE_ASSERT(expression, ...) \
if(expression) _FE_UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	std::abort(); \
}
#else
#define FE_NEGATIVE_ASSERT(expression, ...)
#endif


#ifdef _ENABLE_ASSERT_
/*
%i8 - int8
%u8 - uint8
%i16 - int16
%u16 - uint16
%d, %i32 - int32
%u, %u32 - uint32
%ld, %i64 - int64
%lu, %u64 - uint64
%f, %f32 - float32
%lf, %f64 - float64
%b - bool
%c - char
%s - string
%p - hexadecimal 64-bit pointer

FE_ASSERT is a macro that checks a given expression and logs a fatal error message along with the file name, function signature, and line number before terminating the program if the expression evaluates to false.
*/
#define FE_ASSERT(expression, ...) \
if(!(expression)) _FE_UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	std::abort(); \
}
#else
#define FE_ASSERT(expression, ...)
#endif


#ifdef FE_EXIT
	#error FE_EXIT is a reserved Frogman Engine macro keyword.
#endif
/*
%i8 - int8
%u8 - uint8
%i16 - int16
%u16 - uint16
%d, %i32 - int32
%u, %u32 - uint32
%ld, %i64 - int64
%lu, %u64 - uint64
%f, %f32 - float32
%lf, %f64 - float64
%b - bool
%c - char
%s - string
%p - hexadecimal 64-bit pointer

The FE_EXIT macro logs a fatal error message and terminates the program with a specified error code if a given expression evaluates to true.
*/
#define FE_EXIT(expression, error_code, ...) \
if(expression) _FE_UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	::std::exit(static_cast<::FE::int32>(error_code)); \
} 


#define TO_STRING(p) #p

#define _FE_NODEFAULT_ default: _FE_UNLIKELY_ FE_EXIT(true, ::FE::ErrorCode::_FatalSwitchCaseError_ReachedNoDefault, "Reached Default Case: This switch has no default."); break;


namespace FE
{
	enum struct ErrorCode : FE::int32
	{
		_None = 0,
		_FatalHardwareResourceError_CPU_HasNotEnoughThreads = 1,
		_FatalError_DynamicCastFailure_TypeMismatch = 2,
		_FatalLoggerError_IncorrectStringFormatterSyntex = 3,
		_FatalSwitchCaseError_ReachedNoDefault = 4,

		_FatalMemoryError_1XX_IllegalAddressAlignment = 100,
		_FatalMemoryError_1XX_NullPtr = 101,
		_FatalMemoryError_1XX_AccessViolation = 102,
		_FatalMemoryError_1XX_HeapCorruption = 103,
		_FatalMemoryError_1XX_DoubleFree = 104,
		_FatalMemoryError_1XX_BufferOverflow = 105,
		_FatalMemoryError_1XX_InvalidSize = 106,
		_FatalMemoryError_1XX_InvalidIterator = 107,
		_FatalMemoryError_1XX_VirtualAllocFailure = 108,
		_FatalMemoryError_1XX_VirtualLockFailure = 109,
		_FatalMemoryError_1XX_VirtualUnlockFailure = 110,
		_FatalMemoryError_1XX_VirtualFreeFailure = 110,
		_FatalMemoryError_1XX_FalseDeallocation = 111,

		_FatalInputError_2XX_InvalidArgument = 200,
		_FatalInputError_2XX_Null = 202,

		_FatalSerializationError_3XX_TypeMismatch = 300,
		_FatalSerializationError_3XX_TypeNotFound = 301
	};

	_FE_FORCE_INLINE_ int error_code_to_int(const FE::ErrorCode error_code_p) noexcept
	{
		return static_cast<int>(error_code_p);
	}

	_FE_FORCE_INLINE_ FE::ASCII* error_code_to_string(const FE::ErrorCode code_p) noexcept
	{
		static const robin_hood::unordered_map<FE::ErrorCode, FE::ASCII*>  l_s_message_table =
		{	
			{ FE::ErrorCode::_None, "Frogman Engine Error: Not an Error." },
			{ FE::ErrorCode::_FatalHardwareResourceError_CPU_HasNotEnoughThreads, "Frogman Engine Fatal Hardware Resource Error: CPU Has Not Enough Threads." },
			{ FE::ErrorCode::_FatalError_DynamicCastFailure_TypeMismatch, "Frogman Engine Fatal Error: Dynamic Cast Failed." },
			{ FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex, "Frogman Engine Fatal Logger Error: Incorrect String Formatter Syntex." },
			{ FE::ErrorCode::_FatalSwitchCaseError_ReachedNoDefault, "Frogman Engine Fatal Switch Case Error: Reached No Default." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_IllegalAddressAlignment, "Frogman Engine Fatal Memory Error: The Address is Not Properly Aligned." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_NullPtr, "Frogman Engine Fatal Memory Error: Cannot Dereference a Null Pointer." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation, "Frogman Engine Fatal Memory Error: The Program is Attempting to Access an Undesired Location." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_HeapCorruption, "Frogman Engine Fatal Memory Error: The Program Heap Corruption Detected." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_DoubleFree, "Frogman Engine Fatal Memory Error: Double Free Detected." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "Frogman Engine Fatal Memory Error: The Buffer Overflow Detected." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize, "Frogman Engine Fatal Memory Error: Detected an Invalid Memory Size Argument." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_InvalidIterator, "Frogman Engine Fatal Memory Error: The Iterator is Pointing to an Invalid Address." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_VirtualAllocFailure, "Frogman Engine Fatal Memory Error: Virtual Alloc Failed." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_VirtualLockFailure, "Frogman Engine Fatal Memory Error: Virtual Lock Failed." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_VirtualUnlockFailure, "Frogman Engine Fatal Memory Error: Virtual Unlock Failed." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_VirtualFreeFailure, "Frogman Engine Fatal Memory Error: Virtual Free Failed." },
			{ FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Frogman Engine Fatal Memory Error: False Deallocation Detected." },
			{ FE::ErrorCode::_FatalInputError_2XX_InvalidArgument, "Frogman Engine Fatal Input Error: an Invalid Argument Detected." },
			{ FE::ErrorCode::_FatalInputError_2XX_Null, "Frogman Engine Fatal Input Error: The Value is a Null." },
			{ FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "Frogman Engine Fatal Serialization Error: Serialization of Mismatching Type." },
			{ FE::ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "Frogman Engine Fatal Serialization Error: The Instance Memory Layout is Not Known to the Engine's Reflection System." }
		};

		return l_s_message_table.at(code_p);
	}
}
#endif