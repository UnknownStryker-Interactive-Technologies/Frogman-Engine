#ifndef _FE_CORE_ALGORITHM_STRING_HXX_
#define _FE_CORE_ALGORITHM_STRING_HXX_
#pragma warning(push)
#pragma warning(disable: 4702)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/iterator.hxx>
#include <FE/core/memory.hxx>
#include <optional>




BEGIN_NAMESPACE(FE::algorithm::string)


template<typename CharT, ADDRESS DestAddressAlignment = ADDRESS::_NOT_ALIGNED, ADDRESS SourceAddressAlignment = ADDRESS::_NOT_ALIGNED>
_FORCE_INLINE_ void copy(CharT* const destination_out_p, const CharT* const source_p, capacity_t count_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(destination_out_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(destination_out_p), nullptr);
    FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(source_p), nullptr);

    if constexpr (DestAddressAlignment == ADDRESS::_ALIGNED && SourceAddressAlignment == ADDRESS::_ALIGNED)
    {
        ALIGNED_MEMCPY(destination_out_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr (DestAddressAlignment == ADDRESS::_ALIGNED && SourceAddressAlignment == ADDRESS::_NOT_ALIGNED)
    {
        DEST_ALIGNED_MEMCPY(destination_out_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr (DestAddressAlignment == ADDRESS::_NOT_ALIGNED && SourceAddressAlignment == ADDRESS::_ALIGNED)
    {
        SOURCE_ALIGNED_MEMCPY(destination_out_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr (DestAddressAlignment == ADDRESS::_NOT_ALIGNED && SourceAddressAlignment == ADDRESS::_NOT_ALIGNED)
    {
        UNALIGNED_MEMCPY(destination_out_p, source_p, sizeof(CharT) * count_p);
    }

    destination_out_p[count_p] = _NULL_;
}


template<typename S>
struct count
{
    FE_STATIC_ASSERT(FE::is_char<S>::value == false && FE::is_c_style_constant_string<S>::value == false && std::is_pointer<S>::value == false, "Static Assertion Failed: The template struct count's argument has to be a character type or a pointer type to a string.");
    S _target;
    count_t _match_count;
};


template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ count<CharT> count_chars(const CharT* string_p, const CharT target_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p), nullptr);

    var::count_t l_match_count = 0;

    while (*string_p != _NULL_)
    {
        if (target_p == *string_p)
        {
            ++l_match_count;
        }
        ++string_p;
    }

    CharT l_target_char_buffer = _NULL_;

    if (l_match_count != 0)
    {
        l_target_char_buffer = target_p;
    }

    return count<CharT>{l_target_char_buffer, l_match_count};
}


_MAYBE_UNUSED_ constexpr int8 _ASCII_a_ = 97;
_MAYBE_UNUSED_ constexpr int8 _ASCII_z_ = 122;

_MAYBE_UNUSED_ constexpr int8 _ASCII_A_ = 65;
_MAYBE_UNUSED_ constexpr int8 _ASCII_Z_ = 90;
_MAYBE_UNUSED_ constexpr int8 _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_ = 32;


template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void capitalize(CharT* in_out_string_buffer_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(in_out_string_buffer_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_buffer_p), nullptr);

    while (*in_out_string_buffer_p != _NULL_)
    {
        if ((_ASCII_a_ <= static_cast<uint8>(*in_out_string_buffer_p) && static_cast<uint8>(*in_out_string_buffer_p) <= _ASCII_z_))
        {
            *in_out_string_buffer_p = static_cast<CharT>(static_cast<int16>(*in_out_string_buffer_p) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
        }
        ++in_out_string_buffer_p;
    }
}

template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ CharT capitalize(const CharT value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

    if (_ASCII_a_ <= static_cast<uint8>(value_p) && static_cast<uint8>(value_p) <= _ASCII_z_)
    {
        return static_cast<CharT>(static_cast<int16>(value_p) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
    }

    return value_p;
}


template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void to_lowercase(CharT* in_out_string_buffer_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(in_out_string_buffer_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_buffer_p), nullptr);

    while (*in_out_string_buffer_p != _NULL_)
    {
        if ((_ASCII_A_ <= static_cast<uint8>(*in_out_string_buffer_p) && static_cast<uint8>(*in_out_string_buffer_p) <= _ASCII_Z_))
        {
            *in_out_string_buffer_p = static_cast<CharT>(static_cast<int16>(*in_out_string_buffer_p) + _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
        }
        ++in_out_string_buffer_p;
    }
}

template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ CharT to_lowercase(const CharT value_p) noexcept
{
    FE_STATIC_ASSERT(sizeof(CharT) > sizeof(UTF32), "CharT is not a valid character type");

    if (_ASCII_A_ <= static_cast<uint8>(value_p) && static_cast<uint8>(value_p) <= _ASCII_Z_)
    {
        return static_cast<CharT>(static_cast<int16>(value_p) + _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
    }

    return value_p;
}


template<typename CharT>
_CONSTEXPR20_ void capitalize_every_first_letter_of_words(CharT* in_out_string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(in_out_string_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p), nullptr);

    boolean l_is_front_not_spaced = (*in_out_string_p) != ' ';
    boolean l_is_front_not_underscored = (*in_out_string_p) != '_';
    if (l_is_front_not_spaced || l_is_front_not_underscored)
    {
        *in_out_string_p = static_cast<int16>(*in_out_string_p) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_;
    }

    while (*in_out_string_p != _NULL_)
    {
        if (((*in_out_string_p == static_cast<CharT>(' ')) && (in_out_string_p[1] != static_cast<CharT>(' '))) ||
            ((*in_out_string_p == static_cast<CharT>('_')) && (in_out_string_p[1] != static_cast<CharT>('_'))))
        {
            if (((_ASCII_A_ <= in_out_string_p[1]) && (in_out_string_p[1] <= _ASCII_Z_))
                || ((_ASCII_a_ <= in_out_string_p[1]) && (in_out_string_p[1] <= _ASCII_z_)))
            {
                in_out_string_p[1] = static_cast<int16>(in_out_string_p[1]) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_;
            }
        }
        ++in_out_string_p;
    }
}


template <typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ uint64 length(const CharT* const string_p) noexcept // returns a string length without the null terminator
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p), nullptr);

    const CharT* l_iterator_pointer = string_p;

    while (*(l_iterator_pointer) != _NULL_) { ++l_iterator_pointer; }

    return (l_iterator_pointer - string_p);
}


struct range
{
    var::index_t _begin;
    var::index_t _end;
};


template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ count<CharT> count_chars_within_range(const CharT* string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p), nullptr);
    FE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: the ${%s@1} cannot be greater than ${%s@2}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end), &string_range_p._begin, &string_range_p._end);
    FE_ASSERT(length(string_p) > string_range_p._end, "${%s@0}: the ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(length(string_p)), TO_STRING(string_range_p._end));

    var::count_t l_match_count = 0;
    string_p += string_range_p._begin;

    while (*string_p != _NULL_)
    {
        if (target_p == *string_p)
        {
            ++l_match_count;
        }
        ++string_p;
    }

    CharT l_target_char_buffer = _NULL_;

    if (l_match_count != 0)
    {
        l_target_char_buffer = target_p;
    }

    return count<CharT>{l_target_char_buffer, l_match_count};
}


template <typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ boolean compare(const CharT* lstr_p, const CharT* rstr_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(lstr_p), nullptr);
    FE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(rstr_p), nullptr);

    while ((*lstr_p != _NULL_) && (*lstr_p == *rstr_p))
    {
        ++lstr_p;
        ++rstr_p;
    }

    return ((*lstr_p - *rstr_p) == 0) ? true : false;
}


template <class CharT>
struct equal_to
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

    _NODISCARD constexpr boolean operator()(const CharT* const left_p, const CharT* const right_p) const noexcept
    {
        FE_ASSERT(left_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(left_p), nullptr);
        FE_ASSERT(right_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(right_p), nullptr);

        return compare(left_p, right_p);
    }
};


template <typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ boolean compare_ranged(const CharT* const lstr_p, const range lstr_range_p, const CharT* const rstr_p, const range rstr_range_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(lstr_ptr_p), nullptr);
    FE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(rstr_ptr_p), nullptr);

    length_t l_lstr_length = lstr_range_p._end - lstr_range_p._begin;
    length_t l_rstr_length = rstr_range_p._end - rstr_range_p._begin;

    if ((l_lstr_length != l_rstr_length) || (l_lstr_length == 0))
    {
        return false;
    }

    const CharT* l_lstr_pointer = lstr_p + lstr_range_p._begin;
    const CharT* l_rstr_pointer = rstr_p + rstr_range_p._begin;


    var::count_t l_character_match_count = 0;
    while ((l_character_match_count < l_lstr_length) && (*l_lstr_pointer == *l_rstr_pointer))
    {
        ++l_character_match_count;
        ++l_lstr_pointer;
        ++l_rstr_pointer;
    }

    if (l_character_match_count == l_lstr_length)
    {
        return true;
    }

    return false;
}


template <typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ boolean insensitive_comparison(const CharT* const lstr_p, const CharT* const rstr_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(lstr_ptr_p), nullptr);
    FE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(rstr_ptr_p), nullptr);

    const CharT* l_left_string_pointer = lstr_p;
    const CharT* l_right_string_pointer = rstr_p;

    while (string::capitalize(*l_left_string_pointer) == string::capitalize(*l_right_string_pointer) && *l_left_string_pointer != static_cast<CharT>('\0'))
    {
        ++l_left_string_pointer;
        ++l_right_string_pointer;
    }

    if (*l_left_string_pointer == *l_right_string_pointer)
    {
        return true;
    }

    return false;
}


template <typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ boolean insensitive_ranged_comparison(const CharT* const lstr_p, const range lstr_range_p, const CharT* const rstr_p, const range rstr_range_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(lstr_ptr_p), nullptr);
    FE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(rstr_ptr_p), nullptr);

    length_t l_left_string_length = lstr_range_p._end - lstr_range_p._begin;
    length_t l_right_string_length = rstr_range_p._end - rstr_range_p._begin;

    if (l_left_string_length != l_right_string_length)
    {
        return false;
    }

    const CharT* l_left_string_pointer = lstr_p + lstr_range_p._begin;
    const CharT* l_right_string_pointer = rstr_p + rstr_range_p._begin;

    const CharT* const l_end_of_left_string = lstr_p + lstr_range_p._end;


    while (l_left_string_pointer != l_end_of_left_string && string::capitalize(*l_left_string_pointer) == string::capitalize(*l_right_string_pointer))
    {
        ++l_left_string_pointer;
        ++l_right_string_pointer;
    }

    if (*l_left_string_pointer == *l_right_string_pointer)
    {
        return true;
    }

    return false;
}


template <typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void concatenate(CharT* const destination_out_p, _MAYBE_UNUSED_ count_t string_buffer_size_p, const CharT* const source_p, count_t source_total_count_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(destination_out_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(destination_out_p), nullptr);
    FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(source_p), nullptr);

    FE_ASSERT(string_buffer_size_p < source_total_count_p, "${%s@0}: the ${%s@2} cannot be greater than ${%s@1}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(string_buffer_size_p), TO_STRING(source_total_count_p), &string_buffer_size_p, &source_total_count_p);

    length_t l_destination_string_length = algorithm::string::length<CharT>(destination_out_p);
 
    std::memcpy(destination_out_p + l_destination_string_length, source_p, source_total_count_p * sizeof(CharT));
}


template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void concatenate(CharT* const out_string_buffer_p, _MAYBE_UNUSED_ size_t string_buffer_size_p, ::std::initializer_list<const CharT* const>&& strings_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ABORT_IF(out_string_buffer_p == nullptr, "NULLPTR DETECTED: out_string_buffer_p is nullptr.");

    var::size_t l_current_begin_index = algorithm::string::length(out_string_buffer_p);

    for (var::size_t i = 0; i < strings_p.size(); ++i)
    {
        size_t l_string_length_buffer = algorithm::string::length(strings_p.begin()[i]);

        FE_ABORT_IF(string_buffer_size_p <= l_current_begin_index, "MEMORY BOUNDARY CHECK FAILURES: the string_buffer_size_p is smaller or equal to the l_current_begin_index");

        std::memcpy(out_string_buffer_p + l_current_begin_index, strings_p.begin()[i], l_string_length_buffer * sizeof(CharT));
        l_current_begin_index += l_string_length_buffer;
    }
}


template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void concatenate(CharT* const out_string_buffer_p, _MAYBE_UNUSED_ size_t string_buffer_size_p, ::std::initializer_list<const CharT>&& chars_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(out_string_buffer_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(out_string_buffer_p));

    var::length_t l_string_buffer_length = algorithm::string::length(out_string_buffer_p);

    FE_ASSERT(string_buffer_size_p <= chars_p.size() + l_string_buffer_length, "${%s@0}: The total input string length exceeds the destination string buffer capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

    ::memcpy(out_string_buffer_p + l_string_buffer_length, chars_p.begin(), sizeof(CharT) * chars_p.size());
}


template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ std::optional<range> find_the_first(const CharT* const string_p, const CharT target_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

    const CharT* l_string_pointer = string_p;
    while (*l_string_pointer != _NULL_)
    {
        if (*l_string_pointer == target_p)
        {
            index_t l_position = l_string_pointer - string_p;
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_pointer;
    }
        
    return std::nullopt;
}


template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_last(const CharT* const string_p, const CharT target_char_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rbegin = string_p + (length(string_p)-1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rend = string_p - 1;

    while (l_string_rbegin != l_string_rend)
    {
        if (*l_string_rbegin == target_char_p)
        {
            index_t l_position = l_string_rbegin.operator->() - string_p;
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_rbegin;
    }

    return std::nullopt;
}




template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_first_within_range(const CharT* const string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@2} cannot be greater than the length of ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(length(string_p)), TO_STRING(string_range_p._end));

    const CharT* l_string_pointer = string_p + string_range_p._begin;
    while (*l_string_pointer != _NULL_)
    {
        if (*l_string_pointer == target_p)
        {
            index_t l_position = l_string_pointer - string_p;
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_pointer;
    }

    return std::nullopt;
}


template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_last_within_range(const CharT* const string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@2} cannot be greater than the length of ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(length(string_p)), TO_STRING(string_range_p._end));

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rbegin = string_p + (string_range_p._end - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rend = string_p + (string_range_p._begin - 1);

    while (l_string_rbegin != l_string_rend)
    {
        if (*l_string_rbegin == target_p)
        {
            index_t l_position = l_string_rbegin.operator->() - string_p;
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_rbegin;
    }

    return std::nullopt;
}




template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_first(const CharT* const string_p, const CharT* const target_substring_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(target_substring_p));

    const CharT* l_string_pointer = string_p;
    const CharT* l_target_substring_pointer = target_substring_p;
    length_t l_string_length = length(string_p);
    length_t l_target_substring_length = length(target_substring_p);
 
    FE_ASSERT(l_target_substring_length > l_string_length, "${%s@0}: the ${%s@1} is greater than to the string length of ${%s@2}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(l_target_substring_length), TO_STRING(l_string_length), &l_target_substring_length, &l_string_length);

    var::index_t l_string_idx = 0;
    while (*l_target_substring_pointer != _NULL_ && l_string_idx < l_string_length)
    {
        if (*l_string_pointer == *l_target_substring_pointer)
        {
            ++l_string_pointer;
            ++l_target_substring_pointer;
            continue;
        }
        ++l_string_pointer;
        l_target_substring_pointer = target_substring_p;
        ++l_string_idx;
    }

    index_t l_begin = (l_string_pointer - string_p) - (l_target_substring_pointer - target_substring_p);
    index_t l_end = l_string_pointer - string_p;

    if (compare_ranged(string_p, range{ l_begin, l_end }, target_substring_p, range{ 0, l_target_substring_length }))
    {
        return std::make_optional(range{ l_begin, l_end });
    }

    return std::nullopt;
}




template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_last(const CharT* const string_p, const CharT* const target_substring_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(target_substring_p));

    length_t l_string_length = length(string_p);
    length_t l_target_substring_length = length(target_substring_p);

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_crbegin(string_p + (l_string_length - 1));

    const FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crbegin_read_only(target_substring_p + (l_target_substring_length - 1));
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crbegin = l_target_substring_crbegin_read_only;

    const FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crend_read_only(target_substring_p - 1);

    var::index_t l_string_idx = 0;
    while (l_target_substring_crbegin != l_target_substring_crend_read_only && l_string_idx < l_string_length)
    {
        if (*l_string_crbegin == *l_target_substring_crbegin)
        {
            ++l_string_crbegin;
            ++l_target_substring_crbegin;
            continue;
        }
        ++l_string_crbegin;
        l_target_substring_crbegin = l_target_substring_crbegin_read_only;
        ++l_string_idx;
    }

    index_t l_begin = (l_string_crbegin.operator->() - string_p) + 1;
    index_t l_end = ((l_string_crbegin.operator->() + l_target_substring_length) - string_p) + 1;

    if ( compare_ranged(string_p, range{ l_begin, l_end }, target_substring_p, range{0, l_target_substring_length}) )
    {
        return  std::make_optional(range{ l_begin, l_end });
    }

    return std::nullopt;
}




template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_first_within_range(const CharT* const string_p, const range string_range_p, const CharT* const target_substring_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than the length of ${%s@2}.", TO_STRING(length(string_p)), TO_STRING(string_range_p._end));

    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(target_substring_p));

    const CharT* l_string_pointer = string_p + string_range_p._begin;
    const CharT* l_target_substring_pointer = target_substring_p;
    length_t l_string_length = string_range_p._end - string_range_p._begin;
    length_t l_target_substring_length = length(target_substring_p);

    FE_ASSERT(l_target_substring_length > l_string_length, "${%s@0}: the ${%s@1} is greater than or equal to the string length of ${%s@2}", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(l_target_substring_length), TO_STRING(l_string_length));

    var::index_t l_string_idx = 0;
    while (*l_target_substring_pointer != _NULL_ && l_string_idx < l_string_length)
    {
        if (*l_string_pointer == *l_target_substring_pointer)
        {
            ++l_string_pointer;
            ++l_target_substring_pointer;
            continue;
        }
        ++l_string_pointer;
        l_target_substring_pointer = target_substring_p;
        ++l_string_idx;
    }

    index_t l_begin = (l_string_pointer - string_p) - (l_target_substring_pointer - target_substring_p);
    index_t l_end = l_string_pointer - string_p;

    if (compare_ranged(string_p, range{l_begin, l_end}, target_substring_p, range{0, l_target_substring_length}))
    {
        return  std::make_optional(range{ l_begin, l_end });
    }

    return std::nullopt;
}




template<typename CharT>
_NODISCARD_ _CONSTEXPR20_ std::optional<range> find_the_last_within_range(const CharT* const string_p, const range string_range_p, const CharT* const target_substring_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than the length of ${%s@2}.", TO_STRING(length(string_p)), TO_STRING(string_range_p._end));

    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
    FE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(target_substring_p));


    length_t l_string_length = string_range_p._end - string_range_p._begin;
    length_t l_target_substring_length = algorithm::string::length(target_substring_p);

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_crbegin(string_p + (l_string_length - 1));

    const FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crbegin_read_only(target_substring_p + (l_target_substring_length - 1));
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crbegin = l_target_substring_crbegin_read_only;

    const FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substring_crend_read_only(target_substring_p - 1);

    var::index_t l_string_idx = 0;
    while (l_target_substring_crbegin != l_target_substring_crend_read_only && l_string_idx < l_string_length)
    {
        if (*l_string_crbegin == *l_target_substring_crbegin)
        {
            ++l_string_crbegin;
            ++l_target_substring_crbegin;
            continue;
        }
        ++l_string_crbegin;
        l_target_substring_crbegin = l_target_substring_crbegin_read_only;
        ++l_string_idx;
    }

    index_t l_begin = (l_string_crbegin.operator->() - string_p) + 1;
    index_t l_end = ((l_string_crbegin.operator->() + l_target_substring_length) - string_p) + 1;

    if (compare_ranged(string_p, range{ l_begin, l_end }, target_substring_p, range{ 0, l_target_substring_length }))
    {
        return std::make_optional(range{ l_begin, l_end });
    }

    return std::nullopt;
}




template<typename CharT, typename IntT>
_CONSTEXPR20_ _FORCE_INLINE_ IntT char_to_integer(const CharT value_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_STATIC_ASSERT(::std::is_integral<IntT>::value == false, "typename IntT is not an integer");

    return static_cast<IntT>(value_p) - 48;
}




template <typename CharT, typename IntT>
_NODISCARD_ _CONSTEXPR20_ IntT string_to_integer(const CharT* string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

    var::int16 l_sign = 1;
    if (*string_p == static_cast<const CharT>('-')) { l_sign = -1; }
    while (*string_p >= static_cast<const CharT>('0') && *string_p <= static_cast<const CharT>('9') && *string_p != _NULL_)
    {
        ++string_p;
    }
    --string_p;

    IntT l_result = 0;
    IntT l_multiplier = 1;
    while (*string_p >= static_cast<const CharT>('0') && *string_p <= static_cast<const CharT>('9') && *string_p != _NULL_)
    {
        l_result += static_cast<IntT>(*string_p - static_cast<const CharT>('0')) * l_multiplier;
        l_multiplier *= 10;
        --string_p;
    }
    return l_sign * l_result;
}




template<typename CharT>
_CONSTEXPR20_ _FORCE_INLINE_ void invert(CharT* const in_out_string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(in_out_string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p));

    var::index_t l_start = 0;
    var::index_t l_end = length(in_out_string_p);

    CharT l_temporary = _NULL_;
    while (l_start < l_end)
    {
        l_temporary = in_out_string_p[l_start];
        --l_end;
        in_out_string_p[l_start] = in_out_string_p[l_end];
        in_out_string_p[l_end] = l_temporary;
        ++l_start;
    }
}




/*
This function can be useful when a string needs to be hashed during compile time.
TIP: use this function with switch cases.
*/
template<typename CharT>
_CONSTEXPR17_ uint64 hash_string(const CharT* string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

    constexpr uint64 l_magical_prime_number = 257;

    if (*string_p == _NULL_)
    {
        return 0;
    }

    var::uint64 l_hash = 0;
    for (; *string_p != _NULL_; ++string_p)
    {
        l_hash = ((l_hash * l_hash * l_hash) + *string_p) ^ l_magical_prime_number;
    }

    return l_hash;
}
#define SWITCH(string_p) switch( ::FE::algorithm::string::hash_string(string_p) )
#define CASE(string_p) case ::FE::algorithm::string::hash_string(string_p)




END_NAMESPACE
#pragma warning(pop)
#endif