#ifndef _FE_LOG_MACRO_H_
#define _FE_LOG_MACRO_H_
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

#include <FE/core/log/logger.hpp>
#include <FE/core/log/format_string.h>
#include <FE/core/macros/attributes.h>
#include <cassert>
#include <cstdlib>

#define _ERROR_FROM_FE_CORE_EXIT_CODE_ (-70)




#ifdef _ENABLE_LOG_
// ${%d at n} - int32
// ${%u at n} - uint32
// ${%ld at n} - int64
// ${%lu at n} - uint64
// ${%lf at n} - float64
// ${%f at n} - float32
// ${%b at n} - bool
// ${%c at n} - char
// ${%s at n} - string
// ${%p at n} - hexadecimal 64bit pointer
#define FE_LOG(...) ::FE::internal::log::__FE_LOG_IMPLEMENTATION(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __func__, __LINE__)
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
// ${%p at n} - hexadecimal 64bit pointer  |  FE_ASSERT() invokes abort() if the expression is true.
#define FE_ASSERT(expression, ...) \
if(expression) _UNLIKELY_ \
{ \
	::FE::internal::log::__FE_ABORT_IMPLEMENTATION(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __func__, __LINE__); \
	assert(!(expression)); \
}
#else
#define FE_ASSERT(expression, ...)
#endif


#ifdef _ENABLE_EXIT_
// ${%d at n} - int32
// ${%u at n} - uint32
// ${%ld at n} - int64
// ${%lu at n} - uint64
// ${%lf at n} - float64
// ${%f at n} - float32
// ${%b at n} - bool
// ${%c at n} - char
// ${%s at n} - string
// ${%p at n} - hexadecimal 64bit pointer  |  FE_EXIT() invokes exit() if the expression is true.
#define FE_EXIT(expression, error_code, ...) \
if(expression) _UNLIKELY_ \
{ \
	::FE::internal::log::__FE_ABORT_IMPLEMENTATION(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __func__, __LINE__); \
	::std::exit(static_cast<::FE::var::int32>(error_code)); \
}
#else
#define FE_EXIT(expression, error_code, ...)
#endif


#define FE_EXPECT(fn_call, expected_value, ...)\
{ \
	auto __FE_EXPECT_RESULT__ = fn_call; \
	if(__FE_EXPECT_RESULT__ != expected_value) _UNLIKELY_ \
	{ \
		::FE::internal::log::__FE_ABORT_IMPLEMENTATION(::FE::log::buffered_string_formatter({ __VA_ARGS__ }), __FILE__, __func__, __LINE__); \
		if constexpr (::FE::is_boolean<decltype(__FE_EXPECT_RESULT__)>::value == true) \
		{ \
			::std::exit(_ERROR_FROM_FE_CORE_EXIT_CODE_); \
		} \
		else if constexpr (::FE::is_boolean<decltype(__FE_EXPECT_RESULT__)>::value == false) \
		{ \
			::std::exit((::FE::var::int32)__FE_EXPECT_RESULT__); \
		} \
	} \
}


#define TO_STRING(p) #p

#define _NODEFAULT_ default: _UNLIKELY_ FE_ASSERT(true, "Reached Default Case: This switch has no default."); break;


enum struct INPUT_ERROR_2XX : FE::int32
{
	_FATAL_ERROR_INVALID_ARGUMENT = 200,
	_FATAL_ERROR_INVALID_KEY = 201
};


#endif