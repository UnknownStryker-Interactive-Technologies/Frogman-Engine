#ifndef _FE_LOG_H_
#define _FE_LOG_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#ifdef FE_LOG
#error FE_LOG is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_NEGATIVE_ASSERT
#error FE_ASSERT is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_EXIT
#error FE_EXIT is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_EXPECT
#error FE_EXPECT is a reserved Frogman Engine macro keyword.
#endif 

#include <FE/log/logger.hpp>
#include <FE/log/format_string.h>
#include <FE/definitions.h>

// std
#include <cassert>
#include <cstdlib>

#ifndef __FUNCSIG__
#define __FUNCSIG__ __func__
#endif


#ifdef _ENABLE_LOG_
/*
${%d@n} - int32
${%u@n} - uint32
${%ld@n} - int64
${%lu@n} - uint64
${%lf@n} - float64
${%f@n} - float32
${%b@n} - bool
${%c@n} - char
${%s@n} - string
${%p@n} - hexadecimal 64bit pointer
*/
#define FE_LOG(...) ::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__)
#else
#define FE_LOG(...)
#endif


#ifdef _ENABLE_LOG_IF_
/*
${%d@n} - int32
${%u@n} - uint32
${%ld@n} - int64
${%lu@n} - uint64
${%lf@n} - float64
${%f@n} - float32
${%b@n} - bool
${%c@n} - char
${%s@n} - string
${%p@n} - hexadecimal 64bit pointer
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
${%d@n} - int32
${%u@n} - uint32
${%ld@n} - int64
${%lu@n} - uint64
${%lf@n} - float64
${%f@n} - float32
${%b@n} - bool
${%c@n} - char
${%s@n} - string
${%p@n} - hexadecimal 64bit pointer
*/ // Negative Assertion Macro
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
${%d@n} - int32
${%u@n} - uint32
${%ld@n} - int64
${%lu@n} - uint64
${%lf@n} - float64
${%f@n} - float32
${%b@n} - bool
${%c@n} - char
${%s@n} - string
${%p@n} - hexadecimal 64bit pointer
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


#ifdef _ENABLE_EXIT_
/*
${%d@n} - int32
${%u@n} - uint32
${%ld@n} - int64
${%lu@n} - uint64
${%lf@n} - float64
${%f@n} - float32
${%b@n} - bool
${%c@n} - char
${%s@n} - string
${%p@n} - hexadecimal 64bit pointer
*/
#define FE_EXIT(expression, error_code, ...) \
if(expression) _FE_UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	::std::exit(static_cast<::var::int32>(error_code)); \
}
#else
	#define FE_EXIT(expression, error_code, ...)
#endif


#define TO_STRING(p) #p

#define _NODEFAULT_ default: _FE_UNLIKELY_ FE_NEGATIVE_ASSERT(true, "Reached Default Case: This switch has no default."); break;


namespace FE
{
	enum struct ERROR_CODE : FE::int32
	{
		_NONE = 0,
		_FATAL_LOGGER_ERROR_0XX_INCORRECT_STRING_FORMATTER_SYNTEX = 1,

		_FATAL_MEMORY_ERROR_1XX_ILLEGAL_ADDRESS_ALIGNMENT = 100,
		_FATAL_MEMORY_ERROR_1XX_NULLPTR = 101,
		_FATAL_MEMORY_ERROR_1XX_ACCESS_VIOLATION = 102,
		_FATAL_MEMORY_ERROR_1XX_HEAP_CORRUPTION = 103,
		_FATAL_MEMORY_ERROR_1XX_DOUBLE_FREE = 104,
		_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW = 105,
		_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE = 106,
		_FATAL_MEMORY_ERROR_1XX_INVALID_ITERATOR = 107,

		_FATAL_INPUT_ERROR_2XX_INVALID_ARGUMENT = 200,
		_FATAL_INPUT_ERROR_2XX_NULL = 202,

		_FATAL_SERIALIZATION_ERROR_3XX_TYPE_MISMATCH = 300,
		_FATAL_SERIALIZATION_ERROR_3XX_TYPE_NOT_FOUND = 301,

		_FATAL_ERROR_DYNAMIC_CAST_FAILURE_DUE_TO_TYPE_MISMATCH = 400
	};

	_FE_FORCE_INLINE_ int error_code_cast(const ERROR_CODE error_code_p) noexcept
	{
		return static_cast<int>(error_code_p);
	}
}
#endif