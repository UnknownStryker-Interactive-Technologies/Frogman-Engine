#ifndef _FE_CORE_ALGORITHM_UTILITY_HXX_
#define _FE_CORE_ALGORITHM_UTILITY_HXX_
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
#include <FE/prerequisites.h>
#include <FE/type_traits.hxx>
#include <FE/pair.hxx>

// std
#include <bitset>

// std::less{}
#include <functional>

// for strlen()
#include <cstring>

#ifdef __FE_CHAR_TO_INT
#error __FE_CHAR_TO_INT is a reserved Frogman Engine macro keyword.
#endif
#define __FE_CHAR_TO_INT(c) (c - '0')




BEGIN_NAMESPACE(FE::algorithm::utility)


template<class Comparator, typename T>
_FE_FORCE_INLINE_ T& select(const T& true_p, const T& false_p) noexcept
{
    return Comparator()(true_p, false_p) ? true_p : false_p;
}


template<class Iterator, typename T, class LessThan = std::less<T>, class EqualTo = std::equal_to<T>>
Iterator binary_search(Iterator begin_p, Iterator end_p, const T& value_p, LessThan&& less_than_p = LessThan(), EqualTo&& equal_to_p = EqualTo())
{
	FE_NEGATIVE_STATIC_ASSERT((std::is_same<LessThan, std::greater<T>>::value == true), "The comparator must contain < rather than other operators.");
	FE_NEGATIVE_ASSERT(begin_p >= end_p, "Assertion failure: the 'first' iterator is pointing after the 'last' iterator.");
    Iterator l_mid = begin_p + ((end_p - begin_p) >> 1);

#ifdef _ENABLE_ASSERT_
    var::uint64 l_loop_counter = 0;
	FE::uint64 l_max_loop = static_cast<FE::uint64>(std::log2(end_p - begin_p)) + 1;
#endif
    while ((begin_p <= l_mid) && (l_mid <= end_p))
    {
        FE_ASSERT(l_loop_counter <= l_max_loop, "FE::algorithm::utility::binary_search() operation timed out! Please check if the comparator is '<' operator.");
		if (equal_to_p(*l_mid, value_p)) // A == B
        {
			return l_mid;
		}
        else if (less_than_p(*l_mid, value_p)) // A < B
        {
            l_mid += ((end_p - l_mid) >> 1);
        }
        else if (less_than_p(value_p, *l_mid)) // A < B
        {
            l_mid -= ((l_mid - begin_p) >> 1);
        }
#ifdef _ENABLE_ASSERT_
        ++l_loop_counter;
#endif
    }

    return Iterator{ nullptr };
}


enum struct ExclusionSortMode : var::uint8
{
	_PushToRight = 0,
    _PushToLeft = 1
};

/* 
- Time complexity -
Best: O(n/2)
Worst: O(n)
*/
template<ExclusionSortMode ExclusionSortMode, class Iterator> 
FE::pair<Iterator, Iterator> exclusion_sort(Iterator begin_p, Iterator end_p, const auto& exclusion_target_p)
{
    Iterator l_begin = begin_p;
    Iterator l_end = end_p;
    --end_p;
    FE_NEGATIVE_ASSERT(begin_p >= end_p, "Assertion failure: the 'begin' iterator is pointing after the 'end' iterator.");


    if constexpr (ExclusionSortMode == ExclusionSortMode::_PushToRight)
	{
        while (begin_p < end_p)
        {
            if ((*begin_p == exclusion_target_p) && (*end_p != exclusion_target_p))
            {
                std::swap(*begin_p, *end_p);
            }

            if (*begin_p != exclusion_target_p)
            {
                ++begin_p;
            }

            if (*end_p == exclusion_target_p)
            {
                --end_p;
            }
        }
        if (l_begin == end_p)
        {
			++end_p;
        }
        FE_NEGATIVE_ASSERT(l_begin >= end_p, "Assertion failure: the begin iterator is pointing after the end iterator.");
        return FE::pair<Iterator, Iterator>{l_begin, end_p};
	}
	else if constexpr (ExclusionSortMode == ExclusionSortMode::_PushToLeft)
	{
        while (begin_p < end_p)
        {
            if ((*begin_p != exclusion_target_p) && (*end_p == exclusion_target_p))
            {
                std::swap(*begin_p, *end_p);
            }

            if (*begin_p == exclusion_target_p)
            {
                ++begin_p;
            }

            if (*end_p != exclusion_target_p)
            {
                --end_p;
            }
        }
        if (l_begin == end_p)
        {
            ++end_p;
        }
        FE_NEGATIVE_ASSERT(begin_p >= l_end, "Assertion failure: the begin iterator is pointing after the end iterator.");
        return FE::pair<Iterator, Iterator>{begin_p, l_end};
	}
}


_FE_MAYBE_UNUSED_ constexpr int8 ASCII_code_zero = 48;
_FE_MAYBE_UNUSED_ constexpr int8 ASCII_code_nine = 57;


struct int_info
{
    var::int64 _value = 0;
    var::uint8 _digit_length = 0;
};

struct uint_info
{
    var::uint64 _value = 0;
    var::uint8 _digit_length = 0;
};

struct real_info
{
    var::float64 _value = 0;
    var::uint8 _total_length = 0;
    var::uint8 _int_digit_length = 0;
    var::uint8 _floating_point_length = 0;
};


_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ var::uint8 count_int_digit_length(var::int64 value_p) noexcept
{
    var::uint8 l_length_of_n = 0;
    if (value_p < 0)
    {
        for (; value_p <= -10; value_p /= 10) { ++l_length_of_n; }
        return ++l_length_of_n;
    }

    for (; value_p >= 10; value_p /= 10) { ++l_length_of_n; }
    return ++l_length_of_n;
}

_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ var::uint8 count_uint_digit_length(var::uint64 value_p) noexcept
{
    var::uint8 l_length_of_n = 0;

    for (; value_p >= 10; value_p /= 10) { ++l_length_of_n; }
    return ++l_length_of_n;
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ uint_info string_to_uint(const CharT* const integral_string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a character type.");

    const CharT* l_integral_string_pointer = integral_string_p;
    var::uint64 l_result = 0;

    while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
    {
        l_result *= 10;
        l_result += static_cast<var::uint64>(__FE_CHAR_TO_INT(*l_integral_string_pointer) );
        ++l_integral_string_pointer;
    }

    return uint_info{l_result, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
}

template<typename CharT>
_FE_CONSTEXPR20_ int_info string_to_int(const CharT* integral_string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a character type.");

    const CharT* l_integral_string_pointer = integral_string_p;
    var::int64 l_result = 0;

    if (*l_integral_string_pointer == '-')
    {
        ++l_integral_string_pointer;
        ++integral_string_p;

        while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
        {
            l_result *= 10;
            l_result += static_cast<FE::int64>(__FE_CHAR_TO_INT(*l_integral_string_pointer));
            ++l_integral_string_pointer;
        }

        return int_info{l_result * -1, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
    }


    while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
    {
        l_result *= 10;
        l_result += static_cast<FE::int64>(__FE_CHAR_TO_INT(*l_integral_string_pointer));
        ++l_integral_string_pointer;
    }

    return int_info{l_result, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void int_to_string(CharT* const string_out_p, _FE_MAYBE_UNUSED_ length_t input_string_capacity_p, var::int64 value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");
    FE_NEGATIVE_ASSERT(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");
    FE_NEGATIVE_ASSERT(value_p == FE::min_value<var::int64>, "NaCN ERROR: value_p is not a calculatable number");

    var::uint8 l_integral_digits = count_int_digit_length(value_p);

    if (value_p < 0)
    {
        value_p *= -1;
        *string_out_p = static_cast<CharT>('-');
        ++l_integral_digits;
    }

    FE_NEGATIVE_ASSERT(input_string_capacity_p <= l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::uint8 l_idx = l_integral_digits - 1;
    while (value_p > 0)
    {
        string_out_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }

    string_out_p[l_integral_digits] = null;
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void uint_to_string(CharT* const string_out_p, _FE_MAYBE_UNUSED_ length_t input_string_capacity_p, var::uint64 value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type of value_p assigned to the template argument CharT");
    FE_NEGATIVE_ASSERT(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");

    var::uint8 l_integral_digits = count_uint_digit_length(value_p);

    FE_NEGATIVE_ASSERT(input_string_capacity_p <= l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::uint8 l_idx = l_integral_digits - 1;
    while (value_p > 0)
    {
        string_out_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }

    string_out_p[l_integral_digits] = null;
}


template<typename CharT>
_FE_CONSTEXPR20_ real_info string_to_float(const CharT* float_string_p) noexcept
{
    int_info l_integral_part_info = string_to_int(float_string_p);

    if (*float_string_p == '-')
    {
        ++float_string_p;
        float_string_p += (l_integral_part_info._digit_length + 1);
        uint_info l_real_part_info = string_to_uint(float_string_p);

        var::float64 l_integral_part = static_cast<var::float64>(l_integral_part_info._value);
        var::float64 l_real_part = static_cast<var::float64>(l_real_part_info._value);

        for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
        {
            l_real_part /= 10.0f;
        }
        return real_info{ l_integral_part - l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length };
    }


    float_string_p += (1llu + static_cast<uint64>(l_integral_part_info._digit_length));
    uint_info l_real_part_info = string_to_uint(float_string_p);

    var::float64 l_integral_part = static_cast<var::float64>(l_integral_part_info._value);
    var::float64 l_real_part = static_cast<var::float64>(l_real_part_info._value);

    for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
    {
        l_real_part /= 10.0f;
    }
    return real_info{ l_integral_part + l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length };
}

template<typename CharT>
_FE_CONSTEXPR20_ void float_to_string(CharT* const string_out_p, length_t input_string_capacity_p, float64 value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    FE_NEGATIVE_ASSERT(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");

    int_to_string<CharT>(string_out_p, input_string_capacity_p, static_cast<var::int64>(value_p));

    var::uint64 l_integral_part_string_length = internal::strlen<CharT>(string_out_p);
    string_out_p[l_integral_part_string_length] = '.';
    ++l_integral_part_string_length;

    var::float64 l_floating_point = value_p - static_cast<float64>(static_cast<var::int64>(value_p));
    while (0.0 != (l_floating_point - static_cast<var::float64>(static_cast<var::int64>(l_floating_point))))
    {
        l_floating_point *= 10.0;
    }

    FE_NEGATIVE_ASSERT(input_string_capacity_p <= (count_int_digit_length(static_cast<var::int64>(l_floating_point)) + l_integral_part_string_length), "MEMORY BOUNDRY CHECK FAILURE: the digit length of the integral part exceeds the output string buffer capacity");

    int_to_string<CharT>(string_out_p + l_integral_part_string_length, input_string_capacity_p, static_cast<var::int64>(l_floating_point));
}

#pragma warning (disable: 4702)
template<typename CharT>
_FE_CONSTEXPR20_ FE::boolean string_to_boolean(const CharT* const string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    if (((*string_p == 't') && (string_p[1] == 'r')) && ((string_p[2] == 'u') && (string_p[3] == 'e')))
    {
        return true;
    }
    else if (((*string_p == 'f') && (string_p[1] == 'a')) && ((string_p[2] == 'l') && (string_p[3] == 's')) && string_p[4] == 'e')
    {
        return false;
    }

    std::exit(FE::error_code_cast(FE::ErrorCode::_FATAL_INPUT_ERROR_2XX_INVALID_ARGUMENT));
    return false;
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const CharT* boolean_to_string(boolean value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    return (value_p == true) ? static_cast<const CharT*>("true") : static_cast <const CharT*>("false");
}

END_NAMESPACE
#undef FE_CHAR_TO_INT
#endif