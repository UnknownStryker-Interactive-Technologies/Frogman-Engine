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
#include "FE/log/format_string.h"
#include <FE/algorithm/utility.hxx>
#include <FE/private/allocator_base.hpp>

// std
#include <cmath>
#include <cstdio>
#include <cstdlib>




BEGIN_NAMESPACE(FE::log)


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

_FE_FORCE_INLINE_ bool __is_beginning_of_the_replacement_format(const char* string_format_p) noexcept
{
    return (*string_format_p == '$') 
        && (string_format_p[1] == '{')
        && (string_format_p[2] == '%');
}

_FE_FORCE_INLINE_ char __get_the_first_letter_of_the_format_specifier(const char* string_format_p) noexcept
{
	return string_format_p[3];
}

_FE_FORCE_INLINE_ char __get_the_second_letter_of_the_format_specifier(const char* string_format_p) noexcept
{
    return string_format_p[4];
}

_FE_FORCE_INLINE_ bool __is_index_tocken(const char* string_format_p) noexcept
{
	return (string_format_p[4] == '@') || (string_format_p[5] == '@');
}

void format_string(char* out_buffer_pointer_p, const char* string_format_p, size buffer_size_p, const void** arguments_pointer_p, count_t arguments_count_p) noexcept
{
    if (std::strlen(string_format_p) > buffer_size_p) _FE_UNLIKELY_
    {
        std::cerr << "ERROR: buffer overflowed!";
        std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW));
    }

    while (*string_format_p != '\0')
    {
        if (__is_beginning_of_the_replacement_format(string_format_p))
        {
            var::int64 l_args_index_converted_from_string = 0;
            var::int64 l_index_digit_length = 0;
            if (__is_index_tocken(string_format_p))
            {
                //                            "${%d at n}"
                // The pointer below will point        ^
                const char* l_format_specifier_value_prefix = string_format_p + ((string_format_p[4] == '@') ? 5 : 6);
                l_args_index_converted_from_string = atoll(l_format_specifier_value_prefix); // Convert the string to a number.
				
                if (l_args_index_converted_from_string < 0) _FE_UNLIKELY_
                {
                    std::cerr << "ERROR: Detected an invalid index token. The @n value is negative.";
                    std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
                }
                
                for (var::int64 i = l_args_index_converted_from_string; i >= 10; i /= 10) { ++l_index_digit_length; }
                ++l_index_digit_length;

				// At the end of the number string, there must be a '}' character.
                if (l_format_specifier_value_prefix[l_index_digit_length] != '}') _FE_UNLIKELY_
                {
                    std::cerr << "ERROR: an illegal string format detected! } is missing.";
                    std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_LOGGER_ERROR_0XX_INCORRECT_STRING_FORMATTER_SYNTEX));
                }

				// The index value must be less than the arguments count. Since it is an index to arguments.
                if (static_cast<FE::count_t>(l_args_index_converted_from_string) >= arguments_count_p) _FE_UNLIKELY_
                {
                    std::cerr << "ERROR: Detected an invalid index token. The @n value is too large.";
                    std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
                }
            }
            else
            {
                continue;
            }
        
            //                                                           "${%d@n}"
            // Would return the first letter of the current format_specifier ^
            switch (__get_the_first_letter_of_the_format_specifier(string_format_p))
            {
            case 'd': // %d - int32
            {
                std::snprintf(out_buffer_pointer_p, buffer_size_p, "%d", *static_cast<int32*>( arguments_pointer_p[l_args_index_converted_from_string] ));
                out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                string_format_p += (std::strlen("${%d@}") + l_index_digit_length);
                continue; 
            }

            case 'u': // %u - uint32
            {
                std::snprintf(out_buffer_pointer_p, buffer_size_p, "%u", *static_cast<uint32*>(arguments_pointer_p[l_args_index_converted_from_string]));
                out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                string_format_p += (std::strlen("${%u@}") + l_index_digit_length);
                continue;
            }

            case 'l':
                switch (__get_the_second_letter_of_the_format_specifier(string_format_p))
                {
                case 'd': // %ld - int64
                {
                    std::snprintf(out_buffer_pointer_p, buffer_size_p, "%lld", *static_cast<int64*>(arguments_pointer_p[l_args_index_converted_from_string]));
                    out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                    string_format_p += (std::strlen("${%ld@}") + l_index_digit_length);
                    continue;
                }

                case 'u': // %lu - uint64
                {
                    std::snprintf(out_buffer_pointer_p, buffer_size_p, "%llu", *static_cast<uint64*>(arguments_pointer_p[l_args_index_converted_from_string]));
                    out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                    string_format_p += (std::strlen("${%lu@}") + l_index_digit_length);
                    continue;
                }

                case 'f': // %lf - float64
                {
                    std::snprintf(out_buffer_pointer_p, buffer_size_p, "%lf", *static_cast<float64*>(arguments_pointer_p[l_args_index_converted_from_string]));
                    out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                    string_format_p += (std::strlen("${%lf@}") + l_index_digit_length);
                    continue;
                }

                default: _FE_UNLIKELY_
                    std::cerr << "ERROR: an incorrect type format option detected! The option must be one of %ld (int64), %lu (uint64), and %lf (float64).";
					std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_LOGGER_ERROR_0XX_INCORRECT_STRING_FORMATTER_SYNTEX));
                    return;
                }

            case 'f': // %f - float32
            {
                std::snprintf(out_buffer_pointer_p, buffer_size_p, "%f", *static_cast<float32*>(arguments_pointer_p[l_args_index_converted_from_string]));
                out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                string_format_p += (std::strlen("${%f@}") + l_index_digit_length);
                continue;
            }

            case 'b': // %b - bool
            {
                const char* l_result = ((*static_cast<const bool*>(arguments_pointer_p[l_args_index_converted_from_string])) == true) ? "true" : "false";;
                uint64 l_data_bytes_to_copy = std::strlen(l_result);
                std::memcpy(out_buffer_pointer_p, l_result, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (std::strlen("${%b@}") + l_index_digit_length);
                continue;
            }

            case 'c': // %c - char
                *out_buffer_pointer_p = *static_cast<const char*>(arguments_pointer_p[l_args_index_converted_from_string]);
                out_buffer_pointer_p += sizeof(char);
                string_format_p += (std::strlen("${%c@}") + l_index_digit_length);
                continue;

            case 's': // %s - string
            {
                uint64 l_data_bytes_to_copy = std::strlen(static_cast<const char*>(arguments_pointer_p[l_args_index_converted_from_string]));
                std::memcpy(out_buffer_pointer_p, static_cast<const char*>(arguments_pointer_p[l_args_index_converted_from_string]), l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (std::strlen("${%s@}") + l_index_digit_length);
                continue;
            }

            case 'p': // %p - hexadecimal 64bit pointer
            {
                std::snprintf(out_buffer_pointer_p, buffer_size_p, "%p", arguments_pointer_p[l_args_index_converted_from_string]);
                out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
                string_format_p += (std::strlen("${%p@}") + l_index_digit_length);
                continue;
            }

            default: _FE_UNLIKELY_
                std::cerr << "ERROR: an incorrect type format option detected!";
                std::exit(error_code_cast(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
                return;
            }
        }
        else
        {
            *out_buffer_pointer_p = *string_format_p;

            ++out_buffer_pointer_p;
            ++string_format_p;
        }
    }
}

END_NAMESPACE;