#ifndef _FE_LOG_FORMAT_STRING_H_
#define _FE_LOG_FORMAT_STRING_H_
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

//std
#include <cstring>
#include <initializer_list>
constexpr inline FE::uint32 string_formatter_buffer_size = 4096;




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
void format_string(char* out_buffer_pointer_p, const char* string_format_p, size buffer_size_p, const void** arguments_pointer_p, count_t arguments_count_p) noexcept;

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
_FE_FORCE_INLINE_ const char* buffered_string_formatter(std::initializer_list<const void*> arguments_p) noexcept
{
    thread_local static char tl_s_buffer[string_formatter_buffer_size] = { "\0" };
    std::memset(tl_s_buffer, null, string_formatter_buffer_size);

    if (arguments_p.begin() != nullptr)
    {
        format_string(tl_s_buffer, static_cast<const char*>(*arguments_p.begin()), string_formatter_buffer_size, const_cast<const void**>(arguments_p.begin()) + 1, arguments_p.size());
    }
    return tl_s_buffer;
}

END_NAMESPACE;
#endif