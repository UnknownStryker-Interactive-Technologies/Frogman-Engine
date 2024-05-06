#ifndef _FE_LOG_FORMAT_STRING_H_
#define _FE_LOG_FORMAT_STRING_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>
#include <initializer_list>




BEGIN_NAMESPACE(FE::log)

// %d - int32
// %u - uint32
// %ld - int64
// %lu - uint64
// %lf - float64
// %f - float32
// %b - bool
// %c - char
// %s - string
// %p - hexadecimal 64bit pointer
void format_string(char* out_buffer_pointer_p, const char* string_format_p, _MAYBE_UNUSED_ size_t buffer_size_p, const void** arguments_pointer_p, _MAYBE_UNUSED_ count_t arguments_count_p) noexcept;

// %d - int32
// %u - uint32
// %ld - int64
// %lu - uint64
// %lf - float64
// %f - float32
// %b - bool
// %c - char
// %s - string
// %p - hexadecimal 64bit pointer
const char* buffered_string_formatter(std::initializer_list<const void*> arguments_p) noexcept;


END_NAMESPACE;
#endif