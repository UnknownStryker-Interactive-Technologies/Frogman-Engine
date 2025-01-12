#ifndef _FE_CORE_ALGORITHM_STRING_HXX_
#define _FE_CORE_ALGORITHM_STRING_HXX_
#pragma warning(push)
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
#include <FE/iterator.hxx>
#include <FE/memory.hxx>

// std
#include <optional>




BEGIN_NAMESPACE(FE::algorithm::string)


template<typename CharT, FE::Address DestAddressAlignment = FE::Address::_NotAligned, FE::Address SourceAddressAlignment = FE::Address::_NotAligned>
_FE_FORCE_INLINE_ void copy(CharT* const out_dest_p, const CharT* const source_p, FE::uint64 count_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p), nullptr);
    FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(source_p), nullptr);

    if constexpr ((DestAddressAlignment == Address::_Aligned) && (SourceAddressAlignment == Address::_Aligned))
    {
        FE_ALIGNED_MEMCPY(out_dest_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr ((DestAddressAlignment == Address::_Aligned) && (SourceAddressAlignment == Address::_NotAligned))
    {
        FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr ((DestAddressAlignment == Address::_NotAligned) && (SourceAddressAlignment == Address::_Aligned))
    {
        FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, sizeof(CharT) * count_p);
    }
    else if constexpr ((DestAddressAlignment == Address::_NotAligned) && (SourceAddressAlignment == Address::_NotAligned))
    {
        FE_UNALIGNED_MEMCPY(out_dest_p, source_p, sizeof(CharT) * count_p);
    }
    out_dest_p[count_p] = null;
}


template<typename S>
struct count
{
    FE_NEGATIVE_STATIC_ASSERT((FE::is_char<S>::value == false) && (FE::is_constant_string<S>::value == false) && (std::is_pointer<S>::value == false), "Static Assertion Failed: The template struct count's argument has to be a character type or a pointer type to a string.");
    S _target;
    var::uint64 _match_count;
};


template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ count<CharT> count_chars(const CharT* string_p, const CharT target_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

    var::uint64 l_match_count = 0;

    while (*string_p != null)
    {
        if (target_p == *string_p)
        {
            ++l_match_count;
        }
        ++string_p;
    }

    count<CharT> l_result;
    l_result._target = (l_match_count != 0) ? target_p : null;
    l_result._match_count = l_match_count;
    return l_result;
}


_FE_MAYBE_UNUSED_ constexpr int8 _ASCII_a_ = 97;
_FE_MAYBE_UNUSED_ constexpr int8 _ASCII_z_ = 122;

_FE_MAYBE_UNUSED_ constexpr int8 _ASCII_A_ = 65;
_FE_MAYBE_UNUSED_ constexpr int8 _ASCII_Z_ = 90;
_FE_MAYBE_UNUSED_ constexpr int8 _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_ = 32;


template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ void capitalize(CharT* in_out_string_buffer_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(in_out_string_buffer_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
    
    while (*in_out_string_buffer_p != null)
    {
        if ((_ASCII_a_ <= static_cast<CharT>(*in_out_string_buffer_p)) && (static_cast<CharT>(*in_out_string_buffer_p) <= _ASCII_z_))
        {
            *in_out_string_buffer_p = static_cast<CharT>(static_cast<CharT>(*in_out_string_buffer_p) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
        }
        ++in_out_string_buffer_p;
    }
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ CharT capitalize(const CharT value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

    if ((_ASCII_a_ <= static_cast<CharT>(value_p)) && (static_cast<CharT>(value_p) <= _ASCII_z_))
    {
        return static_cast<CharT>(static_cast<CharT>(value_p) - _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
    }

    return value_p;
}


template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ void to_lowercase(CharT* in_out_string_buffer_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(in_out_string_buffer_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

    while (*in_out_string_buffer_p != null)
    {
        if (((_ASCII_A_ <= static_cast<CharT>(*in_out_string_buffer_p)) && (static_cast<CharT>(*in_out_string_buffer_p) <= _ASCII_Z_)))
        {
            *in_out_string_buffer_p = static_cast<CharT>(static_cast<CharT>(*in_out_string_buffer_p) + _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
        }
        ++in_out_string_buffer_p;
    }
}

template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ CharT to_lowercase(const CharT value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(sizeof(CharT) > sizeof(UTF32), "CharT is not a valid character type");

    if ((_ASCII_A_ <= static_cast<CharT>(value_p)) && (static_cast<CharT>(value_p) <= _ASCII_Z_))
    {
        return static_cast<CharT>(static_cast<CharT>(value_p) + _ASCII_GAP_BETWEEN_UPPERCASE_and_lowercase_);
    }

    return value_p;
}


template<typename CharT>
_FE_CONSTEXPR17_ void capitalize_the_first_letter_of_words(CharT* in_out_string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(in_out_string_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

	var::boolean l_is_space_or_underscore = true;

    while (*in_out_string_p != null)
    {
		if ((*in_out_string_p == ' ') || (*in_out_string_p == '_'))
        {
			l_is_space_or_underscore = true;
		}
        else if (l_is_space_or_underscore)
        {
			*in_out_string_p = string::capitalize(*in_out_string_p);
			l_is_space_or_underscore = false;
		}
        ++in_out_string_p;
    }
}


template <typename CharT>
_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR17_ uint64 length(const CharT* const string_p) noexcept // returns a string length without the null terminator
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

    const CharT* l_iterator = string_p;

    while (*(l_iterator) != null) { ++l_iterator; }

    return static_cast<uint64>(l_iterator - string_p);
}


struct range
{
    var::uint64 _begin;
    var::uint64 _end;
};


template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ count<CharT> count_chars_within_range(const CharT* string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: The input string was nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
    FE_NEGATIVE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: the ${%s@1} cannot be greater than ${%s@2}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end), &string_range_p._begin, &string_range_p._end);
    FE_NEGATIVE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@1} value is pointing the outside of the string range.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(string_range_p._end));

    var::uint64 l_match_count = 0;
    string_p += string_range_p._begin;
	const CharT* const l_end_of_string = string_p + string_range_p._end;

    while ((*string_p != null) && (string_p != l_end_of_string))
    {
        if (target_p == *string_p)
        {
            ++l_match_count;
        }
        ++string_p;
    }

    count<CharT> l_result;
    l_result._target = (l_match_count != 0) ? target_p : null;
    l_result._match_count = l_match_count;
    return l_result;
}


template <typename CharT>
_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR17_ boolean compare(const CharT* const lstr_p, const CharT* const rstr_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(lstr_p), nullptr);
    FE_NEGATIVE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(rstr_p), nullptr);

	const CharT* l_lstr_iterator = lstr_p;
	const CharT* l_rstr_iterator = rstr_p;

    while ((*l_lstr_iterator != FE::null) || (*l_rstr_iterator != FE::null))
    {
        if (*l_lstr_iterator != *l_rstr_iterator)
        {
            return false;
        }
        ++l_lstr_iterator;
        ++l_rstr_iterator;
    }

    return true;
}


template <class CharT>
struct equal_to
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

    _FE_NODISCARD_ _FE_CONSTEXPR17_ boolean operator()(const CharT* const left_p, const CharT* const right_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(left_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(left_p), nullptr);
        FE_NEGATIVE_ASSERT(right_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(right_p), nullptr);

        return compare(left_p, right_p);
    }
};


template <typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ boolean compare_ranged(const CharT* const lstr_p, const range lstr_range_p, const CharT* const rstr_p, const range rstr_range_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(lstr_ptr_p), nullptr);
    FE_NEGATIVE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(rstr_ptr_p), nullptr);

    uint64 l_lstr_length = lstr_range_p._end - lstr_range_p._begin;
	uint64 l_rstr_length = rstr_range_p._end - rstr_range_p._begin;

    if ((l_lstr_length != l_rstr_length) || (l_lstr_length == 0))
    {
        return false;
    }

	const CharT* l_lstr_iterator = lstr_p + lstr_range_p._begin;
	const CharT* l_rstr_iterator = rstr_p + rstr_range_p._begin;
	const CharT* const l_end_of_lstr = lstr_p + lstr_range_p._end;
    const CharT* const l_end_of_rstr = rstr_p + rstr_range_p._end;

    while ((l_lstr_iterator < l_end_of_lstr) || (l_rstr_iterator < l_end_of_rstr))
    {
        if (*l_lstr_iterator != *l_rstr_iterator)
        {
            return false;
        }
        ++l_lstr_iterator;
        ++l_rstr_iterator;
    }

    return true;
}


template <typename CharT>
_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR17_ boolean insensitive_comparison(const CharT* lstr_p, const CharT* rstr_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(lstr_ptr_p), nullptr);
    FE_NEGATIVE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(rstr_ptr_p), nullptr);

    const CharT* l_lstr_iterator = lstr_p;
    const CharT* l_rstr_iterator = rstr_p;

    while ((*l_lstr_iterator != FE::null) || (*l_rstr_iterator != FE::null))
    {
        if (string::capitalize(*l_lstr_iterator) != string::capitalize(*l_rstr_iterator))
        {
            return false;
        }
        ++l_lstr_iterator;
        ++l_rstr_iterator;
    }

    return true;
}


template <typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ boolean insensitive_ranged_comparison(const CharT* const lstr_p, const range lstr_range_p, const CharT* const rstr_p, const range rstr_range_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(lstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(lstr_ptr_p), nullptr);
    FE_NEGATIVE_ASSERT(rstr_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(rstr_ptr_p), nullptr);

    uint64 l_lstrlen = lstr_range_p._end - lstr_range_p._begin;
    uint64 l_rstrlen = rstr_range_p._end - rstr_range_p._begin;

    if ((l_lstrlen != l_rstrlen) || (l_lstrlen == 0))
    {
        return false;
    }

    const CharT* l_lstr_iterator = lstr_p + lstr_range_p._begin;
    const CharT* l_rstr_iterator = rstr_p + rstr_range_p._begin;
    const CharT* const l_end_of_lstr = lstr_p + lstr_range_p._end;
    const CharT* const l_end_of_rstr = rstr_p + rstr_range_p._end;

    while ((l_lstr_iterator < l_end_of_lstr) || (l_rstr_iterator < l_end_of_rstr))
    {
        if (string::capitalize(*l_lstr_iterator) != string::capitalize(*l_rstr_iterator))
        {
            return false;
        }
        ++l_lstr_iterator;
        ++l_rstr_iterator;
    }

    return true;
}


template <typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void concatenate(CharT* const out_dest_p, _FE_MAYBE_UNUSED_ uint64 string_buffer_size_p, const CharT* const source_p, uint64 source_total_count_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p), nullptr);
    FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is ${%p@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(source_p), nullptr);

    FE_NEGATIVE_ASSERT(string_buffer_size_p < source_total_count_p, "${%s@0}: the ${%s@2} cannot be greater than ${%s@1}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(string_buffer_size_p), TO_STRING(source_total_count_p), &string_buffer_size_p, &source_total_count_p);
 
    std::memcpy(out_dest_p + algorithm::string::length<CharT>(out_dest_p), source_p, source_total_count_p * sizeof(CharT));
}


template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void concatenate(CharT* const out_string_buffer_p, _FE_MAYBE_UNUSED_ size string_buffer_size_p, ::std::initializer_list<const CharT* const>&& strings_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    assert(out_string_buffer_p != nullptr);

    var::size l_current_begin_index = algorithm::string::length(out_string_buffer_p);

    for (var::size i = 0; i < strings_p.size(); ++i)
    {
        size l_string_length_buffer = algorithm::string::length(strings_p.begin()[i]);

        assert(!(string_buffer_size_p <= l_current_begin_index));

        std::memcpy(out_string_buffer_p + l_current_begin_index, strings_p.begin()[i], l_string_length_buffer * sizeof(CharT));
        l_current_begin_index += l_string_length_buffer;
    }
}


template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void concatenate(CharT* const out_string_buffer_p, _FE_MAYBE_UNUSED_ size string_buffer_size_p, ::std::initializer_list<const CharT>&& chars_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(out_string_buffer_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_string_buffer_p));

    FE_NEGATIVE_ASSERT(string_buffer_size_p <= chars_p.size() + algorithm::string::length(out_string_buffer_p), "${%s@0}: The total input string length exceeds the destination string buffer capacity.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation));

    std::memcpy(out_string_buffer_p + algorithm::string::length(out_string_buffer_p), chars_p.begin(), sizeof(CharT) * chars_p.size());
}


template<typename CharT>
_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR17_ std::optional<range> find_the_first(const CharT* const string_p, const CharT target_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));

    const CharT* l_iterator = string_p;
    while (*l_iterator != null)
    {
        if (*l_iterator == target_p)
        {
            uint64 l_position = static_cast<uint64>(l_iterator - string_p);
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_iterator;
    }

    return std::nullopt;
}


template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_last(const CharT* const string_p, const CharT target_char_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));

    FE_NEGATIVE_ASSERT(*string_p == null, "Assertion failed: the input string range length must not be zero.");

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rbegin = string_p + (length(string_p) - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rend = string_p - 1;

    while (l_string_rbegin != l_string_rend)
    {
        if (*l_string_rbegin == target_char_p)
        {
            uint64 l_position = static_cast<uint64>(l_string_rbegin.operator->() - string_p);
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_rbegin;
    }

    return std::nullopt;
}




template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_first_within_range(const CharT* const string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@2} cannot be greater than the length of ${%s@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(length(string_p)), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT((string_range_p._end - string_range_p._begin) == 0, "Assertion failed: the input string range length must not be zero.");

    const CharT* l_iterator = string_p + string_range_p._begin;
    const CharT* const l_end_of_string = string_p + string_range_p._end;
    while ((*l_iterator != null) && (l_iterator != l_end_of_string))
    {
        if (*l_iterator == target_p)
        {
            uint64 l_position = static_cast<uint64>(l_iterator - string_p);
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_iterator;
    }

    return std::nullopt;
}


template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_last_within_range(const CharT* const string_p, const range string_range_p, const CharT target_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT(length(string_p) < string_range_p._end, "${%s@0}: ${%s@2} cannot be greater than the length of ${%s@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation), TO_STRING(length(string_p)), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT((string_range_p._end - string_range_p._begin) == 0, "Assertion failed: the input string range length must not be zero.");

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rbegin = string_p + (string_range_p._end - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_string_rend = string_p + (string_range_p._begin - 1);

    while (l_string_rbegin != l_string_rend)
    {
        if (*l_string_rbegin == target_p)
        {
            uint64 l_position = static_cast<uint64>(l_string_rbegin.operator->() - string_p);
            return std::make_optional(range{ l_position,  l_position + 1 });
        }
        ++l_string_rbegin;
    }

    return std::nullopt;
}




template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_first(const CharT* const string_p, const CharT* const target_substring_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(target_substring_p));

    const CharT* l_iterator = string_p;
    const CharT* const l_end_of_it = string_p + length(string_p);

    const CharT* l_target_substr_iterator = target_substring_p;
    uint64 l_target_substr_length = length(target_substring_p);

    FE_NEGATIVE_ASSERT((l_iterator == l_end_of_it) || (*l_iterator == null), "Assertion failed: the input string range length must not be zero.");
    if (l_target_substr_length > static_cast<uint64>(l_end_of_it - l_iterator))
    {
        return std::nullopt;
    }

    while ((*l_target_substr_iterator != null) && (l_iterator != l_end_of_it))
    {
        if (*l_iterator == *l_target_substr_iterator)
        {
            ++l_target_substr_iterator;
        }
        else
        {
            l_target_substr_iterator = target_substring_p;
        }
        ++l_iterator;
    }

    if (static_cast<uint64>(l_target_substr_iterator - target_substring_p) == l_target_substr_length)
    {
        return std::make_optional(range{ static_cast<uint64>((l_iterator - string_p) - l_target_substr_length), static_cast<uint64>(l_iterator - string_p) });
    }
    return std::nullopt;
}




template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_last(const CharT* const string_p, const CharT* const target_substring_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(target_substring_p));

    uint64 l_target_substr_length = length(target_substring_p);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_iterator((string_p + length(string_p)) - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_end_of_it = string_p - 1;
    FE_NEGATIVE_ASSERT((l_iterator == l_end_of_it) || (*l_iterator == null), "Assertion failed: the input string range length must not be zero.");
    if (l_target_substr_length > static_cast<uint64>(l_end_of_it - l_iterator))
    {
        return std::nullopt;
    }

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substr_iterator((target_substring_p + l_target_substr_length) - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substr_begin = l_target_substr_iterator;

    while ((*l_target_substr_iterator != null) && (l_iterator != l_end_of_it))
    {
        if (*l_iterator == *l_target_substr_iterator)
        {
            ++l_target_substr_iterator;
        }
        else
        {
            l_target_substr_iterator = l_target_substr_begin;
        }
        ++l_iterator;
    }

    if (static_cast<uint64>(l_target_substr_iterator - l_target_substr_begin) == l_target_substr_length)
    {
        uint64 l_begin = static_cast<uint64>((FE::iterator_cast<const CharT* const>(l_iterator) - string_p) + 1);
        return std::make_optional(range{ l_begin, l_begin + l_target_substr_length });
    }
    return std::nullopt;
}




template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_first_within_range(const CharT* const string_p, const range string_range_p, const CharT* const target_substring_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(target_substring_p));

    const CharT* l_iterator = string_p + string_range_p._begin;
    const CharT* const l_end_of_it = string_p + string_range_p._end;

    const CharT* l_target_substr_iterator = target_substring_p;
    uint64 l_target_substr_length = length(target_substring_p);

    FE_NEGATIVE_ASSERT((l_iterator == l_end_of_it) || (*l_iterator == null), "Assertion failed: the input string range length must not be zero.");
    if (l_target_substr_length > static_cast<uint64>(l_end_of_it - l_iterator))
    {
        return std::nullopt;
    }

    while ((*l_target_substr_iterator != null) && (l_iterator != l_end_of_it))
    {
        if (*l_iterator == *l_target_substr_iterator)
        {
            ++l_target_substr_iterator;
        }
        else
        {
			l_target_substr_iterator = target_substring_p;
        }
        ++l_iterator;
    }

    if (static_cast<uint64>(l_target_substr_iterator - target_substring_p) == l_target_substr_length)
    {
        const CharT* const l_begin = (string_p + string_range_p._begin);
        return std::make_optional(range{ static_cast<uint64>((l_iterator - l_begin) - l_target_substr_length), static_cast<uint64>(l_iterator - l_begin) });
    }
    return std::nullopt;
}




template<typename CharT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ std::optional<range> find_the_last_within_range(const CharT* const string_p, const range string_range_p, const CharT* const target_substring_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_range_p._begin > string_range_p._end, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(string_range_p._begin), TO_STRING(string_range_p._end));
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));
    FE_NEGATIVE_ASSERT(target_substring_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(target_substring_p));

    uint64 l_target_substr_length = length(target_substring_p);

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_iterator((string_p + string_range_p._end) - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_end_of_it = (string_p + string_range_p._begin) - 1;
    FE_NEGATIVE_ASSERT((l_iterator == l_end_of_it) || (*l_iterator == null), "Assertion failed: the input string range length must not be zero.");
    if (l_target_substr_length > static_cast<uint64>(l_end_of_it - l_iterator))
    {
        return std::nullopt;
    }

    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substr_iterator((target_substring_p + l_target_substr_length) - 1);
    FE::const_reverse_iterator<FE::contiguous_iterator<const CharT>> l_target_substr_begin = l_target_substr_iterator;

    while ((*l_target_substr_iterator != null) && (l_iterator != l_end_of_it))
    {
        if (*l_iterator == *l_target_substr_iterator)
        {
            ++l_target_substr_iterator;
        }
        else
        {
            l_target_substr_iterator = l_target_substr_begin;
        }
        ++l_iterator;
    }

    if (static_cast<uint64>(l_target_substr_iterator - l_target_substr_begin) == l_target_substr_length)
    {
        uint64 l_begin = static_cast<uint64>((FE::iterator_cast<const CharT* const>(l_iterator) - string_p) + 1);
        return std::make_optional(range{ l_begin, l_begin + l_target_substr_length });
    }
    return std::nullopt;
}




template<typename CharT>
FE::boolean compare_except_for(const CharT* lhs_p, const CharT* rhs_p, std::initializer_list<const CharT*> to_ignore_p) noexcept
{
    return false;
}




template<typename CharT, typename IntT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ IntT char_to_integer(const CharT value_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_STATIC_ASSERT(::std::is_integral<IntT>::value == false, "typename IntT is not an integer");

    return static_cast<IntT>(value_p) - 48;
}




template <typename CharT, typename IntT>
_FE_NODISCARD_ _FE_CONSTEXPR17_ IntT string_to_integer(const CharT* string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(string_p));

    var::int16 l_sign = (*string_p == static_cast<const CharT>('-')) ? -1 : 1;

    while ((*string_p >= static_cast<const CharT>('0')) && (*string_p <= static_cast<const CharT>('9')) && (*string_p != null))
    {
        ++string_p;
    }
    --string_p;

    IntT l_result = 0;
    IntT l_multiplier = 1;
    while ((*string_p >= static_cast<const CharT>('0')) && (*string_p <= static_cast<const CharT>('9')) && (*string_p != null))
    {
        l_result += static_cast<IntT>(*string_p - static_cast<const CharT>('0')) * l_multiplier;
        l_multiplier *= 10;
        --string_p;
    }
    return l_sign * l_result;
}




template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ void invert(CharT* const in_out_string_p) noexcept
{
    FE_NEGATIVE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");
    FE_NEGATIVE_ASSERT(in_out_string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(in_out_string_p));

    var::uint64 l_start = 0;
    var::uint64 l_end = length(in_out_string_p);

    CharT l_temporary = null;
    while (l_start < l_end)
    {
        l_temporary = in_out_string_p[l_start];
        --l_end;
        in_out_string_p[l_start] = in_out_string_p[l_end];
        in_out_string_p[l_end] = l_temporary;
        ++l_start;
    }
}




template<typename CharT>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::uint64 space_insensitive_length(const CharT* const string_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == true, "CharT is not a valid character type");
    FE_ASSERT(string_p != nullptr, "Assertion failure: the string is a nullptr.");

    const CharT* l_string = string_p;
    var::uint64 l_string_len = 0;

    while (*l_string != '\0')
    {
        if (*l_string != ' ')
        {
            ++l_string_len;
        }
        ++l_string;
    }

    return l_string_len;
}




template<typename CharT> // This is an alpha version of the implementation. It need some adjustments. 
_FE_CONSTEXPR17_ FE::boolean space_insensitive_comparison(const CharT* const lhs_p, FE::size lhs_len_p, const CharT* const rhs_p, FE::size rhs_len_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == true, "CharT is not a valid character type");
    FE_ASSERT(lhs_p != nullptr, "Assertion failure: the left hand string is a nullptr.");
    FE_ASSERT(lhs_len_p > 0, "Assertion failure: the left hand string length is zero.");

    FE_ASSERT(rhs_p != nullptr, "Assertion failure: the left hand string is a nullptr.");
    FE_ASSERT(rhs_len_p > 0, "Assertion failure: the right hand string length is zero.");


    const CharT* l_rhs_iter = rhs_p;
    const CharT* const l_rhs_end = rhs_p + rhs_len_p;

    const CharT* l_lhs_iter = lhs_p;
    const CharT* const l_lhs_end = lhs_p + lhs_len_p;

    while ((l_lhs_iter != l_lhs_end) && (l_rhs_iter != l_rhs_end))
    {
        switch (*l_lhs_iter)
        {
        case ' ':
            _FE_FALLTHROUGH_;
        case '\a':
            _FE_FALLTHROUGH_;
        case '\b':
            _FE_FALLTHROUGH_;
        case '\f':
            _FE_FALLTHROUGH_;
        case '\n':
            _FE_FALLTHROUGH_;
        case '\r':
            _FE_FALLTHROUGH_;
        case '\t':
            _FE_FALLTHROUGH_;
        case '\v':
            _FE_FALLTHROUGH_;
        case '\0':
            ++l_lhs_iter;
            continue;

        default:
            break;
        }

        switch (*l_rhs_iter)
        {
        case ' ':
            _FE_FALLTHROUGH_;
        case '\a':
            _FE_FALLTHROUGH_;
        case '\b':
            _FE_FALLTHROUGH_;
        case '\f':
            _FE_FALLTHROUGH_;
        case '\n':
            _FE_FALLTHROUGH_;
        case '\r':
            _FE_FALLTHROUGH_;
        case '\t':
            _FE_FALLTHROUGH_;
        case '\v':
            _FE_FALLTHROUGH_;
        case '\0':
            ++l_rhs_iter;
            continue;

        default:
            break;
        }

        if (*l_lhs_iter != *l_rhs_iter)
        {
            return false;
        }

        ++l_lhs_iter;
        ++l_rhs_iter;
    }

    return true;
}




template<typename CharT>
_FE_CONSTEXPR17_ FE::boolean space_insensitive_contains(const CharT* const str_p, const std::size_t str_len_p, const CharT* const target_substr_p) noexcept
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == true, "CharT is not a valid character type");
    FE_ASSERT(str_p != nullptr, "Assertion failure: the string is a nullptr.");
    FE_ASSERT(str_len_p > 0, "Assertion failure: the string length is zero.");

    FE_ASSERT(target_substr_p != nullptr, "Assertion failure: the string is a nullptr.");


    const CharT* l_str_iter = str_p;
    const CharT* const l_str_end = str_p + str_len_p;

    const CharT* l_target_substr = target_substr_p;

    while ((l_str_iter != l_str_end) && (*l_target_substr != '\0'))
    {
        switch (*l_str_iter)
        {
        case ' ':
            _FE_FALLTHROUGH_;
        case '\a':
            _FE_FALLTHROUGH_;
        case '\b':
            _FE_FALLTHROUGH_;
        case '\f':
            _FE_FALLTHROUGH_;
        case '\n':
            _FE_FALLTHROUGH_;
        case '\r':
            _FE_FALLTHROUGH_;
        case '\t':
            _FE_FALLTHROUGH_;
        case '\v':
            _FE_FALLTHROUGH_;
        case '\0':
            ++l_str_iter;
            continue;

        default:
            break;
        }

        switch (*l_target_substr)
        {
        case ' ':
            _FE_FALLTHROUGH_;
		case '\a':
            _FE_FALLTHROUGH_;
		case '\b':
			_FE_FALLTHROUGH_;
		case '\f':
			_FE_FALLTHROUGH_;
		case '\n':
			_FE_FALLTHROUGH_;
		case '\r':
			_FE_FALLTHROUGH_;
		case '\t':
			_FE_FALLTHROUGH_;
		case '\v':
            _FE_FALLTHROUGH_;
		case '\0':
            ++l_target_substr;
            continue;

        default:
            break;
        }

        if (*l_str_iter == *l_target_substr)
        {
            ++l_target_substr;
        }
        else
        {
            l_target_substr = target_substr_p;
        }

        ++l_str_iter;
    }

    if (*l_target_substr == '\0')
    {
        return true;
    }

    return false;
}




/*
The murmurhash implementation is auto generated by copliot.
*/
template<typename CharT>
_FE_CONSTEXPR17_ FE::uint64 hash(const CharT* string_p, FE::uint64 length_p)
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == true, "CharT is not a valid character type");
    //FE_ASSERT(string_p != nullptr, "Assertion failure: the string is a nullptr.");
    //FE_ASSERT(length_p > 0, "Assertion failure: the string length is zero.");

    FE::uint64 l_number_of_blocks = static_cast<FE::uint64>(length_p / 4);
    var::uint64 l_h1 = 0; // seed
    FE::uint64 l_c1 = 0xcc9e2d51;
    FE::uint64 l_c2 = 0x1b873593;

    // Body
    const CharT* L_blocks = string_p + (l_number_of_blocks * 4);
    for (var::int64 i = -static_cast<var::int64>(l_number_of_blocks); i != 0; ++i)
    {
        var::uint64 l_k1 = L_blocks[i];

        l_k1 *= l_c1;
        l_k1 = (l_k1 << 15) | (l_k1 >> (32 - 15));
        l_k1 *= l_c2;

        l_h1 ^= l_k1;
        l_h1 = (l_h1 << 13) | (l_h1 >> (32 - 13));
        l_h1 = l_h1 * 5 + 0xe6546b64;
    }

    // Tail
    const CharT* l_tail = string_p + l_number_of_blocks * 4;
    var::int64 l_k1 = 0;

    switch (length_p & 3)
    {
    case 3:
        l_k1 ^= l_tail[2] << 16;
    case 2:
        l_k1 ^= l_tail[1] << 8;
    case 1:
        l_k1 ^= l_tail[0];
        l_k1 *= l_c1;
        l_k1 = (l_k1 << 15) | (l_k1 >> (32 - 15));
        l_k1 *= l_c2;
        l_h1 ^= l_k1;
    }

    // Finalization
    l_h1 ^= length_p;
    l_h1 ^= l_h1 >> 16;
    l_h1 *= 0x85ebca6b;
    l_h1 ^= l_h1 >> 13;
    l_h1 *= 0xc2b2ae35;
    l_h1 ^= l_h1 >> 16;

    return l_h1;
}


template<typename UTF>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ UTF* skip_BOM(UTF* string_p) noexcept
{
	FE_STATIC_ASSERT(((std::is_same_v<UTF, var::UTF8> == true) || (std::is_same_v<UTF, FE::UTF8> == true) 
                    || (std::is_same_v<UTF, var::UTF16> == true) || (std::is_same_v<UTF, FE::UTF16> == true)
		|| (std::is_same_v<UTF, var::UTF32> == true) || (std::is_same_v<UTF, FE::UTF32> == true)), 
        "the character type is not a valid UTF type");
    
    var::uint32 l_bom_size = 0;
	
    if constexpr ((std::is_same_v<UTF, var::UTF8> == true) || (std::is_same_v<UTF, FE::UTF8> == true))
	{
		l_bom_size = 3;
	}
	else if constexpr ((std::is_same_v<UTF, var::UTF16> == true) || (std::is_same_v<UTF, FE::UTF16> == true))
	{
		l_bom_size = 2;
	}
	else if constexpr ((std::is_same_v<UTF, var::UTF32> == true) || (std::is_same_v<UTF, FE::UTF32> == true))
	{
		l_bom_size = 4;
	}

    if constexpr (std::is_const_v<UTF> == true)
    {
        return reinterpret_cast<const UTF*>( reinterpret_cast<const char*>(string_p) + l_bom_size );
    }
    else
    {
        return reinterpret_cast<UTF*>( reinterpret_cast<char*>(string_p) + l_bom_size );
    } 
}


END_NAMESPACE
#pragma warning(pop)
#endif