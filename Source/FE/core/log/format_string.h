#ifndef _FE_LOG_FORMAT_STRING_H_
#define _FE_LOG_FORMAT_STRING_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>

//std
#include <cstring>
#include <initializer_list>
#define _BUFFERED_STRING_FORMATTER_BUFFER_SIZE_ 2 * 1024




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
void format_string(char* out_buffer_pointer_p, const char* string_format_p, _MAYBE_UNUSED_ size buffer_size_p, const void** arguments_pointer_p, _MAYBE_UNUSED_ count_t arguments_count_p) noexcept;

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
_FORCE_INLINE_ const char* buffered_string_formatter(std::initializer_list<const void*> arguments_p) noexcept
{
    static constexpr auto skip_string_format = 1;

    thread_local static char tl_s_buffer[_BUFFERED_STRING_FORMATTER_BUFFER_SIZE_] = { "\0" };
<<<<<<< HEAD
    std::memset(tl_s_buffer, null, _BUFFERED_STRING_FORMATTER_BUFFER_SIZE_);
=======
    std::memset(tl_s_buffer, _FE_NULL_, _BUFFERED_STRING_FORMATTER_BUFFER_SIZE_);
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

    format_string(tl_s_buffer, static_cast<const char*>(*arguments_p.begin()), _BUFFERED_STRING_FORMATTER_BUFFER_SIZE_, const_cast<const void**>(arguments_p.begin()) + skip_string_format, arguments_p.size());
    return tl_s_buffer;
}

END_NAMESPACE;
#endif