#ifndef _FE_LOG_H_
#define _FE_LOG_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#ifdef FE_LOG
#error FE_LOG is a reserved Frogman Engine macro keyword.
#endif 
#ifdef FE_ASSERT
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
#include <FE/attributes.h>
#include <cassert>
#include <cstdlib>

#define _ERROR_FROM_FE_CORE_EXIT_CODE_ (-70)
#ifndef __FUNCSIG__
#define __FUNCSIG__ __func__
#endif


#ifdef _ENABLE_LOG_
/*
${% d at n} - int32
${%u at n} - uint32
${%ld at n} - int64
${%lu at n} - uint64
${%lf at n} - float64
${%f at n} - float32
${%b at n} - bool
${%c at n} - char
${%s at n} - string
${%p at n} - hexadecimal 64bit pointer | * A file and console logger_base with a string formatter.
*/
#define FE_LOG(...) ::FE::log::logger_base::get_logger<::FE::log::message_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__)
#else
#define FE_LOG(...)
#endif


#ifdef _ENABLE_ASSERT_
// ${%d at n} - int32
// ${%u at n} - uint32
// ${%ld at n} - int64
// ${%lu at n} - uint64
// ${%lf at n} - float64
// ${%f at n} - float32
// ${%b at n} - bool
// ${%c at n} - char
// ${%s at n} - string
// ${%p at n} - hexadecimal 64bit pointer | *Dynamically asserts the condition, that must not be true at runtime. 
#define FE_ASSERT(expression, ...) \
if(UNLIKELY(expression)) _UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	assert(!(expression)); \
}
#else
#define FE_ASSERT(expression, ...)
#endif


#ifdef _ENABLE_EXIT_
/*
${% d at n} - int32   | ${% u at n} - uint32  
${% ld at n} - int64  | ${%lu at n} - uint64
${%lf at n} - float64 | ${%f at n} - float32
${%b at n}  - bool    | ${%c at n}  - char    
${%s at n}  - string  | ${%p at n}  - hexadecimal 64bit pointer
*Dynamically asserts the condition, that must not be true at runtime, and exits the program with the given error code.
*/
#define FE_EXIT(expression, error_code, ...) \
if(UNLIKELY(expression)) _UNLIKELY_ \
{ \
	::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
	::std::exit(static_cast<::var::int32>(error_code)); \
}
#else
#define FE_EXIT(expression, error_code, ...)
#endif


/*
${% d at n} - int32   | ${% u at n} - uint32
${% ld at n} - int64  | ${%lu at n} - uint64
${%lf at n} - float64 | ${%f at n} - float32
${%b at n}  - bool    | ${%c at n}  - char
${%s at n}  - string  | ${%p at n}  - hexadecimal 64bit pointer
*If the given function call does not return the expected value, the program will exit with an error code, retrieved from the method. 
*/
#define FE_EXPECT(fn_call, expected_value, ...)\
{ \
	auto __FE_EXPECT_RESULT__ = fn_call; \
	if(UNLIKELY(__FE_EXPECT_RESULT__ != expected_value)) _UNLIKELY_ \
	{ \
		::FE::log::logger_base::get_fatal_error_logger<::FE::log::fatal_error_logger_base>().do_log(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __FUNCSIG__, __LINE__); \
		if constexpr (::FE::is_boolean<decltype(__FE_EXPECT_RESULT__)>::value == true) \
		{ \
			::std::exit(_ERROR_FROM_FE_CORE_EXIT_CODE_); \
		} \
		else if constexpr (::FE::is_boolean<decltype(__FE_EXPECT_RESULT__)>::value == false) \
		{ \
			::std::exit((::var::int32)__FE_EXPECT_RESULT__); \
		} \
	} \
}


#define TO_STRING(p) #p

#define _NODEFAULT_ default: _UNLIKELY_ FE_ASSERT(true, "Reached Default Case: This switch has no default."); break;


namespace FE
{
	enum struct ERROR_CODE
	{
		_NONE = 0,
		_FATAL_MEMORY_ERROR_1XX_ILLEGAL_ADDRESS_ALIGNMENT = 100,
		_FATAL_MEMORY_ERROR_1XX_NULLPTR = 101,
		_FATAL_MEMORY_ERROR_ACCESS_VIOLATION = 102,
		_FATAL_MEMORY_ERROR_HEAP_CORRUPTION = 103,
		_FATAL_MEMORY_ERROR_DOUBLE_FREE = 104,
		_FATAL_MEMORY_ERROR_OUT_OF_RANGE = 105,
		_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY = 106,
		_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE = 107,
		_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING = 108,

		_FATAL_INPUT_ERROR_2XX_INVALID_ARGUMENT = 200,
		_FATAL_INPUT_ERROR_2XX_INVALID_KEY = 201,
		_FATAL_INPUT_ERROR_2XX_NULL = 202,

		_FATAL_SERIALIZATION_ERROR_3XX_TYPE_MISMATCH = 300
	};
}

#endif