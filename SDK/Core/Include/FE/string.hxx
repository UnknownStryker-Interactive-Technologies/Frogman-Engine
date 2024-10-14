#ifndef _FE_CORE_STRING_HXX_
#define _FE_CORE_STRING_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/allocator.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/char_traits.hxx>
#include <FE/iterator.hxx>

#ifdef _MEMORY_POOL_FE_STRINGS_
#include <FE/pool/pool_allocator.hxx>
#endif

#include <FE/managed.h>

// std
#include <utility>

#pragma warning(push)

#define _NULL_ESCAPE_SIZE_ 1




BEGIN_NAMESPACE(FE)


template<typename CharT>
class basic_string_view;

/* 
Brief: a string class template.
Memory has to be pre-allocated and handled explicitly by the programmer. For example, l_string_instance.append("Hello, World!"); will result in segmentation fault if memory has not been reserve()-ed or pre-allocated for the desired string manipulation.
Frogman Engine string will always allocate extra one byte than the requested size, for the null terminator.
FE.pool can be utilized for it if _MEMORY_POOL_FE_STRINGS_ is defined.
By default, the string would always be aligned to the hardware-specific SIMD Alignment requirements.
*/
template<typename CharT = char, 
#ifdef _MEMORY_POOL_FE_STRINGS_
    class Allocator = FE::pool_allocator<CharT>,
#else
    class Allocator = FE::aligned_allocator<CharT>, 
#endif
    class Traits = FE::char_traits<CharT>
> 
class basic_string final 
{
    FE_STATIC_ASSERT(std::is_const<CharT>::value == true, "static assertion failed: the template argument CharT cannot be const.");
    FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "static assertion failed: the template argument CharT is not a valid character type.");
    FE_STATIC_ASSERT(std::is_class<Allocator>::value == false, "static assertion failed: the template argument Allocator is not a valid class type.");
    FE_STATIC_ASSERT(std::is_class<Traits>::value == false, "static assertion failed: the template argument traits is not a class type.");
    FE_STATIC_ASSERT((std::is_same<CharT, typename Allocator::value_type>::value == false), "static assertion failed: enforcing Allocator's value_type to be equivalent to CharT. The template parameter CharT must be identical to the value_type of the Allocator.");
    FE_STATIC_ASSERT((std::is_same<CharT, typename Traits::value_type>::value == false), "static assertion failed: enforcing traits' value_type to be equivalent to CharT. The template parameter CharT must be identical to traits::value_type.");
    FE_STATIC_ASSERT(Allocator::is_address_aligned == ADDRESS::_NOT_ALIGNED, "static assertion failed: addresses allocated from the class Allocator must meet the hardware-specific SIMD Alignment requirements.");

public:
    using traits_type = Traits;
    using value_type = CharT;
    using allocator_type = Allocator;
    using size_type = var::uint64;
    using length_type = var::uint64;
    using difference_type = var::ptrdiff;
    using reference = CharT&;
    using const_reference = const CharT&;
    using pointer = CharT*;
    using const_pointer = const CharT*;
    using iterator = FE::iterator<FE::contiguous_iterator<CharT>>;
    using const_iterator = FE::const_iterator<FE::contiguous_iterator<CharT>>;
    using reverse_iterator = FE::reverse_iterator<FE::contiguous_iterator<CharT>>;
    using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<CharT>>;
    using smart_pointer = FE::exclusive_ptr<value_type[], Allocator>;

private:
    FE::exclusive_ptr<value_type[], Allocator> m_smart_string;
    length_type m_length;

public:
    _FE_CONSTEXPR20_ basic_string() noexcept : m_smart_string(), m_length() {}
    _FE_CONSTEXPR23_ ~basic_string() noexcept {}

    _FE_CONSTEXPR20_ basic_string(const size_type count_p, const value_type value_p = null) noexcept
    {
        if(count_p == 0)
        {
            return;
        }

        this->m_smart_string = FE::make_exclusive<value_type[], Allocator>(count_p + _NULL_ESCAPE_SIZE_);
        this->m_length = count_p;
        Traits::assign(this->m_smart_string.get(), count_p, value_p);
    }

 
    _FE_CONSTEXPR20_ basic_string(const basic_string& other_p, const size_type position_p) noexcept
    {
        FE_ASSERT(position_p >= other_p.m_length, "${%s@0}: ${%s@1} was ${%lu@2}, and ${%s@3} was ${%lu@4}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(position_p), &position_p, TO_STRING(other_p.m_length), &other_p.m_length);
        
        if (other_p.is_empty() == true)
        {
            return;
        }

        size_type l_input_string_length = other_p.m_length - position_p;
        this->m_smart_string.reset(FE::resize_to{ l_input_string_length + _NULL_ESCAPE_SIZE_ });

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, other_p.m_smart_string.get() + position_p, l_input_string_length);
        this->m_length = l_input_string_length;
    }


    _FE_CONSTEXPR20_ basic_string(basic_string&& rvalue_p, const size_type position_p) noexcept 
    {
        FE_ASSERT(position_p >= rvalue_p.m_length, "${%s@0}: ${%s@1} was ${%lu@2}, and ${%s@3} was ${%lu@4}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(position_p), &position_p, TO_STRING(rvalue_p.m_length), &rvalue_p.m_length);
        
        if (rvalue_p.is_empty() == true)
        {
            return;
        }

        size_type l_input_string_length = rvalue_p.m_length - position_p;
        FE_UNALIGNED_MEMMOVE(rvalue_p.m_smart_string.get() + position_p, rvalue_p.m_smart_string.get(), sizeof(CharT) * l_input_string_length);
        rvalue_p.m_smart_string[l_input_string_length] = null;
        this->m_smart_string.swap(rvalue_p.m_smart_string);
        this->m_length = l_input_string_length;

        rvalue_p.m_length = 0;
    }


    _FE_CONSTEXPR20_ basic_string(const basic_string& other_p, const size_type position_p, const size_type count_p) noexcept
    {
        FE_ASSERT((position_p + count_p) >= other_p.m_length, "${%s@0}: ${%s@1} was ${%lu@2}, and ${%s@3} was ${%lu@4}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING((position_p + count_p)), &FE::buffer<size_type>::set_and_get(position_p + count_p), TO_STRING(other_p.m_length), &other_p.m_length);
        
        if (other_p.is_empty() == true || count_p == 0)
        {
            return;
        }

        this->m_smart_string.reset(FE::resize_to{ count_p + _NULL_ESCAPE_SIZE_ });
        FE_UNALIGNED_MEMCPY(this->m_smart_string.get(), other_p.m_smart_string.get() + position_p, sizeof(CharT) * count_p);
        this->m_length = count_p;
        this->m_smart_string[count_p] = null;
    }


    _FE_CONSTEXPR20_ basic_string(basic_string&& rvalue_p, const size_type position_p, const size_type count_p) noexcept
    {
        FE_ASSERT((position_p + count_p) >= rvalue_p.m_length, "${%s@0}: ${%s@1} was ${%lu@2}, and ${%s@3} was ${%lu@4}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING((position_p + count_p)), &FE::buffer<size_type>::set_and_get(position_p + count_p), TO_STRING(rvalue_p.m_length), &rvalue_p.m_length);
        
        if (rvalue_p.is_empty() == true || count_p == 0)
        {
			return;
        }

        FE_UNALIGNED_MEMMOVE(rvalue_p.m_smart_string.get(), rvalue_p.m_smart_string.get() + position_p, sizeof(CharT) * count_p);
        this->m_smart_string.swap(rvalue_p.m_smart_string);
        this->m_length = count_p;
        this->m_smart_string[count_p] = null;
    }

    _FE_CONSTEXPR20_ basic_string(const value_type* const string_p, const size_type count_p) noexcept 
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(algorithm::string::length(string_p) < count_p, "${%s@0}: ${%s@1} was greater than the length of the input string. The input string length was ${%lu@2}, and ${%s@1} was ${%s@1}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(count_p), &FE::buffer<size_type>::set_and_get(algorithm::string::length(string_p)), &count_p);
        
        if(count_p == 0)
        {
            return;
        }
        this->m_smart_string = FE::make_exclusive<value_type[], Allocator>(count_p + _NULL_ESCAPE_SIZE_);
        this->m_length = count_p;

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_p, count_p);
    }
  
    _FE_CONSTEXPR20_ basic_string(const value_type* const string_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        
        this->m_length =  algorithm::string::length(string_p);
        if(this->m_length == 0)
        {
            return;
        }
        this->m_smart_string = FE::make_exclusive<CharT[], Allocator>(this->m_length + _NULL_ESCAPE_SIZE_);
        
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_p, this->m_length);
    }

    basic_string(std::nullptr_t) noexcept = delete;

    _FE_FORCE_INLINE_ const allocator_type& get_allocator() const noexcept { return this->m_smart_string.get_allocator(); }
    _FE_FORCE_INLINE_ allocator_type& get_allocator() noexcept { return this->m_smart_string.get_allocator(); }

    template<class InputIterator>
    _FE_CONSTEXPR20_ basic_string(InputIterator first_p, InputIterator last_p) noexcept
    {
        FE_STATIC_ASSERT(std::is_class<InputIterator>::value == false, "Static Assertion Failure: The template argument InputIterator must be a class type.");
        FE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value == false), "Static Assertion Failure: InputIterator's value_type has to be the same as basic_string's value_type.");

        FE_ASSERT(first_p >= last_p, "${%s@0}: The input iterator ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(first_p), TO_STRING(last_p));

        this->m_length = static_cast<FE::size>(last_p - first_p);
        if(this->m_length == 0)
        {
            return;
        }
        this->m_smart_string = FE::make_exclusive<CharT[], Allocator>(this->m_length + _NULL_ESCAPE_SIZE_);

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();
        
        Traits::assign(l_this_string_info, first_p, last_p);
    }

 
    _FE_CONSTEXPR20_ basic_string(const basic_string& other_p) noexcept
    {
        if (other_p.is_empty() == true)
        {
            return;
        }
        this->m_smart_string = FE::make_exclusive<value_type[], Allocator>(other_p.m_smart_string.capacity());
        this->m_length = other_p.m_length;
        Traits::template copy<allocator_type::is_address_aligned, allocator_type::is_address_aligned>(this->m_smart_string.get(), other_p.m_smart_string.get(), other_p.m_smart_string.capacity());
    }


    _FE_CONSTEXPR20_ basic_string(basic_string&& rvalue_p) noexcept : m_smart_string(std::move(rvalue_p.m_smart_string)), m_length(rvalue_p.m_length)
    {
        if (rvalue_p.is_empty() == true)
        {
            return;
        }

        rvalue_p.m_length = 0; 
    }

    
    _FE_CONSTEXPR20_ basic_string(basic_string_view<CharT>& string_view_p) noexcept 
    {
        if (string_view_p.is_empty() == true)
        {
            return;
        }
        this->m_length = string_view_p.length();
        this->m_smart_string = FE::make_exclusive<CharT[], Allocator>(this->m_length + _NULL_ESCAPE_SIZE_);

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_view_p.data(), string_view_p.length());
    }

    
    _FE_CONSTEXPR20_ basic_string(basic_string_view<CharT>& string_view_p, const size_type position_p, const size_type count_p) noexcept
    {
        if (string_view_p.is_empty() == true || count_p == 0)
        {
            return;
        }

        FE_ASSERT(position_p >= string_view_p.length(), "${%s@0}: The inserting position index value cannot be greater than the length of ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(string_view_p));
        
        this->m_smart_string = FE::make_exclusive<CharT[], Allocator>(count_p + _NULL_ESCAPE_SIZE_);
        this->m_length = count_p;

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_view_p.data() + position_p, count_p);
    }
    

    _FE_CONSTEXPR20_ basic_string& operator=(const basic_string& other_p) noexcept
    {
        if (other_p.m_length == 0)
        {
            return *this;
        }

        if (this->m_smart_string.capacity() < other_p.m_length)
        {
            this->reserve(other_p.m_length);
        }

        Traits::template copy<allocator_type::is_address_aligned, allocator_type::is_address_aligned>(this->m_smart_string.get(), other_p.m_smart_string.get(), other_p.m_length);
        this->m_length = other_p.m_length;

        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& operator=(basic_string&& rvalue_p) noexcept
    {
        if (rvalue_p.m_length == 0)
        {
            return *this;
        }

        this->m_smart_string = std::move(rvalue_p.m_smart_string);
        this->m_length = std::exchange<length_type>(rvalue_p.m_length, 0);
        return *this;
    }
    _FE_CONSTEXPR20_ basic_string& operator=(const value_type* const string_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        length_type l_input_string_length = algorithm::string::length(string_p);

        if(l_input_string_length == 0)
        {
            return *this;
        }

        if (this->m_smart_string.capacity() < l_input_string_length)
        {
            this->reserve(l_input_string_length + _NULL_ESCAPE_SIZE_);
        }

        FE_UNALIGNED_MEMCPY(this->m_smart_string.get(), string_p, sizeof(CharT) * l_input_string_length);
        this->m_length = l_input_string_length;
        this->m_smart_string[this->m_length] = null;
        return *this;
    }
    _FE_CONSTEXPR20_ basic_string& operator=(const value_type value_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        this->m_smart_string[0] = value_p;
        this->m_smart_string[1] = null;
        this->m_length = 1;
        return *this;
    }
    _FE_CONSTEXPR20_ basic_string& operator=(std::initializer_list<CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: ${%s@1} was empty.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(initializer_list_p));
        size_type l_initializer_list_size = initializer_list_p.size();

        if (this->m_smart_string.capacity() < l_initializer_list_size)
        {
            this->reserve(l_initializer_list_size);
        }

        Traits::template copy<allocator_type::is_address_aligned>(this->m_smart_string.get(), const_cast<CharT*>(initializer_list_p.begin()), l_initializer_list_size);
        this->m_length = l_initializer_list_size;
        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& operator=(basic_string_view<CharT>& string_view_p) noexcept
    {
        if (string_view_p.is_empty() == true)
        {
            return *this;
        }

        if (this->m_smart_string.capacity() < string_view_p.length())
        {
            this->reserve(string_view_p.length());
        }

        Traits::template copy<allocator_type::is_address_aligned>(this->m_smart_string.get(), string_view_p.data(), string_view_p.length());
        this->m_length = string_view_p.length();

        return *this;
    }

    basic_string& operator=(std::nullptr_t) = delete;
    
    _FE_CONSTEXPR20_ basic_string& assign(const size_type count_p, const value_type value_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(count_p >= this->capacity(), "${%s@0}: ${%s@1} exceeds string capacity.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW), TO_STRING(count_p));
        
        if(count_p == 0)
        {
            return *this;
        }
        Traits::assign(this->m_smart_string.get(), count_p, value_p);
        this->m_length = count_p;
        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& assign(const basic_string& other_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than or equal to ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT((input_end_p - input_begin_p) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(input_end_p > other_p.length(), "Assertion failure: the input string capacity overflowed.");

        if (other_p.m_length == 0 || input_end_p == input_begin_p)
        {
            return *this;
        }

        return this->assign(other_p.m_smart_string.get(), input_begin_p, input_end_p);
    }

    _FE_CONSTEXPR20_ basic_string& assign(const value_type* const string_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT((input_end_p - input_begin_p) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(input_end_p > algorithm::string::length(string_p), "Assertion failure: the input string capacity overflowed.");
        
        if (*string_p == null || input_end_p == input_begin_p)
        {
            return *this;
        }
        
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_p, input_begin_p, input_end_p);
        this->m_length = input_end_p - input_begin_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& assign(const value_type* const string_p, const size_type size_to_assign_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(size_to_assign_p >= this->capacity(), "Assertion failed: size_to_assign_p exceeds string capacity.");
        FE_ASSERT(size_to_assign_p >= algorithm::string::length(string_p), "Assertion failure: the input string capacity overflowed.");
        
        if(size_to_assign_p == 0 || *string_p == null)
        {
            return *this;
        }
        
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, string_p, size_to_assign_p);
        this->m_length = size_to_assign_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& assign(const_iterator input_begin_p, const_iterator input_end_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_end_p));

        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} must be smaller than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT((input_end_p - input_begin_p) >= this->capacity(), "${%s@0}: input string range length exceeds string capacity.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));

        if(input_end_p == input_begin_p)
        {
            return *this;
        }

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::assign(l_this_string_info, input_begin_p, input_end_p);
        this->m_length = input_end_p - input_begin_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& assign(std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(initializer_list_p.size() >= this->capacity(), "ERROR: initializer_list_p.size() exceeds the string capacity.");
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: {%s@1} is an empty initializer_list_p.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(initializer_list_p));

        algorithm::string::concatenate(this->m_smart_string.get(), this->capacity(), std::move(initializer_list_p));
        this->m_length = initializer_list_p.size();
        this->m_smart_string[this->m_length] = null;
        return *this;
    }


    _FE_FORCE_INLINE_ basic_string& assign_range(const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return this->assign(*input_iterator_begin_p, input_string_range_p._begin, input_string_range_p._end);
    }


    _FE_FORCE_INLINE_ basic_string& assign_range(const basic_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return this->assign(other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_FORCE_INLINE_ basic_string& assign_range(const value_type* const string_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(string_p == nullptr, "${%s@0}: {%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        if(*string_p == null)
        {
            return *this;
        }
        return this->assign(string_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reference operator[](index_t index_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(index_p >= this->m_smart_string.capacity(), "${%s@0}: ${%s@1} was ${%lu@2}, and the string capacity was ${%lu@3}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(index_p), &index_p, &FE::buffer<size_type>::set_and_get(this->m_smart_string.capacity()));
        return this->m_smart_string[index_p];
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ reference operator[](index_t index_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(index_p >= this->m_smart_string.capacity(), "${%s@0}: ${%s@1} was ${%lu@2}, and the string capacity was ${%lu@3}", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(index_p), &index_p, &FE::buffer<size_type>::set_and_get(this->m_smart_string.capacity()));
        return this->m_smart_string[index_p];
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reference front() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return *(this->m_smart_string.get()); 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ reference front() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return *(this->m_smart_string.get()); 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reference back() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return *((this->m_smart_string.get() + this->m_length) - 1); 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ reference back() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return *((this->m_smart_string.get() + this->m_length) - 1); 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_pointer data() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return this->m_smart_string.get(); 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ pointer data() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return this->m_smart_string.get(); 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const CharT* c_str() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return this->m_smart_string.get();
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ operator basic_string_view<CharT>() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return FE::basic_string_view<CharT>(this->m_smart_string); 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ operator ptr<CharT[]>() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return FE::ptr<CharT>(this->m_smart_string);
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ iterator begin() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return iterator{this->m_smart_string.get()}; 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ iterator end()  noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return iterator{this->m_smart_string.get() + this->m_length}; 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator begin() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_iterator{this->m_smart_string.get()}; 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator end() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_iterator{this->m_smart_string.get() + this->m_length}; 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator cbegin() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_iterator{this->m_smart_string.get()}; 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator cend() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_iterator{this->m_smart_string.get() + this->m_length};
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ reverse_iterator rbegin() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return reverse_iterator{(this->m_smart_string.get() + this->m_length) - 1}; 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ reverse_iterator rend() noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return reverse_iterator{this->m_smart_string.get() - 1}; 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator rbegin() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_reverse_iterator{(this->m_smart_string.get() + this->m_length) - 1}; 
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator rend() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_reverse_iterator{this->m_smart_string.get() - 1}; 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_reverse_iterator{(this->m_smart_string.get() + this->m_length) - 1};
    }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crend() const noexcept
    { 
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        return const_reverse_iterator{this->m_smart_string.get() - 1}; 
    }

    _FE_NODISCARD_ _FE_FORCE_INLINE_ boolean is_empty() const noexcept { return this->m_length == 0; }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ length_type length() const noexcept { return this->m_length; }
    _FE_NODISCARD_ _FE_FORCE_INLINE_ length_type max_length() const noexcept { return FE::max_value<length_type> / sizeof(CharT); }


    // if requested size is 5, then the acual size will be 6 including the null terminator.
    _FE_CONSTEXPR20_ void reserve(const length_type new_capacity_p) noexcept
    {
        FE_ASSERT(new_capacity_p == 0, "${%s@0}: Unable to reserve(). ${%s@1} was zero.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(new_capacity_p));
        FE_ASSERT(new_capacity_p > this->max_length(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(new_capacity_p), TO_STRING(this->max_length()));

        if (new_capacity_p <= this->m_smart_string.capacity())
        {
            return;
        }

        this->m_smart_string.reset(FE::resize_to{ new_capacity_p + _NULL_ESCAPE_SIZE_});
    }

    // if requested size is 5, then the acual size will be 6 including the null terminator.
    _FE_CONSTEXPR20_ void extend(const length_type extra_capacity_p) noexcept
    {
        FE_ASSERT(extra_capacity_p == 0, "${%s@0}: Unable to extend(). ${%s@1} was zero.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(extra_capacity_p));
        FE_ASSERT(extra_capacity_p + this->m_smart_string.capacity() > this->max_length(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(extra_capacity_p + this->m_smart_string.capacity()), TO_STRING(this->max_length()));
        FE_ASSERT(this->m_smart_string.capacity() == 0, "Assertion Failure: Unable to extend an empty string. Please reserve() or resize() before extend() to ensure allocating extra space for the null escape sequence.");

        this->m_smart_string.reset(FE::resize_to{ extra_capacity_p + this->m_smart_string.capacity() });
    }
    
    // returns the current capacity of the string including the null terminator.
    _FE_NODISCARD_ _FE_FORCE_INLINE_ length_type capacity() const noexcept { return this->m_smart_string.capacity(); }
    
    _FE_CONSTEXPR20_ void shrink_to_fit() noexcept
    {
        FE_ASSERT(this->m_length == 0, "${%s@0}: Unable to shrink_to_fit() an empty string instance.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        this->m_smart_string.reset(FE::resize_to{ this->m_length + _NULL_ESCAPE_SIZE_ });
    }


    _FE_FORCE_INLINE_ void clear() noexcept 
    { 
        FE_ASSERT(this->m_length == 0, "${%s@0}: Unable to clear() an empty string instance.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        *(this->m_smart_string) = null;
        this->m_length = 0;
    }   

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, const size_type count_p, value_type value_p) noexcept
    {
        FE_ASSERT((position_p + count_p) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(count_p == 0, "ERROR: insert() operation was not successful. size_type count_p was zero.");
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, count_p, value_p);
        this->m_length += count_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, const value_type* const string_p) noexcept
    {
        size_type l_inout_string_length = algorithm::string::length(string_p);

        FE_ASSERT((position_p + l_inout_string_length) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(string_p == nullptr, "${%s@0}: the input string is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if(*string_p == null)
        {
            return *this;
        }

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, string_p, l_inout_string_length);
        this->m_length += l_inout_string_length;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, const basic_string& other_p) noexcept
    {
        FE_ASSERT((position_p + other_p.m_length) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, other_p.m_smart_string.get(), other_p.m_length);
        this->m_length += other_p.m_length;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, const basic_string& other_p, const size_type input_begin_p, size_type input_end_p) noexcept
    {
        FE_ASSERT((position_p + (input_end_p - input_begin_p)) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(input_end_p > other_p.length(), "Assertion failure: the input string capacity overflowed.");
        
        if (other_p.m_length == 0)
        {
            return *this;
        }

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, other_p.m_smart_string.get(), input_begin_p, input_end_p);
        this->m_length += input_end_p - input_begin_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, const value_type* const string_p, const size_type input_begin_p, const size_type input_end_p) noexcept
    {
        FE_ASSERT((position_p + (input_end_p - input_begin_p)) >= this->capacity(), "Assertion failure: string capacity overflowed.");
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(input_begin_p), TO_STRING(input_end_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(input_end_p > algorithm::string::length(string_p), "Assertion failure: the input string capacity overflowed.");

        if(*string_p == null)
        {
            return *this;
        }

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, string_p, input_begin_p, input_end_p);
        this->m_length += input_end_p - input_begin_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& insert(const size_type position_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: the initializer_list_p is empty.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
        FE_ASSERT((position_p + initializer_list_p.size()) >= this->capacity(), "${%s@0}: string capacity overflowed.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::insert(l_this_string_info, position_p, std::move(initializer_list_p));
        this->m_length += initializer_list_p.size();
        return *this;
    }

    _FE_FORCE_INLINE_ basic_string& insert_range(const size_type position_p, const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return this->insert(position_p, *input_iterator_begin_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_FORCE_INLINE_ basic_string& insert_range(const size_type position_p, const basic_string& other_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return this->insert(position_p, other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_FORCE_INLINE_ basic_string& insert_range(const size_type position_p, const value_type* const string_p, const algorithm::string::range input_string_range_p)  noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        return this->insert(position_p, string_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_CONSTEXPR20_ basic_string& erase(const size_type index_p = 0, const size_type count_p = 1) noexcept
    {
        FE_ASSERT(this->capacity() < (index_p + count_p), "${%s@0}: string capacity overflowed.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
      
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::erase(l_this_string_info, index_p, count_p);
        this->m_length -= count_p;
        return *this;
    }

    _FE_CONSTEXPR20_ void push_back(const value_type value_p) noexcept
    {
        FE_ASSERT(this->m_length >= this->capacity(), "${%s0}: out of capacity", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        this->m_smart_string[this->m_length] = value_p;
        ++this->m_length;
        this->m_smart_string[this->m_length] = null;
    }

    _FE_CONSTEXPR20_ CharT pop_back() noexcept
    {
        FE_ASSERT(this->m_length == 0, "${%s0}: Unable to pop an empty fstring", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        const CharT l_return_value = this->back();
        --this->m_length;
        this->m_smart_string[this->m_length] = null;
        return l_return_value;
    }

    _FE_CONSTEXPR20_ basic_string& append(const size_type input_count_p, const value_type value_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append ${%ld@1} character(s) to the fstring.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), &input_count_p);
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::append(l_this_string_info, input_count_p, value_p);
        this->m_length += input_count_p;
        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& append(const basic_string& other_p, const size_type input_position_p, const size_type input_count_p = 1) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(other_p.m_length < (input_count_p + input_position_p), "${%s0}: out of input fstring index boundary.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::append(l_this_string_info, other_p.m_smart_string.get(), input_position_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& append(const value_type* string_p, const size_type input_position_p, const size_type input_count_p = 1) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(cstr_ptrc_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (input_position_p + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::append(l_this_string_info, string_p, input_position_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& append(const value_type* string_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_count_p == 0, "${%s0}: ${%s1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(this->capacity() < (this->m_length + input_count_p), "${%s0}: cannot append another fstring that exceeds the capacity of a caller fstring.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::append(l_this_string_info, string_p, input_count_p);
        this->m_length += input_count_p;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& append(const_iterator input_begin_p, const_iterator input_end_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_same<typename const_iterator::value_type, value_type>::value == false), "static assertion failed: const_iterator::value_type is not equal to value_type.");
        FE_ASSERT(input_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_begin_p));
        FE_ASSERT(input_end_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_end_p));
        FE_ASSERT(input_begin_p >= input_end_p, "${%s@0}: input_position_p must not be greater than input_count_p.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(static_cast<size_type>(input_end_p - input_begin_p) + this->m_length >= this->capacity(), "${%s@0}: input string range length exceeds string capacity.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(input_begin_p == input_end_p)
        {
            return *this;
        }
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::append(l_this_string_info, input_begin_p, input_end_p);
        this->m_length += static_cast<size_type>(input_end_p - input_begin_p);
        return *this;
    }

#pragma warning(push)
#pragma warning(disable: 26800)
    _FE_CONSTEXPR20_ basic_string& append(std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}: The initializer_list_p is empty.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
        FE_ASSERT(this->capacity() < initializer_list_p.size() + this->m_length, "${%s@0}: initializer_list_p is too large to take the contents.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        algorithm::string::concatenate(this->data() + this->m_length, this->capacity(), std::move(initializer_list_p));
        this->m_length += initializer_list_p.size();
        this->m_smart_string[this->m_length] = null;
        return *this;
    }
#pragma warning(pop)

    _FE_FORCE_INLINE_ basic_string& append_range(const_iterator input_iterator_begin_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        
        return this->append(input_iterator_begin_p + input_string_range_p._begin, input_iterator_begin_p + input_string_range_p._end);
    }


    _FE_FORCE_INLINE_ basic_string& append_range(const basic_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return this->append(other_p, input_string_range_p._begin, input_string_range_p._end);
    }

    _FE_FORCE_INLINE_ basic_string& append_range(const CharT* const string_p, algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        return this->append(string_p, input_string_range_p._begin, input_string_range_p._end);
    }


    _FE_CONSTEXPR20_ basic_string& operator+=(const basic_string& other_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        this->m_length += other_p.m_length;
        FE_ASSERT( this->m_length >= this->m_smart_string.capacity(), "FE::string overflowed: please reference 'basic_string& operator+=(const basic_string& other_p) noexcept'.");
        algorithm::string::concatenate<CharT>(this->m_smart_string.get(), this->capacity(), other_p.m_smart_string.get(), other_p.m_length);
        this->m_smart_string[this->m_length] = null;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& operator+=(const value_type* const string_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        length_type l_input_string_length = algorithm::string::length<CharT>(string_p);
        FE_ASSERT( (this->m_length + l_input_string_length) >= this->m_smart_string.capacity(), "FE::string overflowed: please reference 'basic_string& operator+=(const basic_string& other_p) noexcept'.");
        algorithm::string::concatenate<CharT>(this->m_smart_string.get(), this->capacity(), string_p, l_input_string_length);
        this->m_length += l_input_string_length;
        this->m_smart_string[this->m_length] = null;
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& operator+=(const value_type value_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        FE_ASSERT( (this->m_length + 1) >= this->m_smart_string.capacity(), "FE::string overflowed: please reference 'basic_string& operator+=(const basic_string& other_p) noexcept'.");
        algorithm::string::concatenate<CharT>(this->m_smart_string.get(), this->capacity(), &value_p, 1);
        this->m_length += 1;
        this->m_smart_string[this->m_length] = null;
        return *this;
    }


    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean starts_with(const basic_string& other_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return false;
        }

        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{0, other_p.m_length},
            other_p.data(), algorithm::string::range{ 0, other_p.m_length }
        );
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean starts_with(const value_type value_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{0, 1},
            & value_p, algorithm::string::range{ 0, 1 }
        );
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean starts_with(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@0} is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return false;
        }
        length_t l_substring_length = algorithm::string::length(string_p);
        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{0, l_substring_length},
            string_p, algorithm::string::range{ 0, l_substring_length }
        );
    }


    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean ends_with(const basic_string& other_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return false;
        }

        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{this->m_length - other_p.m_length, this->m_length},
            other_p.data(), algorithm::string::range{ 0, other_p.m_length }
        );
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean ends_with(const value_type value_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{this->m_length - 1, this->m_length},
            & value_p, algorithm::string::range{ 0, 1 }
        );
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean ends_with(const value_type* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return false;
        }
        length_t l_substring_length = algorithm::string::length(string_p);
        return algorithm::string::compare_ranged<CharT>(this->m_smart_string.get(), algorithm::string::range{this->m_length - l_substring_length, this->m_length},
            string_p, algorithm::string::range{ 0, l_substring_length }
        );
    }


    _FE_NODISCARD_ _FE_CONSTEXPR20_  boolean contains(const basic_string& other_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return false;
        }

        return (algorithm::string::find_the_first(this->m_smart_string.get(), other_p.m_smart_string.get())).has_value();
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean contains(const value_type value_p) const noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return algorithm::string::find_the_first(this->m_smart_string.get(), value_p).has_value();
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ boolean contains(const CharT* const string_p) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return false;
        }
        return (algorithm::string::find_the_first(this->m_smart_string.get(), string_p)).has_value();
    }


    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, const basic_string& other_p) noexcept
    {
        FE_ASSERT(((this->m_length + other_p.m_length) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, other_p.m_smart_string.get(), other_p.m_length);
        this->m_length = (this->m_length + other_p.m_length) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, const basic_string& other_p, const size_type other_position_p, const size_type other_count_p) noexcept
    {
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(((this->m_length + other_count_p) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(other_position_p > other_p.m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE), TO_STRING(other_position_p), TO_STRING(other_p.m_length));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));
        FE_ASSERT(other_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(other_count_p));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, other_p.m_smart_string.get() + other_position_p, other_count_p);
        this->m_length = (this->m_length + other_count_p) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, const value_type* const string_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(((this->m_length + input_count_p) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, string_p, input_count_p);
        this->m_length = (this->m_length + input_count_p) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, const value_type* const string_p) noexcept
    {
        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        size_type l_input_length = algorithm::string::length(string_p);
        FE_ASSERT(((this->m_length + l_input_length) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        if(l_input_length == 0)
        {
            return *this;
        }
        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, string_p, l_input_length);
        this->m_length = (this->m_length + l_input_length) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, const value_type value_p, const size_type input_count_p) noexcept
    {
        FE_ASSERT(input_count_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_count_p));
        FE_ASSERT(((this->m_length + input_count_p) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, value_p, input_count_p);
        this->m_length = (this->m_length + input_count_p) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& replace(const size_type position_p, const size_type count_to_be_removed_p, std::initializer_list<const CharT>&& initializer_list_p) noexcept
    {
        size_type l_input_size = initializer_list_p.size();

        FE_ASSERT(((this->m_length + l_input_size) - count_to_be_removed_p) >= this->capacity(), "${%s@0}: failed to replace.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));
        FE_ASSERT(count_to_be_removed_p == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(count_to_be_removed_p));
        FE_ASSERT(l_input_size == 0, "${%s@0}: ${%s@1} is zero.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(initializer_list_p.size()));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, position_p, count_to_be_removed_p, std::move(initializer_list_p));
        this->m_length = (this->m_length + l_input_size) - count_to_be_removed_p;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }

    _FE_CONSTEXPR20_ basic_string& replace(const size_type first_index_p, const size_type last_index_p, const_iterator input_first_p, const_iterator input_last_p) noexcept
    {
        FE_ASSERT(input_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_first_p));
        FE_ASSERT(input_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(input_last_p));
        FE_ASSERT(input_first_p > input_last_p, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_first_p), TO_STRING(input_last_p));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
 
        size_type l_input_size = static_cast<size_type>(input_last_p - input_first_p);
        size_type l_this_count_to_replace = last_index_p - first_index_p;

        string_info<CharT> l_this_string_info;
        l_this_string_info._string_pointer = this->m_smart_string.get();
        l_this_string_info._length = this->m_length;
        l_this_string_info._capacity = this->m_smart_string.capacity();

        Traits::replace(l_this_string_info, first_index_p, last_index_p, input_first_p, input_last_p);
        this->m_length = (this->m_length + l_input_size) - l_this_count_to_replace;
        this->m_smart_string[this->m_length] = null;
        FE_ASSERT(this->m_length > algorithm::string::length(this->m_smart_string.get()), "length integrity is broken: The replace() opertation failed due to undesired use.");
        return *this;
    }


    _FE_CONSTEXPR20_ basic_string& replace_with_range(const size_type position_p, const size_type count_to_be_removed_p, const_iterator input_iterator_begin_p, algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(input_iterator_begin_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(cstr_ptrc_p));
        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return this->replace(position_p, count_to_be_removed_p, input_iterator_begin_p + input_string_range_p._begin, input_iterator_begin_p + input_string_range_p._end);
    }


    _FE_CONSTEXPR20_ basic_string& replace_with_range(const size_type position_p, const size_type count_to_be_removed_p, const basic_string& other_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return *this;
        }

        return this->replace(position_p, count_to_be_removed_p, other_p, input_string_range_p._begin, input_string_range_p._end - input_string_range_p._begin);
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ basic_string& replace_with_range(const size_type position_p, const size_type count_to_be_removed_p, const value_type* const string_p, const algorithm::string::range input_string_range_p) noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(string_p));
        FE_ASSERT(input_string_range_p._begin >= input_string_range_p._end, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(input_string_range_p._begin), TO_STRING(input_string_range_p._end));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} must not be greater than ${%s@2}.", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return *this;
        }
        return this->replace(position_p, count_to_be_removed_p, string_p + input_string_range_p._begin, input_string_range_p._end - input_string_range_p._begin);
    }

#pragma warning(push)
#pragma warning(disable: 26479)
    _FE_NODISCARD_ _FE_CONSTEXPR20_ basic_string substr(const size_type position_p, const size_type count_p) const noexcept
    {
        basic_string l_tmp(*this, position_p, count_p);
        return std::move(l_tmp);
    }
#pragma warning(pop)

    _FE_CONSTEXPR20_ void copy(value_type* const dest_out_buffer_p, _FE_MAYBE_UNUSED_ const size_type buffer_capacity_p, const size_type count_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(dest_out_buffer_p == nullptr, "${%s@0}: dest_out_buffer_p is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
        FE_ASSERT(count_p > this->m_length, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(count_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        FE_ASSERT(count_p > buffer_capacity_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW), TO_STRING(count_p), TO_STRING(buffer_capacity_p));

        algorithm::string::copy(dest_out_buffer_p, this->m_smart_string.get() + position_p, count_p);
    }
    
	// if requested size is 5, then the acual size will be 6 including the null terminator.
    _FE_CONSTEXPR20_ void resize(const size_type count_p) noexcept
    {
        this->m_smart_string.reset(FE::resize_to{ count_p  + _NULL_ESCAPE_SIZE_ });
    }

    _FE_FORCE_INLINE_ void swap(basic_string& in_out_other_p) noexcept
    {
        algorithm::utility::swap(*this, in_out_other_p);
    }


    _FE_NODISCARD_ _FE_CONSTEXPR20_ std::optional<algorithm::string::range> find(const basic_string& other_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return std::nullopt;
        }

        return this->find(other_p.m_smart_string.get(), position_p);
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ std::optional<algorithm::string::range> find(const value_type* const string_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->m_smart_string.get() + position_p, string_p);

        if (l_result == std::nullopt)
        {
            return std::nullopt;
        }

        l_result->_begin += position_p;
        l_result->_end += position_p;
        return l_result;
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ iterator find(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->m_smart_string.get() + position_p, value_p);

        if (l_result == std::nullopt)
        {
            return this->end();
        }

        return this->begin() + (l_result->_begin + position_p);
    }


    _FE_NODISCARD_ _FE_CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const basic_string& other_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        if (other_p.m_length == 0)
        {
            return std::nullopt;
        }

        return this->rfind(other_p.m_smart_string.get(), position_p);
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const value_type* const string_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));
        if(*string_p == null)
        {
            return std::nullopt;
        }
        return algorithm::string::find_the_last_within_range(this->m_smart_string.get(), algorithm::string::range{ position_p, this->m_length }, string_p);
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ iterator rfind(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        std::optional<algorithm::string::range> l_result = algorithm::string::find_the_last_within_range(this->m_smart_string.get(), algorithm::string::range{ position_p, this->m_length }, value_p);

        if (l_result == std::nullopt)
        {
            return this->end();
        }

        return this->begin() + (l_result->_begin);
    }

    _FE_NODISCARD_ _FE_CONSTEXPR20_ std::optional<algorithm::string::count<CharT>> count_chars(const value_type value_p, const size_type position_p = 0) const noexcept
    {
        FE_ASSERT(position_p > this->m_length, "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_OUT_OF_RANGE), TO_STRING(this->m_length));
        FE_ASSERT(this->m_smart_string.get() == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_string.get()));

        return algorithm::string::count_chars(this->m_smart_string.get() + position_p, value_p);
    }

    
    _FE_CONSTEXPR20_ boolean operator==(const basic_string_view<CharT>& other_p) const noexcept
    {
        return algorithm::string::compare(this->m_smart_string.get(), other_p.begin().operator->());
    }


    _FE_CONSTEXPR20_ boolean operator==(const basic_string& other_p) const noexcept
    {
        return algorithm::string::compare(this->m_smart_string.get(), other_p.m_smart_string.get());
    }
    _FE_CONSTEXPR20_ boolean operator==(const value_type* const string_p) const noexcept
    {
        return algorithm::string::compare(this->m_smart_string.get(), string_p);
    }


    _FE_CONSTEXPR20_ friend boolean operator==(const value_type* const string_p, const basic_string& other_p) noexcept
    {
        return algorithm::string::compare(string_p, other_p.m_smart_string.get());
    }

    
    _FE_CONSTEXPR20_ boolean operator!=(const basic_string_view<CharT>& other_p) const noexcept
    {
        return !algorithm::string::compare(this->m_smart_string.get(), other_p.begin().operator->());
    }


    _FE_CONSTEXPR20_ boolean operator!=(const basic_string& other_p) const noexcept
    {
        return !algorithm::string::compare(this->m_smart_string.get(), other_p.m_smart_string.get());
    }
    _FE_CONSTEXPR20_ boolean operator!=(const value_type* const string_p) const noexcept
    {
        return !algorithm::string::compare(this->m_smart_string.get(), string_p);
    }


    _FE_CONSTEXPR20_ friend boolean operator!=(const value_type* const string_p, const basic_string& other_p) noexcept
    {
        return !algorithm::string::compare(string_p, other_p.m_smart_string.get());
    }
};


using string = basic_string<var::ASCII>;
using ustring = basic_string<var::uchar>;
using sstring = basic_string<var::schar>;
using wstring = basic_string<var::wchar>;

#ifdef _FE_HAS_CXX20_
using string8 = basic_string<var::UTF8>;
#endif

using string16 = basic_string<var::UTF16>;
using string32 = basic_string <var::UTF32>;


template<>
struct is_string_class<FE::string>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<FE::ustring>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<FE::sstring>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<FE::wstring>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _FE_HAS_CXX20_
template<>
struct is_string_class<FE::string8>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_string_class<FE::string16>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<FE::string32>
{
    _FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


END_NAMESPACE
#pragma warning(pop)
#endif
#undef _NULL_ESCAPE_SIZE_
