#ifndef _FE_LOG_HXX_
#define _FE_LOG_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

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
#ifdef FE_EXIT_IF
#error FE_EXIT_IF is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_DEBUG_BREAK
#error FE_DEBUG_BREAK is a reserved Frogman Engine macro keyword.
#endif

#include <FE/log/logger.hxx>
#include <FE/log/format_string.hxx>
#include <FE/definitions.hxx>

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
#define FE_LOG(severity, ...) ::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__, severity)
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
#define FE_LOG_IF(condition, severity, ...) \
{ \
	if(condition) \
	{ \
		::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__, severity); \
	} \
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
{ \
	if(expression) _FE_UNLIKELY_ \
	{ \
		::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
		FE_DEBUG_BREAK(); \
		std::abort(); \
	} \
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
{ \
	if(!(expression)) _FE_UNLIKELY_ \
	{ \
		::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
		FE_DEBUG_BREAK(); \
		std::abort(); \
	} \
}

#else
#define FE_ASSERT(expression, ...)
#endif


#ifdef FE_EXIT_IF
	#error FE_EXIT_IF is a reserved Frogman Engine macro keyword.
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

The FE_EXIT_IF macro logs a fatal error message and terminates the program with a specified error code if a given expression evaluates to true.
*/
#define FE_EXIT_IF(expression, error_code, ...) \
{ \
	if(expression) _FE_UNLIKELY_ \
	{ \
		::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
		::std::exit(static_cast<::FE::int32>(error_code)); \
	} \
}


#define TO_STRING(p) #p

#define _FE_NODEFAULT_ default: _FE_UNLIKELY_ FE_EXIT_IF(true, ::FE::ErrorCode::_FatalSwitchCaseError_ReachedNoDefault, "Reached Default Case: This switch has no default."); break;


namespace FE
{
	FE_ENUM_STRUCT();
	enum struct ErrorCode : FE::int32
	{
		_None = 0,
		_FatalHardwareResourceError_CPU_HasNotEnoughThreads = 1,
		_FatalError_DynamicCastFailure_TypeMismatch = 2,
		_FatalLoggerError_IncorrectStringFormatterSyntex = 3,
		_FatalSwitchCaseError_ReachedNoDefault = 4,
		_FatalError_TableInsertionFailure = 5,

		_FatalMemoryError_1XX_IncorrectAddressAlignment = 100,
		_FatalMemoryError_1XX_NullPtr = 101,
		_FatalMemoryError_1XX_AccessViolation = 102,
		_FatalMemoryError_1XX_HeapCorruption = 103,
		_FatalMemoryError_1XX_DoubleFree = 104,
		_FatalMemoryError_1XX_BufferOverflow = 105,
		_FatalMemoryError_1XX_InvalidSize = 106,
		_FatalMemoryError_1XX_InvalidIterator = 107,
		_FatalMemoryError_1XX_FalseDeallocation = 108,

		_FatalInputError_2XX_InvalidArgument = 200,
		_FatalInputError_2XX_Null = 201,

		_FatalSerializationError_3XX_TypeMismatch = 300,
		_FatalSerializationError_3XX_TypeNotFound = 301,
		_FatalSerializationError_3XX_FileVersionMismatch = 302,
		_FatalDeserializationError_3XX_FileBufferEmpty = 303,

		_FatalWinAPI_Error_4XX_OpenProcessTokenFailure = 400,
		_FatalWinAPI_Error_4XX_LookupPrivilegeValueFailure = 401,
		_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure = 402,
		_FatalMemoryError_4XX_VirtualAllocFailure = 403,
		_FatalMemoryError_4XX_VirtualLockFailure = 404,
		_FatalMemoryError_4XX_VirtualUnlockFailure = 405,
		_FatalMemoryError_4XX_VirtualFreeFailure = 406,

		_FatalRendererError_5XX_GLFW_InitializationFailure = 500,
		_FatalRendererError_5XX_GLFW_WindowCreationFailure = 501,
		_FatalRendererError_5XX_RendererBackendDeviceCreationFailure = 502,
		_FatalRendererError_5XX_RendererSwapChainCreationFailure = 503,
		_FatalRendererError_5XX_RendererRenderTargetViewCreationFailure = 504
	};
}
#endif