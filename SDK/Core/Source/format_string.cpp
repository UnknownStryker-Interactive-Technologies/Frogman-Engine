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
#include "FE/log/format_string.hxx"
#include <FE/algorithm/utility.hxx>
#include <FE/memory.hxx>

// std::snprintf
#include <cstdio>

// std::memcpy
#include <cstring>

// std::atoll
#include <cstdlib>

// std::cerr
#include <iostream>

#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/hash_append.hpp>




BEGIN_NAMESPACE(FE::log)


constexpr FE::uint32 __hash_string(const char* string_p, FE::uint32 length_p)
{
	boost::hash2::fnv1a_32 l_fnv1a;
	boost::hash2::hash_append_range(l_fnv1a, {}, string_p, string_p + length_p);
	//l_fnv1a.update(string_p, length_p);
	return l_fnv1a.result();
}

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
*/

enum struct FormatSpecifier : FE::uint32
{
    _Int8,
	_UInt8,
	_Int16,
	_UInt16,
	_Int32,
	_UInt32,
	_Int64,
	_UInt64,
	_Float32,
	_Float64,
	_Boolean,
	_Char,
	_String,
	_Pointer,
	_Invalid
};

FormatSpecifier __identify_format_specifier(const char* const string_format_p) noexcept
{
	const char* l_iterator = string_format_p;

	while (*l_iterator != FE::null)
	{
		if (*l_iterator == '@')
		{
			break;
		}
		++l_iterator;
	}

	FE::uint32 l_format_specifier_length = static_cast<FE::uint32>(l_iterator - string_format_p);

    switch (__hash_string(string_format_p, l_format_specifier_length))
    {
    case __hash_string("i8", 2):
		return FormatSpecifier::_Int8;

	case __hash_string("u8", 2):
		return FormatSpecifier::_UInt8;

	case __hash_string("i16", 3):
		return FormatSpecifier::_Int16;

	case __hash_string("u16", 3):
		return FormatSpecifier::_UInt16;

	case __hash_string("d", 1):
        _FE_FALLTHROUGH_;

	case __hash_string("i32", 3):
		return FormatSpecifier::_Int32;

	case __hash_string("u", 1):
		_FE_FALLTHROUGH_;

	case __hash_string("u32", 3):
		return FormatSpecifier::_UInt32;

	case __hash_string("ld", 2):
		_FE_FALLTHROUGH_;

	case __hash_string("i64", 3):
		return FormatSpecifier::_Int64;

	case __hash_string("lu", 2):
		_FE_FALLTHROUGH_;

	case __hash_string("u64", 3):
		return FormatSpecifier::_UInt64;

	case __hash_string("f", 1):
		_FE_FALLTHROUGH_;

	case __hash_string("f32", 3):
		return FormatSpecifier::_Float32;

	case __hash_string("lf", 2):
		_FE_FALLTHROUGH_;

	case __hash_string("f64", 3):
		return FormatSpecifier::_Float64;

	case __hash_string("b", 1):
		return FormatSpecifier::_Boolean;

	case __hash_string("c", 1):
		return FormatSpecifier::_Char;

	case __hash_string("s", 1):
		return FormatSpecifier::_String;

	case __hash_string("p", 1):
		return FormatSpecifier::_Pointer;

	default: // Of course, the code has to be unreachable. What r u talking about MSVC?
		std::cerr << "Invalid format specifier detected: the format specifier is not correctly formatted.";
        std::exit((int)FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex);
		return FormatSpecifier::_Invalid;
    }
}

FE::int64 __parse_index(const char* string_p) noexcept
{
    while (*string_p != FE::null)
    {
        if (*string_p == '@')
        {
            return std::atoll(string_p + 1);
        }
		++string_p;
    }
	return -1;
}

const char* __find_bracket_token(const char* const string_format_p) noexcept
{
	constexpr const char* l_bracket_token = "${%";
	const char* l_token_validator = l_bracket_token;
	const char* l_iterator = string_format_p;

    while (*l_iterator != FE::null)
    {
		if (*l_iterator == *l_token_validator)
		{
			++l_token_validator;
            if (*l_token_validator == FE::null)
            {
                return l_iterator - 2;
            }
		}
		else
		{
			l_token_validator = l_bracket_token;
		}
		++l_iterator;
    }
    return l_iterator;
}

const char* __find_end_of_token(const char* const string_format_p) noexcept
{
    const char* l_iterator = string_format_p;
    while (*l_iterator != FE::null)
    {
        if (*l_iterator == '}')
        {
            return l_iterator + 1;
        }
        ++l_iterator;
    }
    return l_iterator;
}

// It is not allowed to use the in-house algorithm functions for the implementation of format_string(). The debug macros within the algorithm functions call format_string() recursively when the error coindition is true.
void format_string(char* out_buffer_pointer_p, const char* string_format_p, size buffer_size_p, const void** arguments_pointer_p, uint64 arguments_count_p) noexcept
{
    assert(out_buffer_pointer_p != nullptr);
	assert(string_format_p != nullptr);
    assert(buffer_size_p > 0);
	assert(arguments_pointer_p != nullptr);
    assert(arguments_count_p >= 0);
    var::int64 l_buffer_size = static_cast<var::int64>(buffer_size_p);

    while (*string_format_p != FE::null)
    {
        const char* l_token = __find_bracket_token(string_format_p);
		size l_message_length = l_token - string_format_p;
		std::memcpy(out_buffer_pointer_p, string_format_p, l_message_length);
		out_buffer_pointer_p += l_message_length;
        l_buffer_size -= l_message_length;
		string_format_p += l_message_length;

		// It could not find the token and reached the end of the string.
        if (*l_token == FE::null)
        {
			*out_buffer_pointer_p = FE::null;
            return;
        }
        else
        {
            string_format_p += 3; // Skip the "${%" token.
        }

        FormatSpecifier l_format_specifier = __identify_format_specifier(string_format_p);
        FE::int64 l_idx = __parse_index(string_format_p);

		if (l_idx == -1)
		{
			std::cerr << "Invalid format specifier detected: the format specifier is not correctly formatted.";
			std::exit((int)FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex);
		}

		if (static_cast<FE::uint64>(l_idx) >= arguments_count_p)
		{
			std::cerr << "The message log format is not correctly written: the index is out of range.";
			std::exit((int)FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex);
		}

        switch (l_format_specifier)
        {
		case FormatSpecifier::_Int8:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%hhi", *static_cast<int8*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
            break;

		case FormatSpecifier::_UInt8:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%hhu", *static_cast<uint8*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;
		
		case FormatSpecifier::_Int16:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%hi", *static_cast<int16*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_UInt16:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%hu", *static_cast<uint16*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Int32:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%d", *static_cast<int32*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_UInt32:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%u", *static_cast<uint32*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Int64:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%lld", *static_cast<int64*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_UInt64:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%llu", *static_cast<uint64*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Float32:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%f", *static_cast<float32*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Float64:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%lf", *static_cast<float64*>(arguments_pointer_p[l_idx]));
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Boolean:
			{
				const char* l_result = ((*static_cast<const bool*>(arguments_pointer_p[l_idx])) == true) ? "true" : "false";
				uint64 l_data_bytes_to_copy = std::strlen(l_result);
				std::memcpy(out_buffer_pointer_p, l_result, l_data_bytes_to_copy);
				out_buffer_pointer_p += l_data_bytes_to_copy;
				string_format_p = __find_end_of_token(string_format_p);
			}
			break;

		case FormatSpecifier::_Char:
			*out_buffer_pointer_p = *static_cast<const char*>(arguments_pointer_p[l_idx]);
			++out_buffer_pointer_p;
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_String:
			{
				uint64 l_data_bytes_to_copy = std::strlen(static_cast<const char*>(arguments_pointer_p[l_idx]));
				std::memcpy(out_buffer_pointer_p, static_cast<const char*>(arguments_pointer_p[l_idx]), l_data_bytes_to_copy);
				out_buffer_pointer_p += l_data_bytes_to_copy;
				string_format_p = __find_end_of_token(string_format_p);
			}

			break;

		case FormatSpecifier::_Pointer:
			std::snprintf(out_buffer_pointer_p, l_buffer_size, "%p", arguments_pointer_p[l_idx]);
			out_buffer_pointer_p += std::strlen(out_buffer_pointer_p);
			string_format_p = __find_end_of_token(string_format_p);
			break;

		case FormatSpecifier::_Invalid:
			_FE_FALLTHROUGH_;

		default:
			std::cerr << "Invalid format specifier detected: the format specifier is not correctly formatted.";
			std::exit((int)FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex);
			break;
        }
    }
	*out_buffer_pointer_p = FE::null;
}

const char* buffered_string_formatter(std::initializer_list<const void*> arguments_p) noexcept
{
	thread_local static char tl_s_buffer[string_formatter_buffer_size];
    std::memset(tl_s_buffer, null, string_formatter_buffer_size);
    if (arguments_p.begin() != nullptr)
    {
        format_string(tl_s_buffer, static_cast<const char*>(*arguments_p.begin()), string_formatter_buffer_size, const_cast<const void**>(arguments_p.begin()+1), arguments_p.size()-1);
    }
    return tl_s_buffer;
}

END_NAMESPACE;