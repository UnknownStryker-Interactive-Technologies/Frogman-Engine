#ifndef _FE_CORE_ALGORITHM_UTILITY_HXX_
#define _FE_CORE_ALGORITHM_UTILITY_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/algorithm/math.hxx>
#include <FE/iterator.hxx>
#include <FE/pair.hxx>
#include <FE/type_traits.hxx>


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


template <typename T>
using value_type_of = std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<T>, typename T::value_type>;

template <typename T>
using pointer_of = std::conditional_t<std::is_pointer_v<T>, T, typename T::pointer>;


enum struct IsolationVector : var::uint8
{
	_Right = 0,
    _Left = 1
};
/*
    - Time complexity -
    O(n)
    The partition_stable function template sorts a range of elements defined by two iterators based on a specified predicate
    stable partitioning of elements based on exclusion_target_p.
    Groups all elements NOT equal to exclusion_target_p at one end (based on IsolationVector).
    Preserves relative order of retained elements.
    Useful when element order matters (e.g., rendering, logic sequencing).
*/
template<IsolationVector IsolationVector, class Iterator, class Predicate>
constexpr FE::pair<Iterator, Iterator> partition_stable_if(Iterator begin_p, Iterator end_p, Predicate predicate_p)
{
    if constexpr (IsolationVector == IsolationVector::_Right)
    {
        Iterator l_end = end_p;
        Iterator l_begin = begin_p;
        Iterator l_tmp_it = begin_p;

        while (l_tmp_it != l_end)
        {
            if (predicate_p(*l_begin))
            {
                if (!predicate_p(*l_tmp_it))
                {
                    std::swap(*l_begin, *l_tmp_it);
                    ++l_begin;
                    ++l_tmp_it;
                    continue;
                }

                ++l_tmp_it;
                continue;
            }

            ++l_begin;
            ++l_tmp_it;
        }
        return FE::pair<Iterator, Iterator>{begin_p, l_tmp_it};
    }
    else if constexpr (IsolationVector == IsolationVector::_Left)
    {
        Iterator l_rend = begin_p;
        Iterator l_rbegin = std::next(begin_p, ((end_p - begin_p) - 1));
        Iterator l_rtmp_it = l_rbegin;

        while (l_rtmp_it > l_rend)
        {
            if (predicate_p(*l_rbegin))
            {
                if (!predicate_p(*l_rtmp_it))
                {
                    std::swap(*l_rbegin, *l_rtmp_it);
                    --l_rbegin;
                    --l_rtmp_it;
                    continue;
                }

                --l_rtmp_it;
                continue;
            }

            --l_rbegin;
            --l_rtmp_it;
        }
        std::swap(*l_rbegin, *l_rtmp_it);
        if (predicate_p(*l_rbegin))
        {
            ++l_rbegin;
        }
        return FE::pair<Iterator, Iterator>{l_rbegin, end_p};
    }
}
/*
    - Time complexity -
    O(n)

    partition_stable: Stable partitioning of elements based on exclusion_target_p.
    Groups all elements NOT equal to exclusion_target_p at one end (based on IsolationVector).
    Preserves relative order of retained elements.
    Useful when element order matters (e.g., rendering, logic sequencing).
*/
template<IsolationVector IsolationVector, class Iterator>
constexpr FE::pair<Iterator, Iterator> partition_stable(Iterator begin_p, Iterator end_p, const auto& exclusion_target_p)
{
    return partition_stable_if<IsolationVector>(begin_p, end_p, [&](const auto& value_p) { return value_p == exclusion_target_p; });
   // if constexpr (IsolationVector == IsolationVector::_Right)
   // {
   //     Iterator l_end = end_p;
   //     Iterator l_begin = begin_p;
   //     Iterator l_tmp_it = begin_p;

   //     while (l_tmp_it != l_end)
   //     {
   //         if (*l_begin == exclusion_target_p)
   //         {
   //             if (*l_tmp_it != exclusion_target_p)
   //             {
   //                 std::swap(*l_begin, *l_tmp_it);
   //                 ++l_begin;
   //                 ++l_tmp_it;
   //                 continue;
   //             }

   //             ++l_tmp_it;
   //             continue;
   //         }

   //         ++l_begin;
   //         ++l_tmp_it;
   //     }
   //     return FE::pair<Iterator, Iterator>{begin_p, l_tmp_it};
   // }
   // else if constexpr (IsolationVector == IsolationVector::_Left)
   // {
   //     Iterator l_rend = begin_p;
   //     Iterator l_rbegin = std::next(begin_p, ((end_p - begin_p) - 1));
   //     Iterator l_rtmp_it = l_rbegin;

   //     while (l_rtmp_it > l_rend)
   //     {
   //         if (*l_rbegin == exclusion_target_p)
   //         {
   //             if (*l_rtmp_it != exclusion_target_p)
   //             {
   //                 std::swap(*l_rbegin, *l_rtmp_it);
   //                 --l_rbegin;
   //                 --l_rtmp_it;
   //                 continue;
   //             }

   //             --l_rtmp_it;
   //             continue;
   //         }

   //         --l_rbegin;
   //         --l_rtmp_it;
   //     }
   //     std::swap(*l_rbegin, *l_rtmp_it);
   //     if (*l_rbegin == exclusion_target_p)
   //     {
			//++l_rbegin;
   //     }
   //     return FE::pair<Iterator, Iterator>{l_rbegin, end_p};
   // }
}

/* 
    - Time complexity -
    Best: O(n/2)
    Worst: O(n)

    The partition_unstable function template sorts a range of elements defined by two iterators based on a specified predicate
    partitioning the elements into two groups according to the provided IsolationVector.
    Unstable in-place partitioning based on exclusion_target_p.
    Swaps elements from both ends to isolate non-excluded elements.
    Does NOT preserve relative order.
    Optimized for performance in large datasets or non-order-sensitive contexts.
*/
template<IsolationVector IsolationVector, class Iterator, class Predicate> 
constexpr FE::pair<Iterator, Iterator> partition_unstable_if(Iterator begin_p, Iterator end_p, Predicate predicate_p)
{
    Iterator l_begin = begin_p;
    Iterator l_end = end_p;
    --end_p;
    FE_NEGATIVE_ASSERT(begin_p >= end_p, "Assertion failure: the 'begin' iterator is pointing after the 'end' iterator.");


    if constexpr (IsolationVector == IsolationVector::_Right)
    {
        while (begin_p < end_p)
        {
            if (predicate_p(*begin_p) && !predicate_p(*end_p))
            {
                std::swap(*begin_p, *end_p);
            }

            if (!predicate_p(*begin_p))
            {
                ++begin_p;
            }

            if (predicate_p(*end_p))
            {
                --end_p;
            }
        }
        if (begin_p >= end_p)
        {
            end_p = begin_p;
        }
        FE_NEGATIVE_ASSERT(l_begin > end_p, "Assertion failure: the begin iterator is pointing after the end iterator.");
        return FE::pair<Iterator, Iterator>{l_begin, end_p};
    }
    else if constexpr (IsolationVector == IsolationVector::_Left)
    {
        while (begin_p <= end_p)
        {
            if (!predicate_p(*begin_p) && predicate_p(*end_p))
            {
                std::swap(*begin_p, *end_p);
            }

            if (!predicate_p(*end_p))
            {
                --end_p;
            }

            if (predicate_p(*begin_p))
            {
                ++begin_p;
            }
        }
        FE_NEGATIVE_ASSERT(begin_p > l_end, "Assertion failure: the begin iterator is pointing after the end iterator.");
        return FE::pair<Iterator, Iterator>{begin_p, l_end};
	}
}
/*
    - Time complexity -
    Best: O(n/2)
    Worst: O(n)

    The partition_unstable function template sorts a range of elements defined by two iterators
    moving elements equal to a specified exclusion target to one end of the range based on the specified isolation vector direction (either left or right).
    Unstable in-place partitioning based on exclusion_target_p.
    Swaps elements from both ends to isolate non-excluded elements.
    Does NOT preserve relative order.
    Optimized for performance in large datasets or non-order-sensitive contexts.
*/
template<IsolationVector IsolationVector, class Iterator>
constexpr FE::pair<Iterator, Iterator> partition_unstable(Iterator begin_p, Iterator end_p, const auto& exclusion_target_p)
{
    return partition_unstable_if<IsolationVector>(begin_p, end_p, [&](const auto& value_p) { return value_p == exclusion_target_p; });
    //Iterator l_begin = begin_p;
    //_FE_MAYBE_UNUSED_ Iterator l_end = end_p;
    //--end_p;
    //FE_NEGATIVE_ASSERT(begin_p >= end_p, "Assertion failure: the 'begin' iterator is pointing after the 'end' iterator.");


    //if constexpr (IsolationVector == IsolationVector::_Right)
    //{
    //    while (begin_p < end_p)
    //    {
    //        if ((*begin_p == exclusion_target_p) && (*end_p != exclusion_target_p))
    //        {
    //            std::swap(*begin_p, *end_p);
    //        }

    //        if (*begin_p != exclusion_target_p)
    //        {
    //            ++begin_p;
    //        }

    //        if (*end_p == exclusion_target_p)
    //        {
    //            --end_p;
    //        }
    //    }
    //    if (begin_p >= end_p)
    //    {
    //        end_p = begin_p;
    //    }
    //    FE_NEGATIVE_ASSERT(l_begin > end_p, "Assertion failure: the begin iterator is pointing after the end iterator.");
    //    return FE::pair<Iterator, Iterator>{l_begin, end_p};
    //}
    //else if constexpr (IsolationVector == IsolationVector::_Left)
    //{
    //    while (begin_p <= end_p)
    //    {
    //        if ((*begin_p != exclusion_target_p) && (*end_p == exclusion_target_p))
    //        {
    //            std::swap(*begin_p, *end_p);
    //        }

    //        if (*end_p != exclusion_target_p)
    //        {
    //            --end_p;
    //        }

    //        if (*begin_p == exclusion_target_p)
    //        {
    //            ++begin_p;
    //        }
    //    }
    //    FE_NEGATIVE_ASSERT(begin_p > l_end, "Assertion failure: the begin iterator is pointing after the end iterator.");
    //    return FE::pair<Iterator, Iterator>{begin_p, l_end};
    //}
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


constexpr var::uint8 count_int_digit_length(var::int64 value_p) noexcept
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

_FE_FORCE_INLINE_ constexpr var::uint8 count_uint_digit_length(var::uint64 value_p) noexcept
{
    var::uint8 l_length_of_n = 0;

    for (; value_p >= 10; value_p /= 10) { ++l_length_of_n; }
    return ++l_length_of_n;
}

template<typename CharT>
constexpr uint_info string_to_uint(const CharT* const integral_string_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "static assertion failed: the template argument CharT is not a character type.");

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
constexpr int_info string_to_int(const CharT* integral_string_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "static assertion failed: the template argument CharT is not a character type.");

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
constexpr void int_to_string(CharT* const out_string_p, _FE_MAYBE_UNUSED_ uint64 str_buff_len_p, var::int64 value_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "an illegal type assigned to the template argument CharT");
    FE_NEGATIVE_ASSERT(out_string_p == nullptr, "NULLPTR DETECTED: out_string_p is nullptr.");
    FE_NEGATIVE_ASSERT(value_p == FE::min_value<var::int64>, "NaCN ERROR: value_p is not a calculatable number");

    var::uint8 l_integral_digits = algorithm::utility::count_int_digit_length(value_p);

    if (value_p < 0)
    {
        value_p *= -1;
        *out_string_p = static_cast<CharT>('-');
        ++l_integral_digits;
    }

    FE_NEGATIVE_ASSERT(str_buff_len_p < l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::uint8 l_idx = l_integral_digits - 1;

    if (value_p == 0) _FE_UNLIKELY_
    {
        out_string_p[0] = (CharT)'0';
        out_string_p[l_integral_digits] = null;
        return;
    }

    while (value_p > 0)
    {
        out_string_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }

    out_string_p[l_integral_digits] = null;
}

template<typename CharT>
constexpr void uint_to_string(CharT* const out_string_p, _FE_MAYBE_UNUSED_ uint64 input_string_capacity_p, var::uint64 value_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "an illegal type of value_p assigned to the template argument CharT");
    FE_NEGATIVE_ASSERT(out_string_p == nullptr, "NULLPTR DETECTED: out_string_p is nullptr.");

    var::uint8 l_integral_digits = algorithm::utility::count_uint_digit_length(value_p);

    FE_NEGATIVE_ASSERT(input_string_capacity_p < l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::uint8 l_idx = l_integral_digits - 1;

    if (value_p == 0) _FE_UNLIKELY_
    {
        out_string_p[0] = (CharT)'0';
        out_string_p[l_integral_digits] = null;
        return;
    }

    while (value_p > 0)
    {
        out_string_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }

    out_string_p[l_integral_digits] = null;
}


template<typename CharT>
constexpr real_info string_to_float(const CharT* float_string_p) noexcept
{
    int_info l_integral_part_info = string_to_int(float_string_p);

    if (*float_string_p == '-')
    {
        ++float_string_p;
        float_string_p += (l_integral_part_info._digit_length + 1);
        uint_info l_real_part_info = algorithm::utility::string_to_uint(float_string_p);

        var::float64 l_integral_part = static_cast<var::float64>(l_integral_part_info._value);
        var::float64 l_real_part = static_cast<var::float64>(l_real_part_info._value);

        for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
        {
            l_real_part /= 10.0f;
        }
        return real_info{ l_integral_part - l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length };
    }


    float_string_p += (1llu + static_cast<uint64>(l_integral_part_info._digit_length));
    uint_info l_real_part_info = algorithm::utility::string_to_uint(float_string_p);

    var::float64 l_integral_part = static_cast<var::float64>(l_integral_part_info._value);
    var::float64 l_real_part = static_cast<var::float64>(l_real_part_info._value);

    for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
    {
        l_real_part /= 10.0f;
    }
    return real_info{ l_integral_part + l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length };
}

template<typename CharT>
constexpr void float_to_string(CharT* const string_out_p, uint64 input_string_capacity_p, float64 value_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "an illegal type assigned to the template argument CharT");

    FE_NEGATIVE_ASSERT(string_out_p == nullptr, "NULLPTR DETECTED: out_string_p is nullptr.");

    algorithm::utility::int_to_string<CharT>(string_out_p, input_string_capacity_p, static_cast<var::int64>(value_p));

    var::uint64 l_integral_part_string_length = FE::internal::strlen<CharT>(string_out_p);
    string_out_p[l_integral_part_string_length] = '.';
    ++l_integral_part_string_length;

    var::float64 l_floating_point = value_p - static_cast<float64>(static_cast<var::int64>(value_p));
    while (0.0 != (l_floating_point - static_cast<var::float64>(static_cast<var::int64>(l_floating_point))))
    {
        l_floating_point *= 10.0;
    }

    FE_NEGATIVE_ASSERT(input_string_capacity_p <= (count_int_digit_length(static_cast<var::int64>(l_floating_point)) + l_integral_part_string_length), "MEMORY BOUNDRY CHECK FAILURE: the digit length of the integral part exceeds the output string buffer capacity");

    algorithm::utility::int_to_string<CharT>(string_out_p + l_integral_part_string_length, input_string_capacity_p, static_cast<var::int64>(l_floating_point));
}


template<typename CharT>
constexpr FE::boolean string_to_boolean(const CharT* const string_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "an illegal type assigned to the template argument CharT");

    char l_buffer[] = "true";
    var::int32* const l_lhs = (var::int32* const)l_buffer;
    var::int32* const l_rhs = (var::int32* const)string_p;
    return ((*l_lhs) xor (*l_rhs)) == 0;
}

template<typename CharT>
_FE_FORCE_INLINE_ constexpr const CharT* boolean_to_string(boolean value_p) noexcept
{
    static_assert(FE::is_char<CharT>::value, "an illegal type assigned to the template argument CharT");

    return (value_p == true) ? static_cast<const CharT*>("true") : static_cast<const CharT*>("false");
}


END_NAMESPACE
#undef FE_CHAR_TO_INT
#endif