#include "FE/core/log/format_string.h"
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/memory_metrics.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#define _OPEN_CURLY_BRACKET_ 1
#define _FORMAT_SPECIFIER_PERCENT_ 2
#define _FORMAT_SPECIFIER_VALUE_PREFIX_ 3
#define _FORMAT_SPECIFIER_VALUE_SUFFIX_ 4




BEGIN_NAMESPACE(FE::log)


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
void format_string(char* out_buffer_pointer_p, const char* string_format_p, _MAYBE_UNUSED_ size_t buffer_size_p, const void** arguments_pointer_p, _MAYBE_UNUSED_ count_t arguments_count_p) noexcept
{
    FE_ABORT_IF(FE::internal::strlen(string_format_p) > buffer_size_p, "ERROR: buffer overflowed!");
    thread_local static char tl_s_buffer[_16_BYTES_BIT_COUNT_] = { "\0" };

    while (*string_format_p != '\0')
    {
        if ((*string_format_p == '$') && (string_format_p[_OPEN_CURLY_BRACKET_] == '{') && (string_format_p[_FORMAT_SPECIFIER_PERCENT_] == '%'))
        {
            std::memset(tl_s_buffer, _NULL_, FE::internal::strlen(tl_s_buffer));
            algorithm::utility::integral_info<var::uint32> l_index;

            if (string_format_p[4] == '@')
            {
                constexpr static index_t _JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_ = 5;
                l_index = algorithm::utility::string_to_uint<var::uint32>(string_format_p + _JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_);
                FE_ABORT_IF(string_format_p[_JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_ + l_index._digit_length] != '}', "ERROR: an illegal string format detected! } is missing.");
            }
            else if (string_format_p[5] == '@')
            {
                constexpr static index_t _JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_ = 6;
                l_index = algorithm::utility::string_to_uint<var::uint32>(string_format_p + _JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_);
                FE_ABORT_IF(string_format_p[_JUMP_TO_FORMAT_SPECIFIER_VALUE_PREFIX_ + l_index._digit_length] != '}', "ERROR: an illegal string format detected! } is missing.");
            }
            else
            {
                continue;
            }
         

            FE_ABORT_IF(l_index._value >= arguments_count_p, "ERROR: Detected an invalid index token. The @n value is too large.");


            switch (string_format_p[_FORMAT_SPECIFIER_VALUE_PREFIX_])
            {
            case 'd': // %d - int32
            {
                algorithm::utility::int_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<int32*>(arguments_pointer_p[l_index._value]));
                uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;
            }

            case 'u': // %u - uint32
            {
                algorithm::utility::uint_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<uint32*>(arguments_pointer_p[l_index._value]));
                uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;
            }

            case 'l':
                switch (string_format_p[_FORMAT_SPECIFIER_VALUE_SUFFIX_])
                {
                case 'd': // %ld - int64
                {
                    algorithm::utility::int_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<int64*>(arguments_pointer_p[l_index._value]));
                    uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                    std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                    out_buffer_pointer_p += l_data_bytes_to_copy;
                    string_format_p += (7llu + (uint64)l_index._digit_length);
                    continue;
                }

                case 'u': // %lu - uint64
                {
                    algorithm::utility::uint_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<uint64*>(arguments_pointer_p[l_index._value]));
                    uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                    std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                    out_buffer_pointer_p += l_data_bytes_to_copy;
                    string_format_p += (7llu + (uint64)l_index._digit_length);
                    continue;
                }

                case 'f': // %lf - float64
                {
                    algorithm::utility::float_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<float64*>(arguments_pointer_p[l_index._value]));
                    uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                    std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                    out_buffer_pointer_p += l_data_bytes_to_copy;
                    string_format_p += (7llu + (uint64)l_index._digit_length);
                    continue;
                }

                default:
                    FE_ABORT_IF(true, "ERROR: an incorrect type format option detected! The option must be %ld (int64), %lu (uint64), or %lf (float64).");
                    return;
                }

            case 'f': // %f - float32
            {
                algorithm::utility::float_to_string(tl_s_buffer, _16_BYTES_BIT_COUNT_, *static_cast<float32*>(arguments_pointer_p[l_index._value]));
                uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;
            }

            case 'b': // %b - bool
            {
                const char* l_result = algorithm::utility::boolean_to_string<char>(*static_cast<boolean*>(arguments_pointer_p[l_index._value]));
                uint64 l_data_bytes_to_copy = FE::internal::strlen(l_result);
                std::memcpy(out_buffer_pointer_p, l_result, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;
            }

            case 'c': // %c - char
                *out_buffer_pointer_p = *static_cast<const char*>(arguments_pointer_p[l_index._value]);
                out_buffer_pointer_p += sizeof(char);
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;

            case 's': // %s - string
            {
                uint64 l_data_bytes_to_copy = FE::internal::strlen(static_cast<const char*>(arguments_pointer_p[l_index._value]));
                std::memcpy(out_buffer_pointer_p, static_cast<const char*>(arguments_pointer_p[l_index._value]), l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
                continue;
            }

            case 'p': // %p - hexadecimal 64bit pointer
            {
                std::snprintf(tl_s_buffer, _16_BYTES_BIT_COUNT_ * sizeof(char), "%p\0", arguments_pointer_p[l_index._value]);
                uint64 l_data_bytes_to_copy = FE::internal::strlen(tl_s_buffer);
                std::memcpy(out_buffer_pointer_p, tl_s_buffer, l_data_bytes_to_copy);
                out_buffer_pointer_p += l_data_bytes_to_copy;
                string_format_p += (6llu + (uint64)l_index._digit_length);
            }
            break;

            default: _UNLIKELY_
                FE_ABORT_IF(true, "ERROR: an incorrect type format option detected!");
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
const char* buffered_string_formatter(std::initializer_list<const void*> arguments_p) noexcept
{
    constexpr static auto _SKIP_STRING_FORMAT_ = 1;

    thread_local static char tl_s_buffer[4 KB] = { "\0" };
    std::memset(tl_s_buffer, _NULL_, internal::strlen(tl_s_buffer));

    format_string(tl_s_buffer, static_cast<const char*>(*arguments_p.begin()), 4 KB, const_cast<const void**>(arguments_p.begin()) + _SKIP_STRING_FORMAT_, arguments_p.size());
    return tl_s_buffer;
}


END_NAMESPACE;