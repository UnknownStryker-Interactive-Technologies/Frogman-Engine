#ifndef _FE_CORE_FSTRING_HXX_
#define _FE_CORE_FSTRING_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/char_traits.hxx>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/iterator.hxx>
#include <FE/core/memory.hxx>
#pragma warning (push)
#pragma warning (disable: 6386)
#pragma warning (disable: 26800)
#pragma warning (disable: 26495)




BEGIN_NAMESPACE(FE)


template<typename CharT, uint64 Capacity, class Traits = FE::char_traits<CharT>>
class fixed_sized_string final
{
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a valid character type.");
    FE_STATIC_ASSERT(std::is_class<Traits>::value == false, "static assertion failed: the template argument traits is not a class or a struct type.");
    FE_STATIC_ASSERT((std::is_same<CharT, Traits::value_type>::value == false), "static assertion failed: enforcing traits' value_type to be equivalent to CharT. The template parameter CharT must be identical to traits::value_type.");


public:
    using traits_type = Traits;
    using value_type = CharT;
    using size_type = var::uint64;
    using length_type = var::uint64;
    using difference_type = var::ptrdiff_t;
    using reference = CharT&;
    using const_reference = const CharT&;
    using pointer = CharT*;
    using const_pointer = const CharT*;
    using iterator = FE::iterator<FE::contiguous_iterator<CharT>>;
    using const_iterator = FE::const_iterator<FE::contiguous_iterator<CharT>>;
    using reverse_iterator = FE::reverse_iterator<FE::contiguous_iterator<CharT>>;
    using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<CharT>>;


private:
    CharT m_fstring[Capacity];
    length_type m_length;


public:
    _CONSTEXPR20_ fixed_sized_string() noexcept : m_fstring{ _NULL_ }, m_length() {}

    _CONSTEXPR20_ fixed_sized_string(const value_type* const string_p) noexcept : m_fstring{ _NULL_ }, m_length(algorithm::string::length<CharT>(string_p))
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_length >= Capacity, "${%s@0}: the input string length exceeds the max string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        Traits::copy( this->m_fstring, string_p, this->m_length);
    }

    _CONSTEXPR20_ fixed_sized_string(const fixed_sized_string& other_p) noexcept : m_fstring{ _NULL_ }, m_length(other_p.m_length)
    {
        if (other_p.m_length == 0)
        {
            return;
        }

        Traits::copy(this->m_fstring, other_p.m_fstring, Capacity);
    }

    template<class InputIterator>
    _CONSTEXPR20_ fixed_sized_string(InputIterator first_p, InputIterator last_p) noexcept
    {
        FE_STATIC_ASSERT(std::is_class<InputIterator>::value == false, "Static Assertion Failure: The template argument InputIterator must be a class or a struct type.");
        FE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value == false), "Static Assertion Failure: InputIterator's value_type has to be the same as fixed_sized_string's value_type.");

        FE_ASSERT(first_p >= last_p, "${%s@0}: The input iterator ${%s@1} must not be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(first_p), TO_STRING(last_p));
        FE_ASSERT((last_p - first_p) > Capacity, "${%s@0}: The input size exceeds the fixed_sized_string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        Traits::assign(string_info<CharT>{ this->m_fstring, length_t{static_cast<length_t>(last_p - first_p)}, capacity_t{Capacity} }, first_p, last_p);
    }

    _CONSTEXPR20_ fixed_sized_string(fixed_sized_string&& rvalue_p) noexcept : m_fstring{ _NULL_ }, m_length(rvalue_p.m_length)
    {
        Traits::copy(this->m_fstring, rvalue_p.m_fstring, Capacity);
        *(rvalue_p.m_fstring) = _NULL_;
        rvalue_p.m_length = 0;
    }


    _CONSTEXPR20_ fixed_sized_string& operator=(const value_type* const string_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

        this->m_length = algorithm::string::length<CharT>(string_p);
        FE_ASSERT(this->m_length >= Capacity, "${%s@0}: the input string length exceeds the max string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        Traits::copy(this->m_fstring, string_p, this->m_length);
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& operator=(const fixed_sized_string& other_p) noexcept
    {
        if (other_p.m_length == 0)
        {
            return *this;
        }

        this->m_length = other_p.m_length;
        Traits::copy(this->m_fstring, other_p.m_fstring, Capacity);
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& operator=(fixed_sized_string&& rvalue_p) noexcept
    {
        if (rvalue_p.m_length == 0)
        {
            return *this;
        }

        Traits::copy(this->m_fstring, rvalue_p.m_fstring, Capacity);
        *(rvalue_p.m_fstring) = _NULL_;
        this->m_length = rvalue_p.m_length;
        rvalue_p.m_length = 0;
        return *this;
    }

    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ CharT& operator[](index_t index_p) noexcept
    {
        FE_ASSERT(index_p >= this->m_length, "${%s@0}: index out of boundary.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        return this->m_fstring[index_p];
    }


    _CONSTEXPR20_ fixed_sized_string& assign(const size_type count_p, const value_type value_p) noexcept
    {
        FE_ASSERT(count_p >= Capacity, "${%s@0}: ${%s@1} exceeds fixed sized string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(count_p));
        Traits::assign(this->m_fstring, count_p, value_p);
        this->m_length = count_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& assign(const fixed_sized_string& other_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than or equal to ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(input_begin_p), TO_STRING(input_end_p));

        if (other_p.m_length == 0)
        {
            return *this;
        }
    
        return this->assign(other_p.m_fstring, input_begin_p, input_end_p);
    }

    _CONSTEXPR20_ fixed_sized_string& assign(const value_type* const string_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        Traits::assign(string_info<CharT>{this->m_fstring, _NULL_, Capacity}, string_p, input_begin_p, input_end_p);
        this->m_length = input_end_p - input_begin_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& assign(const value_type* const string_p, const size_type size_to_assign_p) noexcept
    {
        FE_ASSERT(size_to_assign_p == 0, "${%s@0}: {%s@1} is zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(size_to_assign_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(size_to_assign_p >= Capacity, "ERROR: size_to_assign_p exceeds fixed sized string capacity.");
       
        Traits::assign(string_info<CharT>{this->m_fstring, _NULL_, Capacity}, string_p, size_to_assign_p);
        this->m_length = size_to_assign_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& assign(const_iterator input_begin_p, const_iterator input_end_p) noexcept
    {
        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_end_p));

        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} must be smaller than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT((input_end_p - input_begin_p) >= Capacity, "${%s@0}: input string range length exceeds fixed sized string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        
        Traits::assign(string_info<CharT>{this->m_fstring, _NULL_, Capacity}, input_begin_p, input_end_p);
        this->m_length = input_end_p - input_begin_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string&  assign(std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() >= Capacity, "ERROR: initializer_list_p.size() exceeds the string capacity.");
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: {%s@1} is an empty initializer_list_p.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(initializer_list_p));
       
        algorithm::string::concatenate(this->m_fstring, Capacity, std::move(initializer_list_p));
        this->m_length = initializer_list_p.size();
        this->m_fstring[this->m_length] = _NULL_;
        return *this;
    }


    _FORCE_INLINE_ fixed_sized_string& assign_range(const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p) noexcept
    {
        return this->assign(*input_iterator_begin_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& assign_range(const fixed_sized_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        return this->assign(other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& assign_range(const value_type* const string_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return this->assign(string_p, input_string_range_p._begin, input_string_range_p._end);
    }


    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ CharT front() const noexcept { return *this->m_fstring; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ CharT back() const noexcept { return this->m_fstring[this->m_length - 1]; }
    _NODISCARD_  _CONSTEXPR20_ _FORCE_INLINE_ CharT* data() const noexcept { return const_cast<CharT*>(this->m_fstring); }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const CharT* c_str() const noexcept { return this->m_fstring; }


    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ iterator begin() noexcept { return this->m_fstring; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ iterator end() noexcept { return this->m_fstring + this->m_length; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ reverse_iterator rbegin() const noexcept { return this->m_fstring + (this->m_length - 1); }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ reverse_iterator rend() const noexcept { return this->m_fstring - 1; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_iterator cbegin() noexcept { return this->m_fstring; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_iterator cend() noexcept { return this->m_fstring + this->m_length; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept { return this->m_fstring + (this->m_length - 1); }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_reverse_iterator crend() const noexcept { return this->m_fstring - 1; }

    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ boolean is_empty() const noexcept { return (this->m_length == 0) ? true : false; }

    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ length_type length() const noexcept { return this->m_length; }
    _NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ length_type max_length() const noexcept { return Capacity; }
    _NODISCARD_ _FORCE_INLINE_ size_type capacity() const noexcept { return Capacity; }

    _CONSTEXPR20_ _FORCE_INLINE_ void clear() noexcept 
    {
        this->m_fstring[0] = _NULL_;
        this->m_length = 0;
    }
   

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, const size_type count_p, value_type value_p) noexcept
    {
        FE_ASSERT(position_p >= Capacity, "ERROR: position_p cannot be greater than the string capacity.");
        FE_ASSERT(count_p == 0, "ERROR: insert() operation was not successful. size_type count_p was zero.");
        FE_ASSERT((this->m_length + count_p) >= Capacity, "ERROR: fixed sized string capacity overflowed.");

        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_p, value_p);
        this->m_length += count_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, const value_type* const string_p) noexcept
    {
        FE_ASSERT(position_p >= Capacity, "${%s@0}: position_p cannot be greater than the string capacity.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        FE_ASSERT(string_p == nullptr, "${%s@0}: the input string is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
        
        size_type l_inout_string_length = algorithm::string::length(string_p);
        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, string_p, l_inout_string_length);
        this->m_length += l_inout_string_length;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, const fixed_sized_string& other_p) noexcept
    {
        FE_ASSERT(position_p >= Capacity, "${%s@0}: ${%s@1} cannot be greater than the ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(position_p), TO_STRING(Capacity));
        FE_ASSERT((this->m_length + other_p.m_length) >= Capacity, "${%s@0}: fixed sized string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, other_p.m_fstring, other_p.m_length);
        this->m_length += other_p.m_length;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, const fixed_sized_string& other_p, const size_type input_begin_p, size_type input_end_p) noexcept
    {
        FE_ASSERT(position_p >= Capacity, "${%s@0}: ${%s@1} cannot be greater than the ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(position_p), TO_STRING(Capacity));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(input_begin_p), TO_STRING(input_end_p));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, other_p.m_fstring, input_begin_p, input_end_p);
        this->m_length += input_end_p - input_begin_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, const value_type* const string_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        FE_ASSERT(position_p >= Capacity, "${%s@0}: ${%s@1} cannot be greater than the ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(position_p), TO_STRING(Capacity));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, string_p, input_begin_p, input_end_p);
        this->m_length += input_end_p - input_begin_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& insert(const size_type position_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: initializer_list_p is empty.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE));
        FE_ASSERT((this->m_length + initializer_list_p.size()) >= Capacity, "${%s@0}: fixed sized string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        Traits::insert(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, std::move(initializer_list_p));
        this->m_length += initializer_list_p.size();
        return *this;
    }


    _FORCE_INLINE_ fixed_sized_string& insert_range(const size_type position_p, const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        return this->insert(position_p, *input_iterator_begin_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& insert_range(const size_type position_p, const fixed_sized_string& other_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        return this->insert(position_p, other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& insert_range(const size_type position_p, const value_type* const string_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        return this->insert(position_p, string_p, input_string_range_p._begin, input_string_range_p._end);
    }
    

    _CONSTEXPR20_ fixed_sized_string& erase(const size_type index_p = 0, const size_type count_p = 1) noexcept
    {
        FE_ASSERT(Capacity < (index_p + count_p), "${%s@0}: fixed sized string capacity overflowed.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        FE_ASSERT(count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_p));

        Traits::erase(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, index_p, count_p);
        this->m_length -= count_p;
        return *this;
    }


    _CONSTEXPR20_ void push_back(const value_type value_p) noexcept
    {
        FE_ASSERT(this->m_length >= Capacity, "${%s0}: out of capacity", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        this->m_fstring[this->m_length] = value_p;
        ++this->m_length;
        this->m_fstring[this->m_length] = _NULL_;
    }

    _CONSTEXPR20_ CharT pop_back() noexcept
    {
        FE_ASSERT(this->m_length == 0, "${%s0}: Unable to pop an empty fstring", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        const CharT l_return_value = this->back();
        --this->m_length;
        this->m_fstring[this->m_length] = _NULL_;
        return l_return_value;
    }


    _CONSTEXPR20_ fixed_sized_string& append(const size_type input_count_p, const value_type value_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->max_length() < (this->m_length + input_count_p), "${%s0}: cannot append ${%ld@1} character(s) to the fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), &input_count_p);

        Traits::append(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, input_count_p, value_p);
        this->m_length += input_count_p;
        return *this;
    }
    

    _CONSTEXPR20_ fixed_sized_string& append(const fixed_sized_string& other_p, const size_type input_position_p, const size_type input_count_p = 1) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        FE_ASSERT(other_p.m_length < (input_count_p + input_position_p), "${%s0}: out of input fstring index boundary.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        Traits::append(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, other_p.m_fstring, input_position_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }


    _CONSTEXPR20_ fixed_sized_string& append(const value_type* string_p, const size_type input_position_p, const size_type input_count_p = 1) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(cstr_ptrc_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        Traits::append(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, string_p, input_position_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }


    _CONSTEXPR20_ fixed_sized_string& append(const value_type* string_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));

        Traits::append(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, string_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& append(const_iterator input_begin_p, const_iterator input_end_p) noexcept
    {
        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_end_p));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: input_position_p must not be greater than input_count_p.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        FE_ASSERT((input_end_p - input_begin_p) + this->m_length >= Capacity, "${%s@0}: input string range length exceeds fixed sized string capacity.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
        
        Traits::append(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, input_begin_p, input_end_p);
        this->m_length += input_end_p - input_begin_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& append(std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: The initializer_list_p is empty.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE));
        FE_ASSERT(this->max_length() < initializer_list_p.size() + this->m_length, "${%s@0}: initializer_list_p is too large to take the contents.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        
        algorithm::string::concatenate(this->data() + this->m_length, this->max_length(), std::move(initializer_list_p));
        this->m_length += initializer_list_p.size();
        this->m_fstring[this->m_length] = _NULL_;
        return *this;
    }


    _FORCE_INLINE_ fixed_sized_string& append_range(const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p) noexcept
    {  
        return this->append(input_iterator_begin_p + input_string_range_p._begin, input_iterator_begin_p + input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& append_range(const fixed_sized_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        return this->append(other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FORCE_INLINE_ fixed_sized_string& append_range(const CharT* const string_p, algorithm::string::range input_string_range_p) noexcept
    {
        return this->append(string_p, input_string_range_p._begin, input_string_range_p._end);
    }


    _CONSTEXPR20_ fixed_sized_string& operator+=(const fixed_sized_string& other_p) noexcept
    {
        if (other_p.m_length == 0)
        {
            return *this;
        }

        this->m_length += other_p.m_length;
        algorithm::string::concatenate<CharT>(this->m_fstring, Capacity, other_p.m_fstring, other_p.m_length);
        this->m_fstring[this->m_length] = _NULL_;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& operator+=(const value_type* const string_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

        length_type l_input_string_length = algorithm::string::length<CharT>(string_p);
        algorithm::string::concatenate<CharT>(this->m_fstring, Capacity, string_p, l_input_string_length);
        this->m_length += l_input_string_length;
        this->m_fstring[this->m_length] = _NULL_;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& operator+=(const value_type value_p) noexcept
    {
        algorithm::string::concatenate<CharT>(this->m_fstring, Capacity, &value_p, 1);
        this->m_length += 1;
        this->m_fstring[this->m_length] = _NULL_;
        return *this;
    }


    _NODISCARD_ _CONSTEXPR20_ boolean starts_with(const fixed_sized_string& other_p) const noexcept
    {
        if (other_p.m_length == 0)
        {
            return false;
        }

        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{0, other_p.m_length},
            other_p.data(), algorithm::string::range{0, other_p.m_length }
        );
    }

    _NODISCARD_ _CONSTEXPR20_ boolean starts_with(const value_type value_p) const noexcept
    {
        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{0, 1},
            & value_p, algorithm::string::range{ 0, 1 }
        );
    }

    _NODISCARD_ _CONSTEXPR20_ boolean starts_with(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@0} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

        length_t l_substring_length = algorithm::string::length(string_p);
        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{0, l_substring_length},
            string_p, algorithm::string::range{ 0, l_substring_length }
        );
    }


    _NODISCARD_ _CONSTEXPR20_ boolean ends_with(const fixed_sized_string& other_p) const noexcept
    {
        if (other_p.m_length == 0)
        {
            return false;
        }

        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{this->m_length - other_p.m_length, this->m_length},
            other_p.data(), algorithm::string::range{ 0, other_p.m_length }
        );
    }

    _NODISCARD_ _CONSTEXPR20_ boolean ends_with(const value_type value_p) const noexcept
    {
        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{this->m_length - 1, this->m_length},
            & value_p, algorithm::string::range{ 0, 1 }
        );
    }

    _NODISCARD_ _CONSTEXPR20_ boolean ends_with(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        length_t l_substring_length = algorithm::string::length(string_p);
        return algorithm::string::compare_ranged(this->m_fstring, algorithm::string::range{this->m_length - l_substring_length, this->m_length},
            string_p, algorithm::string::range{ 0, l_substring_length }
        );
    }


    _NODISCARD_ _CONSTEXPR20_  boolean contains(const fixed_sized_string& other_p) const noexcept
    {
        if (other_p.m_length == 0)
        {
            return false;
        }

        return (algorithm::string::find_the_first(this->m_fstring, other_p.m_fstring)).has_value();
    }

    _NODISCARD_ _CONSTEXPR20_ boolean contains(const value_type value_p) const noexcept
    {
        return algorithm::string::find_the_first(this->m_fstring, value_p).has_value();
    }

    _NODISCARD_ _CONSTEXPR20_ boolean contains(const CharT* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return (algorithm::string::find_the_first(this->m_fstring, string_p)).has_value();
    }


    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, const fixed_sized_string& other_p) noexcept
    {
        FE_ASSERT(((this->m_length + other_p.m_length) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        
        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, other_p.m_fstring, other_p.m_length);
        this->m_length = (this->m_length + other_p.m_length) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, const fixed_sized_string& other_p, const size_type other_position_p, const size_type other_count_p) noexcept
    {
        FE_ASSERT(((this->m_length + other_count_p) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(other_position_p > other_p.m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(other_position_p), TO_STRING(other_p.m_length));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        FE_ASSERT(other_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(other_count_p));

        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, other_p.m_fstring + other_position_p, other_count_p);
        this->m_length = (this->m_length + other_p.m_length) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, const value_type* const string_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(((this->m_length + input_count_p) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        
        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, string_p, input_count_p);
        this->m_length = (this->m_length + input_count_p) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, const value_type* const string_p) noexcept
    {
        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        
        size_type l_input_length = algorithm::string::length(string_p);
        FE_ASSERT(((this->m_length + l_input_length) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));

        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, string_p, l_input_length);
        this->m_length = (this->m_length + l_input_length) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, const value_type value_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(((this->m_length + input_count_p) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        
        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, value_p, input_count_p);
        this->m_length = (this->m_length + input_count_p) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type position_p, const size_type count_to_replace_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        size_type l_input_size = initializer_list_p.size();

        FE_ASSERT(((this->m_length + l_input_size) - count_to_replace_p) > this->max_length(), "${%s@0}: failed to replace.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_replace_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(count_to_replace_p));
        FE_ASSERT(l_input_size == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(initializer_list_p.size()));

        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, position_p, count_to_replace_p, std::move(initializer_list_p));
        this->m_length = (this->m_length + l_input_size) - count_to_replace_p;
        return *this;
    }

    _CONSTEXPR20_ fixed_sized_string& replace(const size_type first_index_p, const size_type last_index_p, const_iterator input_first_p, const_iterator input_last_p) noexcept
    {
        FE_ASSERT(input_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_first_p));
        FE_ASSERT(input_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(input_last_p));
        FE_ASSERT(input_first_p > input_last_p, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_first_p), TO_STRING(input_last_p));
       
        size_type l_input_size = input_last_p - input_first_p;
        size_type l_this_count_to_replace = last_index_p - first_index_p;

        Traits::replace(string_info<CharT>{this->m_fstring, this->m_length, Capacity}, first_index_p, last_index_p, input_first_p, input_last_p);
        this->m_length = (this->m_length + l_input_size) - l_this_count_to_replace;
        return *this;
    }


    _CONSTEXPR20_ fixed_sized_string& replace_with_range(const size_type position_p, const size_type count_to_replace_p, const_iterator input_iterator_begin_p, algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(input_iterator_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(cstr_ptrc_p));
        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->max_length(), "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->max_length()));

        return this->replace(position_p, count_to_replace_p, input_iterator_begin_p + input_string_range_p._begin, input_iterator_begin_p + input_string_range_p._end);
    }

    _CONSTEXPR20_ fixed_sized_string& replace_with_range(const size_type position_p, const size_type count_to_replace_p, const fixed_sized_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        if (other_p.m_length == 0)
        {
            return *this;
        }

        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->max_length(), "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->max_length()));

        return this->replace(position_p, count_to_replace_p, other_p, input_string_range_p._begin, input_string_range_p._end - input_string_range_p._begin);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ fixed_sized_string& replace_with_range(const size_type position_p, const size_type count_to_replace_p, const value_type* const string_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->max_length(), "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->max_length()));

        return this->replace(position_p, count_to_replace_p, string_p + input_string_range_p._begin, input_string_range_p._end - input_string_range_p._begin);
    }


    _NODISCARD_ _CONSTEXPR20_ fixed_sized_string substr(const size_type position_p, const size_type count_p) const noexcept
    {
        fixed_sized_string l_tmp;
        l_tmp.assign(*this, position_p, position_p + count_p);
        return l_tmp;
    }

    _CONSTEXPR20_ void copy(value_type* const dest_out_buffer_p, _MAYBE_UNUSED_ const size_type buffer_capacity_p, const size_type count_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(dest_out_buffer_p == nullptr, "${%s@0}: dest_out_buffer_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
        FE_ASSERT(count_p > this->m_length, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(count_p), TO_STRING(this->m_length));
        FE_ASSERT(count_p > buffer_capacity_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(count_p), TO_STRING(buffer_capacity_p));

        algorithm::string::copy(dest_out_buffer_p, this->m_fstring + position_p, count_p);
    }


    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const fixed_sized_string& other_p, const size_type position_p = 0) const noexcept
    {
        if (other_p.m_length == 0)
        {
            return std::nullopt;
        }

        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));

        return std::move(this->find(other_p.m_fstring, position_p));
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const value_type* const string_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));
        
        std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->m_fstring + position_p, string_p);
        l_result->_begin += position_p;
        l_result->_end += position_p;
        return std::move(l_result);
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));
        
        std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->m_fstring + position_p, value_p);
        
        if (l_result == std::nullopt)
        {
            return std::nullopt;
        }

        l_result->_begin += position_p;
        l_result->_end += position_p;
        return std::move(l_result);
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const fixed_sized_string& other_p, const size_type position_p = 0) const noexcept
    {
        if (other_p.m_length == 0)
        {
            return std::nullopt;
        }

        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));

        return std::move(this->rfind(other_p.m_fstring, position_p));
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const value_type* const string_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));

        return std::move(algorithm::string::find_the_last_within_range(this->m_fstring, algorithm::string::range{position_p, this->m_length}, string_p));
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));

        return std::move(algorithm::string::find_the_last_within_range(this->m_fstring, algorithm::string::range{position_p, this->m_length}, value_p));
    }

    _NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::count<CharT>> count_chars(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->m_length));

        return std::move(algorithm::string::count_chars(this->m_fstring + position_p, value_p));
    }

    _CONSTEXPR20_ void swap(fixed_sized_string& in_out_other_p) noexcept
    {
        algorithm::utility::swap(*this, in_out_other_p);
    }
          
    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const fixed_sized_string& other_p) const noexcept
    {
        return this->operator==(other_p.m_fstring);
    }

    _CONSTEXPR20_ boolean operator==(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return algorithm::string::compare(this->m_fstring, string_p);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const fixed_sized_string& other_p) const noexcept
    {
        return this->operator!=(other_p.m_fstring);
    }

    _CONSTEXPR20_ boolean operator!=(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return !algorithm::string::compare(this->m_fstring, string_p);
    }

    _CONSTEXPR20_ friend boolean operator==(const value_type* const string_p, const fixed_sized_string& fstring_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return algorithm::string::compare(string_p, fstring_p.m_fstring);
    }

    _CONSTEXPR20_ friend boolean operator!=(const value_type* const string_p, const fixed_sized_string& fstring_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
        return !algorithm::string::compare(string_p, fstring_p.m_fstring);
    }
};


template<uint64 Capacity>
using fstring = FE::fixed_sized_string<char, Capacity>;

template<uint64 Capacity>
using ufstring = FE::fixed_sized_string<var::uchar, Capacity>;

template<uint64 Capacity>
using sfstring = FE::fixed_sized_string<var::schar, Capacity>;

template<uint64 Capacity>
using fwstring = FE::fixed_sized_string<var::wchar, Capacity>;

#ifdef _HAS_CXX20_
template<uint64 Capacity>
using fstring8 = FE::fixed_sized_string<var::UTF8, Capacity>;
#endif

template<uint64 Capacity>
using fstring16 = FE::fixed_sized_string<var::UTF16, Capacity>;

template<uint64 Capacity>
using fstring32 = FE::fixed_sized_string<var::UTF32, Capacity>;


END_NAMESPACE;
#pragma warning (pop)
#endif
