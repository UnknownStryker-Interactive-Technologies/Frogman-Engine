#ifndef _FE_CORE_CHAR_TRAITS_HXX_
#define _FE_CORE_CHAR_TRAITS_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/string.hxx>
#include <FE/core/memory.hxx>




BEGIN_NAMESPACE(FE)


template<typename CharT>
struct null_escape_size
{
    _MAYBE_UNUSED_ constexpr static uint8 value = sizeof(CharT);
};


template<typename CharT>
struct string_info
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

    CharT* _string_pointer;
    length_t _length;
    capacity_t _capacity;
};


template<typename CharT>
class char_traits
{
    FE_STATIC_ASSERT(std::is_const<CharT>::value == true, "static assertion failed: the template argument CharT cannot be const.");
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

public:
    using value_type = CharT;

    template<ADDRESS DestAddressAlignment = ADDRESS::_NOT_ALIGNED, ADDRESS SourceAddressAlignment = ADDRESS::_NOT_ALIGNED>
    _FORCE_INLINE_ static void copy(CharT* const out_dest_string_p, const CharT* const source_string_p, size_t count_p) noexcept
    {
        algorithm::string::copy<CharT, DestAddressAlignment, SourceAddressAlignment>(out_dest_string_p, source_string_p, count_p);
    }

	_FORCE_INLINE_ static void assign(CharT* in_out_string_p, size_t count_p, CharT value_p) noexcept
	{
        FE_ASSERT(in_out_string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p));
        FE_ASSERT(count_p == 0, "${%s@0}: {%s@1} is zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_p));

        for (var::size_t i = 0; i != count_p; ++i)
        {
            *in_out_string_p = value_p;
            ++in_out_string_p;
        }
        *in_out_string_p = _NULL_;
	}

    _FORCE_INLINE_ static void assign(string_info<CharT> in_out_string_p, const CharT* const string_p, size_t input_begin_p, size_t input_end_p) noexcept
    {
        FE_ASSERT(in_out_string_p._string_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p._string_pointer));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than or equal to ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(input_begin_p), TO_STRING(input_end_p));

        size_t l_size_to_copy = input_end_p - input_begin_p;
        FE_ASSERT(l_size_to_copy > in_out_string_p._capacity, "${%s@0}: ${%s@2} was greater than or equal to ${%s@1}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(in_out_string_p._capacity), TO_STRING(l_size_to_copy), &in_out_string_p._capacity, &l_size_to_copy);

        algorithm::string::copy<CharT>(in_out_string_p._string_pointer, string_p + input_begin_p, l_size_to_copy);
    }

    _FORCE_INLINE_ static void assign(string_info<CharT> in_out_string_p, const CharT* const string_p, size_t size_to_assign_p) noexcept
    {
        FE_ASSERT(size_to_assign_p == 0, "${%s@0}: {%s@1} is zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(size_to_assign_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(size_to_assign_p > in_out_string_p._capacity, "${%s@0}: ${%s@2} was greater than ${%s@1}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(in_out_string_p._capacity), TO_STRING(size_to_assign_p), &in_out_string_p._capacity, &size_to_assign_p);

        algorithm::string::copy<CharT>(in_out_string_p._string_pointer, string_p, size_to_assign_p);
    }

    template<class ConstIterator>
    _FORCE_INLINE_ static void assign(string_info<CharT> in_out_string_p, ConstIterator input_begin_p, ConstIterator input_end_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename ConstIterator::value_type>::type, typename std::remove_const<CharT>::type>::value == false), "static assertion failed: const_iterator::value_type is not equal to CharT.");
        FE_STATIC_ASSERT((std::is_class<ConstIterator>::value == false), "static assertion failed: the template argument const_iterator must be a class or a struct type.");

        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_end_p));

        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} must be smaller than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(input_begin_p), TO_STRING(input_end_p));

        size_t l_size_to_assign = input_end_p - input_begin_p;
        FE_ASSERT(l_size_to_assign > in_out_string_p._capacity, "${%s@0}: input string range length exceeds the string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == true)
        {
            algorithm::string::copy<CharT>(in_out_string_p._string_pointer, input_begin_p.operator->(), l_size_to_assign);
            in_out_string_p._string_pointer[l_size_to_assign] = _NULL_;
        }
        else if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == false)
        {
            const CharT* const l_end_ptrc = in_out_string_p._string_pointer + l_size_to_assign;
            while (in_out_string_p._string_pointer != l_end_ptrc)
            {
                *in_out_string_p._string_pointer = *input_begin_p;
                ++in_out_string_p._string_pointer;
                ++input_begin_p;
            }
            *in_out_string_p._string_pointer = _NULL_;
        }
    }


    _FORCE_INLINE_ static void insert(string_info<CharT> in_out_string_p, size_t position_p, size_t count_p, CharT value_p) noexcept
    {
        FE_ASSERT(in_out_string_p._string_pointer == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p._string_pointer));
        FE_ASSERT(count_p == 0, "ERROR: insert() operation was unsuccessful. ${%s@0} was zero.", TO_STRING(count_p));
 
        std::memmove(in_out_string_p._string_pointer + (position_p + count_p), in_out_string_p._string_pointer + position_p, (in_out_string_p._length - position_p) * sizeof(CharT) + FE::null_escape_size<CharT>::value);

        in_out_string_p._string_pointer += position_p;
        for (var::length_t i = 0; i != count_p; ++i)
        {
            *in_out_string_p._string_pointer = value_p;
            ++in_out_string_p._string_pointer;
        }

        in_out_string_p._string_pointer[in_out_string_p._length] = _NULL_;
    }

    _FORCE_INLINE_ static void insert(string_info<CharT> in_out_string_p, size_t this_position_p, const CharT* const string_p, size_t input_begin_p, size_t input_end_p) noexcept
    {
        FE_ASSERT(this_position_p > in_out_string_p._capacity, "${%s@0}: ${%s@1} cannot be greater than the ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(this_position_p), TO_STRING(in_out_string_p._capacity));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than or equal to ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(input_begin_p), TO_STRING(input_end_p));

        size_t l_input_string_length = input_end_p - input_begin_p;
        FE_ASSERT((in_out_string_p._length + l_input_string_length) > in_out_string_p._capacity, "${%s@0}: string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        std::memmove(in_out_string_p._string_pointer + (this_position_p + l_input_string_length), in_out_string_p._string_pointer + this_position_p, (in_out_string_p._length - this_position_p) * sizeof(CharT));
        std::memcpy(in_out_string_p._string_pointer + this_position_p, string_p + input_begin_p, l_input_string_length * sizeof(CharT));

        in_out_string_p._string_pointer[in_out_string_p._length + l_input_string_length] = _NULL_;
    }

    _FORCE_INLINE_ static void insert(string_info<CharT> in_out_string_p, size_t this_position_p, const CharT* const string_p, size_t inout_string_length_p) noexcept
    {
        FE_ASSERT(this_position_p >= in_out_string_p._capacity, "${%s@0}: ${%s@1} cannot be greater than the string capacity.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this_position_p));
        FE_ASSERT(in_out_string_p._string_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(in_out_string_p));

        std::memmove(in_out_string_p._string_pointer + (this_position_p + inout_string_length_p), in_out_string_p._string_pointer + this_position_p, (in_out_string_p._length - this_position_p) * sizeof(CharT));
        std::memcpy(in_out_string_p._string_pointer + this_position_p, string_p, inout_string_length_p * sizeof(CharT));

        in_out_string_p._string_pointer[in_out_string_p._length + inout_string_length_p] = _NULL_;
    }

    _FORCE_INLINE_ static void insert(string_info<CharT> in_out_string_p, size_t this_position_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: initializer_list_p is empty.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE));
        FE_ASSERT((in_out_string_p._length + initializer_list_p.size()) >= in_out_string_p._capacity, "${%s@0}: string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        std::memmove(in_out_string_p._string_pointer + (this_position_p + initializer_list_p.size()), in_out_string_p._string_pointer + this_position_p, (in_out_string_p._length - this_position_p) * sizeof(CharT));
        std::memset(in_out_string_p._string_pointer + this_position_p, _NULL_, initializer_list_p.size() * sizeof(CharT));
        algorithm::string::concatenate(in_out_string_p._string_pointer + this_position_p, in_out_string_p._capacity - this_position_p, std::move(initializer_list_p));
        in_out_string_p._string_pointer[in_out_string_p._length + initializer_list_p.size()] = _NULL_;
    }

    _FORCE_INLINE_ static void erase(string_info<CharT> in_out_string_p, size_t this_index_p = 0, size_t this_count_p = 1) noexcept
    {
        FE_ASSERT(in_out_string_p._capacity < (this_index_p + this_count_p), "${%s@0}: fixed sized string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        FE_ASSERT(this_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(this_count_p));

        std::memmove(in_out_string_p._string_pointer + this_index_p, in_out_string_p._string_pointer + (this_index_p + this_count_p), (in_out_string_p._length - this_index_p) * sizeof(CharT));
        in_out_string_p._string_pointer[in_out_string_p._length - this_count_p] = _NULL_;
    }

    _FORCE_INLINE_ static void append(string_info<CharT> in_out_string_p, size_t input_count_p, CharT value_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(in_out_string_p._capacity < (in_out_string_p._length + input_count_p), "${%s0}: cannot append ${%ld@1} character(s) to the string.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), &input_count_p);

        in_out_string_p._string_pointer += in_out_string_p._length;
        const CharT* const l_end_ptr = in_out_string_p._string_pointer + input_count_p;

        while (in_out_string_p._string_pointer != l_end_ptr)
        {
            *in_out_string_p._string_pointer = value_p;
            ++in_out_string_p._string_pointer;
        }

        *in_out_string_p._string_pointer = _NULL_;
    }

    _FORCE_INLINE_ static void append(string_info<CharT> in_out_string_p, const CharT* const string_p, size_t input_position_p, size_t input_count_p = 1) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(in_out_string_p._capacity < (in_out_string_p._length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        in_out_string_p._string_pointer += in_out_string_p._length;
        algorithm::string::copy<CharT>(in_out_string_p._string_pointer, string_p + input_position_p, input_count_p);
    }

    _FORCE_INLINE_ static void append(string_info<CharT> in_out_string_p, const CharT* string_p, size_t input_count_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(in_out_string_p._capacity < (in_out_string_p._length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        
        in_out_string_p._string_pointer += in_out_string_p._length;
        algorithm::string::copy<CharT>(in_out_string_p._string_pointer, string_p, input_count_p);
        return *this;
    }

    template<class ConstIterator>
    _FORCE_INLINE_ static void append(string_info<CharT> in_out_string_p, ConstIterator input_begin_p, ConstIterator input_end_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_class<ConstIterator>::value == false), "static assertion failed: the template argument const_iterator must be a class or a struct type.");
        FE_STATIC_ASSERT((std::is_same<ConstIterator::value_type, CharT>::value == false), "static assertion failed: const_iterator::value_type is not equal to CharT.");
        
        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_end_p));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@0} must not be greater than input_count_p.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(input_begin_p));
 
        size_t l_input_size = input_end_p - input_begin_p;
        FE_ASSERT(l_input_size + in_out_string_p._length >= in_out_string_p._capacity, "${%s@0}: input string range length exceeds fixed sized string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        in_out_string_p._string_pointer += in_out_string_p._length;

        if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == true)
        {
            algorithm::string::copy<CharT>(in_out_string_p._string_pointer, input_begin_p.operator->(), l_input_size);
        }
        else if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == false)
        {
            const CharT* const l_end_ptrc = in_out_string_p._string_pointer + l_input_size;

            while (in_out_string_p._string_pointer != l_end_ptrc)
            {
                *in_out_string_p._string_pointer = *input_begin_p;
                ++in_out_string_p._string_pointer;
                ++input_begin_p;
            }
            *in_out_string_p._string_pointer = _NULL_;
        }
    }


    _FORCE_INLINE_ static void replace(string_info<CharT> in_out_string_p, size_t this_position_p, size_t this_count_to_replace_p, const CharT* const string_p, size_t input_count_p) noexcept
    {
        FE_ASSERT(((in_out_string_p._length + input_count_p) - this_count_to_replace_p) > in_out_string_p._capacity, "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(this_position_p > in_out_string_p._length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this_position_p), TO_STRING(in_out_string_p._length));
        FE_ASSERT(this_count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(this_count_to_replace_p));

        in_out_string_p._string_pointer += this_position_p;
        std::memmove(in_out_string_p._string_pointer + input_count_p, in_out_string_p._string_pointer + this_count_to_replace_p, (in_out_string_p._length - this_count_to_replace_p) * sizeof(CharT));
        std::memcpy(in_out_string_p._string_pointer, string_p, input_count_p * sizeof(CharT));
        in_out_string_p._string_pointer -= this_position_p;
        in_out_string_p._string_pointer[(in_out_string_p._length + input_count_p) - this_count_to_replace_p] = _NULL_;
    }

    _FORCE_INLINE_ static void replace(string_info<CharT> in_out_string_p, size_t this_position_p, size_t this_count_to_replace_p, const CharT value_p, size_t input_count_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(((in_out_string_p._length + input_count_p) - this_count_to_replace_p) > in_out_string_p._capacity, "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(this_count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(this_count_to_replace_p));

        in_out_string_p._string_pointer += this_position_p;
        std::memmove(in_out_string_p._string_pointer + input_count_p, in_out_string_p._string_pointer + this_count_to_replace_p, (in_out_string_p._length - this_count_to_replace_p) * sizeof(CharT));

        const CharT* const l_end_ptrc = in_out_string_p._string_pointer + input_count_p;

        while (in_out_string_p._string_pointer != l_end_ptrc)
        {
            *in_out_string_p._string_pointer = value_p;
            ++in_out_string_p._string_pointer;
        }

        in_out_string_p._string_pointer -= this_position_p;
        in_out_string_p._string_pointer[(in_out_string_p._length + input_count_p) - this_count_to_replace_p] = _NULL_;
    }

    _FORCE_INLINE_ static void replace(string_info<CharT> in_out_string_p, size_t this_position_p, size_t this_count_to_replace_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        size_t l_input_size = initializer_list_p.size();

        FE_ASSERT(((in_out_string_p._length + l_input_size) - this_count_to_replace_p) > in_out_string_p._capacity, "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(this_count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(this_count_to_replace_p));
        FE_ASSERT(l_input_size == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(initializer_list_p.size()));

        std::memmove(in_out_string_p._string_pointer + this_position_p + l_input_size, in_out_string_p._string_pointer + this_position_p + this_count_to_replace_p, (in_out_string_p._length - (this_position_p + this_count_to_replace_p)) * sizeof(CharT));
        std::memcpy(in_out_string_p._string_pointer + this_position_p, initializer_list_p.begin(), l_input_size * sizeof(CharT));
        in_out_string_p._string_pointer[(in_out_string_p._length + l_input_size) - this_count_to_replace_p] = _NULL_;
    }

    template<class ConstIterator>
    _FORCE_INLINE_ static void replace(string_info<CharT> in_out_string_p, size_t first_index_p, size_t last_index_p, ConstIterator input_first_p, ConstIterator input_last_p) noexcept
    {
        FE_ASSERT(input_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_first_p));
        FE_ASSERT(input_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_last_p));
        FE_ASSERT(input_first_p > input_last_p, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_first_p), TO_STRING(input_last_p));
        FE_ASSERT(first_index_p > last_index_p, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(first_index_p), TO_STRING(last_index_p));
     
        size_t l_input_size = input_last_p - input_first_p;
        size_t l_this_count_to_replace = last_index_p - first_index_p;

        std::memmove(in_out_string_p._string_pointer + first_index_p + l_input_size, in_out_string_p._string_pointer + first_index_p + l_this_count_to_replace, (in_out_string_p._length - (first_index_p + l_this_count_to_replace)) * sizeof(CharT));
        
        if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == true)
        {
            std::memcpy(in_out_string_p._string_pointer + first_index_p, input_first_p.operator->(), l_input_size * sizeof(CharT));
            in_out_string_p._string_pointer[(in_out_string_p._length + l_input_size) - l_this_count_to_replace] = _NULL_;
        }
        else if constexpr (std::is_same<ConstIterator::iterator_category, FE::contiguous_iterator<CharT>::category>::value == false)
        {
            const CharT* const l_end_ptrc = in_out_string_p._string_pointer + l_input_size;
            in_out_string_p._string_pointer += first_index_p;

            while (in_out_string_p._string_pointer != l_end_ptrc)
            {
                *in_out_string_p._string_pointer = *input_first_p;
                ++in_out_string_p._string_pointer;
                ++input_first_p;
            }
            *in_out_string_p._string_pointer = _NULL_;
        }
    }
};

END_NAMESPACE
#endif // 