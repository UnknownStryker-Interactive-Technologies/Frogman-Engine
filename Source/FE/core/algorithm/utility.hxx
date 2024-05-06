#ifndef _FE_CORE_ALGORITHM_UTILITY_HXX_
#define _FE_CORE_ALGORITHM_UTILITY_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>
#include <FE/core/private/debug.h>
#include <FE/core/private/strlen.h>
#include <FE/core/type_traits.hxx>
#include <bitset>
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 26479)
#define CHAR_TO_INT(c) (c - '0')




BEGIN_NAMESPACE(FE::algorithm::utility)


template<typename T>
_FORCE_INLINE_ void swap(T& in_out_first_p, T& in_out_second_p) noexcept
{
	T l_temporary = std::move(in_out_first_p);

	in_out_first_p = std::move(in_out_second_p);
	in_out_second_p = std::move(l_temporary);
}

template<typename T>
_FORCE_INLINE_ T exchange(T& in_out_target_p, T value_p) noexcept
{
    T l_temporary = std::move(in_out_target_p);
    in_out_target_p = std::move(value_p);
    return std::move(l_temporary);
}

template<class Comparator, typename T>
_FORCE_INLINE_ T& select(T& for_true_p, T& for_false_p) noexcept
{
    return Comparator()(for_true_p, for_false_p) ? for_true_p : for_false_p;
}


_MAYBE_UNUSED_ constexpr int8 ASCII_code_zero = 48;
_MAYBE_UNUSED_ constexpr int8 ASCII_code_nine = 57;


template<typename IntT>
struct integral_info
{
    FE_STATIC_ASSERT(std::is_integral<IntT>::value == false, "static assertion failed: The template argument IntT is not an integral type.");
    IntT _value = 0;
    var::uint8 _digit_length = 0;
};

template<typename RealType>
struct real_info
{
    FE_STATIC_ASSERT(std::is_floating_point<RealType>::value == false, "static assertion failed: The template argument RealType is not a floating point type.");
    RealType _value = 0;
    var::uint8 _total_length = 0;
    var::uint8 _integral_part_length = 0;
    var::uint8 _real_part_length = 0;
};


template<typename UIntT, typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ integral_info<UIntT> string_to_uint(const CharT* const integral_string_p) noexcept
{
    FE_STATIC_ASSERT(std::is_unsigned<UIntT>::value == false, "static assertion failed: the template argument UIntT is not an unsigned integral type.");
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a character type.");

    const CharT* l_integral_string_pointer = integral_string_p;
    UIntT l_result = 0;

    while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
    {
        l_result *= 10;
        l_result += static_cast<UIntT>(CHAR_TO_INT(*l_integral_string_pointer));
        ++l_integral_string_pointer;
    }

    return integral_info<UIntT>{l_result, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
}


template<typename SIntT, typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ integral_info<SIntT> string_to_int(const CharT* integral_string_p) noexcept
{
    FE_STATIC_ASSERT(std::is_signed<SIntT>::value == false, "static assertion failed: the template argument SIntT is not a signed integral type.");
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a character type.");

    const CharT* l_integral_string_pointer = integral_string_p;
    SIntT l_result = 0;

    if (*l_integral_string_pointer == '-')
    {
        ++l_integral_string_pointer;
        ++integral_string_p;

        while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
        {
            l_result *= 10;
            l_result += static_cast<SIntT>(CHAR_TO_INT(*l_integral_string_pointer));
            ++l_integral_string_pointer;
        }

        return integral_info<SIntT>{l_result * -1, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
    }


    while ((*l_integral_string_pointer >= FE::algorithm::utility::ASCII_code_zero) && (*l_integral_string_pointer <= FE::algorithm::utility::ASCII_code_nine))
    {
        l_result *= 10;
        l_result += static_cast<SIntT>(CHAR_TO_INT(*l_integral_string_pointer));
        ++l_integral_string_pointer;
    }

    return integral_info<SIntT>{l_result, static_cast<var::uint8>(l_integral_string_pointer - integral_string_p)};
}




#pragma warning(push)
#pragma warning(disable:4715)
template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ boolean string_to_boolean(const CharT* const string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    const CharT* l_string_pointer = string_p;

    const CharT* l_s_true_pointer = "true";
    const CharT* l_s_false_pointer = "false";


    switch (*l_string_pointer)
    {
    case 't':
        while ((*l_s_true_pointer == *l_string_pointer) && (*l_s_true_pointer != '\0'))
        {
            ++l_s_true_pointer;
            ++l_string_pointer;
        }
        FE_ABORT_IF((l_string_pointer - string_p) != 4, "ERROR: The calculation cannot be done properly. The input string is not \"true\".");
        return true;

    case 'f':
        while ((*l_s_false_pointer == *l_string_pointer) && (*l_s_false_pointer != '\0'))
        {
            ++l_s_false_pointer;
            ++l_string_pointer;
        }
        FE_ABORT_IF((l_string_pointer - string_p) != 5, "ERROR: The calculation cannot be done properly. The input string is not \"false\".");
        return false;

    default: _UNLIKELY_
        FE_ABORT_IF(true, "ERROR: The calculation cannot be done properly. The input string is neither \"true\" nor \"false\".");
        break;
    }
}
#pragma warning(pop)




template<typename FloatT, typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ real_info<FloatT> string_to_float(const CharT* float_string_p) noexcept
{
    integral_info<var::int64> l_integral_part_info = string_to_int<var::int64>(float_string_p);

    if (*float_string_p == '-')
    {
        ++float_string_p;
        float_string_p += (l_integral_part_info._digit_length + 1);
        integral_info<var::uint64> l_real_part_info = string_to_uint<var::uint64>(float_string_p);

        FloatT l_integral_part = static_cast<FloatT>(l_integral_part_info._value);
        FloatT l_real_part = static_cast<FloatT>(l_real_part_info._value);

        for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
        {
            l_real_part /= 10.0f;
        }
        return real_info<FloatT>{l_integral_part - l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length};
    }
    

    float_string_p += (1llu + (uint64)l_integral_part_info._digit_length);
    integral_info<var::uint64> l_real_part_info = string_to_uint<var::uint64>(float_string_p);

    FloatT l_integral_part = static_cast<FloatT>(l_integral_part_info._value);
    FloatT l_real_part = static_cast<FloatT>(l_real_part_info._value);

    for (var::int32 i = 0; i < l_real_part_info._digit_length; ++i)
    {
        l_real_part /= 10.0f;
    }
    return real_info<FloatT>{l_integral_part + l_real_part, static_cast<var::uint8>(l_integral_part_info._digit_length + l_real_part_info._digit_length + 1), l_integral_part_info._digit_length, l_real_part_info._digit_length};
}




template<typename IntT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ var::uint8 count_integral_digit_length(IntT value_p) noexcept
{
    static_assert(::std::is_integral<IntT>::value == true || ::std::is_floating_point<IntT>::value == true, "the type of the template parameter IntT is not numeric");

    var::int8 l_length_of_n = 0;
    if (value_p < 0)
    {
        for (; value_p <= static_cast<IntT>(-10); value_p /= static_cast<IntT>(10)) { ++l_length_of_n; }
        return ++l_length_of_n;
    }

    for (; value_p >= static_cast<IntT>(10); value_p /= static_cast<IntT>(10)) { ++l_length_of_n; }
    return ++l_length_of_n;
}




#define _MAX_NUMERIC_STRING_LENGTH_ 512
#define _256_BYTES_BIT_COUNT_ 2048
#define _16_BYTES_BIT_COUNT_ 128


template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void int_to_string(CharT* const string_out_p, _MAYBE_UNUSED_ length_t input_string_capacity_p, var::int64 value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");
    FE_ABORT_IF(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");
    FE_ABORT_IF(value_p == FE::min_value<var::int64>, "NaCN ERROR: value_p is not a calculatable number");

    var::int8 l_integral_digits = count_integral_digit_length<var::int64>(value_p);
    var::boolean l_is_negative = false;

    if (value_p < 0)
    {
        l_is_negative = true;
        value_p *= -1;
        ++l_integral_digits;
    }
    else if (value_p == 0)
    {
        string_out_p[0] = '0';
        string_out_p[1] = '\0';
        return;
    }

    FE_ABORT_IF(input_string_capacity_p <= l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::int8 l_idx = l_integral_digits - 1;
    while (value_p > 0)
    {
        string_out_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }


    if (l_is_negative) { *string_out_p = static_cast<CharT>('-'); }

    string_out_p[l_integral_digits] = _NULL_;
}




template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void uint_to_string(CharT* const string_out_p, _MAYBE_UNUSED_ length_t input_string_capacity_p, var::uint64 value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type of value_p assigned to the template argument CharT");
    FE_ABORT_IF(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");

    var::int8 l_integral_digits = count_integral_digit_length<var::uint64>(value_p);
    var::boolean l_is_negative = false;

    if (value_p == 0)
    {
        string_out_p[0] = '0';
        string_out_p[1] = '\0';
        return;
    }

    FE_ABORT_IF(input_string_capacity_p <= l_integral_digits, "MEMORY BOUNDRY CHECK FAILURE: the digit length of an integer exceeds the output string buffer capacity");

    var::int8 l_idx = l_integral_digits - 1;
    while (value_p > 0)
    {
        string_out_p[l_idx] = ASCII_code_zero + (value_p % 10);
        value_p /= 10;
        --l_idx;
    }


    if (l_is_negative) { *string_out_p = static_cast<CharT>('-'); }

    string_out_p[l_integral_digits] = _NULL_;
}




template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void float_to_string(CharT* const string_out_p, length_t input_string_capacity_p, float64 value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    FE_ABORT_IF(string_out_p == nullptr, "NULLPTR DETECTED: string_out_p is nullptr.");

    int_to_string<CharT>(string_out_p, input_string_capacity_p, static_cast<var::int64>(value_p));

    var::uint64 l_integral_part_string_length = internal::strlen<CharT>(string_out_p);
    string_out_p[l_integral_part_string_length] = '.';
    ++l_integral_part_string_length;

    var::float64 l_floating_point = value_p - static_cast<float64>(static_cast<var::int64>(value_p));
    while (0 != (l_floating_point - static_cast<var::int64>(l_floating_point)))
    {
        l_floating_point *= 10.0f;
    }

    FE_ABORT_IF(input_string_capacity_p <= (count_integral_digit_length<var::int64>(l_floating_point) + l_integral_part_string_length), "MEMORY BOUNDRY CHECK FAILURE: the digit length of the integral part exceeds the output string buffer capacity");

    int_to_string<CharT>(string_out_p + l_integral_part_string_length, input_string_capacity_p, static_cast<var::int64>(l_floating_point));
}




template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ const CharT* boolean_to_string(boolean value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    return (value_p == true) ? static_cast<const CharT*>("true") : static_cast <const CharT*>("false");
}




template <typename CharT, typename T>
_CONSTEXPR20_ _FORCE_INLINE_ void any_object_binary_representation(CharT* const dest_buffer_p, _MAYBE_UNUSED_ capacity_t dest_buffer_capacity_p, T& object_p)
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    constexpr count_t l_required_bit_count = (sizeof(T) * 8) + sizeof(T);
    FE_ABORT_IF(dest_buffer_capacity_p < l_required_bit_count, "ERROR: The input buffer capacity is insufficient to store the binary representation of the subject");
    CharT l_buffer[l_required_bit_count] = { 0 };
    CharT* l_buffer_pointer = static_cast<CharT*>(l_buffer);

    byte* l_ptr = reinterpret_cast<byte*>(&object_p);

    std::bitset<8> l_binary_representation = *l_ptr;
    for (var::size_t i = 0; i != sizeof(T); ++i)
    {
        *l_buffer_pointer = (l_binary_representation[0] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[1] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[2] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[3] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[4] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[5] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[6] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = (l_binary_representation[7] + 48);
        ++l_buffer_pointer;
        *l_buffer_pointer = '\n';

        ++l_ptr;
        l_binary_representation = *l_ptr;
        ++l_buffer_pointer;
    }

    std::memcpy(dest_buffer_p, l_buffer, l_required_bit_count * sizeof(CharT));
}


template<typename CharT, typename T>
_CONSTEXPR20_ _FORCE_INLINE_ const CharT* any_primitive_to_string(T value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");
    FE_STATIC_ASSERT(FE::is_primitive<T>::value == false, "static assertion failed: T must be a primitive type.");
    thread_local static CharT tl_s_buffer[_MAX_NUMERIC_STRING_LENGTH_]{};

    if constexpr (FE::is_boolean<T>::value)
    {
        std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));
        return boolean_to_string<CharT>(value_p);
    }
    else if constexpr (FE::is_char<T>::value)
    {
        tl_s_buffer[0] = value_p;
        tl_s_buffer[1] = _NULL_;
        return tl_s_buffer;
    }
    else if constexpr (std::is_unsigned<T>::value && std::is_integral<T>::value)
    {
        std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));
        uint_to_string<CharT>(tl_s_buffer, _MAX_NUMERIC_STRING_LENGTH_, value_p);
        return tl_s_buffer;
    }
    else if constexpr (std::is_signed<T>::value && std::is_integral<T>::value)
    {
        std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));
        int_to_string<CharT>(tl_s_buffer, _MAX_NUMERIC_STRING_LENGTH_, value_p);
        return tl_s_buffer;
    }
    else if constexpr (std::is_floating_point<T>::value)
    {
        std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));
        float_to_string<CharT>(tl_s_buffer, _MAX_NUMERIC_STRING_LENGTH_, value_p);
        return tl_s_buffer;
    }
    else if constexpr (FE::is_c_style_constant_string<T>::value)
    {
        return value_p;
    }
    else if constexpr (std::is_pointer<T>::value)
    {
        std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));
        std::snprintf(tl_s_buffer, _MAX_NUMERIC_STRING_LENGTH_, "%p", value_p);
        return tl_s_buffer;
    }
    else if constexpr (FE::is_nullptr<T>::value)
    {
        return static_cast<const CharT*>("nullptr");
    }
}


template<typename CharT, typename T>
_CONSTEXPR20_ _FORCE_INLINE_ void any_primitive_to_string(CharT* const dest_buffer_p, capacity_t dest_buffer_capacity_p, T value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");
    FE_STATIC_ASSERT(FE::is_primitive<T>::value == false, "static assertion failed: T must be a primitive type.");

    if constexpr (FE::is_boolean<T>::value)
    {
        const CharT* const l_result = boolean_to_string<CharT>(value_p);
        uint64 l_length = internal::strlen(l_result);
        std::memcpy(dest_buffer_p, l_result, l_length * sizeof(CharT));
        dest_buffer_p[l_length] = _NULL_;
    }
    else if constexpr (FE::is_char<T>::value)
    {
        dest_buffer_p[0] = value_p;
        dest_buffer_p[1] = _NULL_;
    }
    else if constexpr (std::is_unsigned<T>::value && std::is_integral<T>::value)
    {
        uint_to_string<CharT>(dest_buffer_p, dest_buffer_capacity_p, value_p);
    }
    else if constexpr (std::is_signed<T>::value && std::is_integral<T>::value)
    {
        int_to_string<CharT>(dest_buffer_p, dest_buffer_capacity_p, value_p);
    }
    else if constexpr (std::is_floating_point<T>::value)
    {
        float_to_string<CharT>(dest_buffer_p, dest_buffer_capacity_p, value_p);
    }
    else if constexpr (FE::is_c_style_constant_string<T>::value)
    {
        uint64 l_length = internal::strlen(value_p);
        uint64 l_bytes_to_copy = l_length * sizeof(CharT);
        if (dest_buffer_capacity_p < l_bytes_to_copy) _UNLIKELY_{ std::abort(); }
        std::memcpy(dest_buffer_p, value_p, l_bytes_to_copy);
        dest_buffer_p[l_length] = _NULL_;
    }
    else if constexpr (std::is_pointer<T>::value)
    {
        std::snprintf(dest_buffer_p, dest_buffer_capacity_p, "%p", value_p);
    }
    else if constexpr (FE::is_nullptr<T>::value)
    {
        dest_buffer_p[0] = 'n';
        dest_buffer_p[1] = 'u';
        dest_buffer_p[2] = 'l';
        dest_buffer_p[3] = 'l';
        dest_buffer_p[4] = 'p';
        dest_buffer_p[5] = 't';
        dest_buffer_p[6] = 'r';
        dest_buffer_p[7] = _NULL_;
    }
}


template<typename CharT, typename U>
_CONSTEXPR20_ _FORCE_INLINE_ const CharT* any_to_string(U& value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    thread_local static CharT tl_s_buffer[_256_BYTES_BIT_COUNT_]{};
    std::memset(tl_s_buffer, _NULL_, sizeof(CharT) * internal::strlen(tl_s_buffer));

    if constexpr (std::is_class< typename std::remove_reference< decltype(value_p) >::type >::value == true)
    {
        any_object_binary_representation(tl_s_buffer, _256_BYTES_BIT_COUNT_, value_p);
    }
    else if constexpr (FE::is_primitive< typename std::remove_reference< decltype(value_p) >::type >::value == true)
    {
        any_primitive_to_string(tl_s_buffer, _256_BYTES_BIT_COUNT_, value_p);
    }
    return tl_s_buffer;
}


template<typename CharT, typename U>
_CONSTEXPR20_ _FORCE_INLINE_ void any_to_string(CharT* const dest_buffer_p, capacity_t dest_buffer_capacity_p, U& value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "an illegal type assigned to the template argument CharT");

    if constexpr (std::is_class< typename std::remove_reference< decltype(value_p) >::type >::value == true)
    {
        any_object_binary_representation(dest_buffer_p, dest_buffer_capacity_p, value_p);
    }
    else if constexpr (FE::is_primitive< typename std::remove_reference< decltype(value_p) >::type >::value == true)
    {
        any_primitive_to_string(dest_buffer_p, dest_buffer_capacity_p, value_p);
    }
}


END_NAMESPACE
#pragma warning(pop)
#endif