#ifndef _FE_CORE_PTR_HXX_
#define _FE_CORE_PTR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/smart_pointers/private/smart_ptr_variants_base.hpp>
#include <FE/core/iterator.hxx>




BEGIN_NAMESPACE(FE)


template<typename T, class Allocator>
class exclusive_ptr;


template<typename T>
class ptr final : public internal::smart_ptr::ptr_base
{
public:
	using base_type = internal::smart_ptr::ptr_base;
	using ref_table_key_type = typename base_type::ref_table_key_type;

	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;

private:
	mutable element_type* m_smart_ptr;
	mutable ref_table_key_type m_key;

public:
	_CONSTEXPR20_ ptr() noexcept : m_smart_ptr(), m_key(base_type::invalid_key_value) {}

	_CONSTEXPR20_ ptr(const ptr& other_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = other_p.m_key;
		}
	}

	_CONSTEXPR20_ ptr(ptr&& rvalue_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			rvalue_p.m_smart_ptr = nullptr;
			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
	}

	template<class Allocator>
	_CONSTEXPR20_ ptr(const FE::exclusive_ptr<T, Allocator>& exclusive_ptr_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(exclusive_ptr_p.get_key()))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = exclusive_ptr_p.get_key();
		}
	}

	_CONSTEXPR20_ ptr& operator=(const ptr& other_p) noexcept
	{
		if (other_p.m_smart_ptr == nullptr)
		{
			return *this;
		}

		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = other_p.m_key;
		}
		return *this;
	}

	_CONSTEXPR20_ ptr& operator=(ptr&& rvalue_p) noexcept
	{
		if (rvalue_p.m_smart_ptr == nullptr)
		{
			return *this;
		}

		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key));
		if (this->m_smart_ptr != nullptr)
		{
			rvalue_p.m_smart_ptr = nullptr;

			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
		return *this;
	}

	template<class Allocator>
	_CONSTEXPR20_ ptr& operator=(const FE::exclusive_ptr<T, Allocator>& exclusive_ptr_p) noexcept
	{
		if (!exclusive_ptr_p)
		{
			return *this;
		}
		
		ref_table_key_type l_retrieved_ref_table_key = exclusive_ptr_p.get_key();
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(l_retrieved_ref_table_key));
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = l_retrieved_ref_table_key;
		}
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		this->m_smart_ptr = nullptr;
		this->m_key = base_type::invalid_key_value;
	}

	_FORCE_INLINE_ ref_table_key_type get_key() const noexcept
	{
		return this->m_key;
	}

 	_CONSTEXPR20_ void swap(ptr& in_out_other_p) noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		in_out_other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(in_out_other_p.m_key));

		pointer l_temporary_smart_ptr = algorithm::utility::exchange<pointer>(in_out_other_p.m_smart_ptr, this->m_smart_ptr);
		this->m_smart_ptr = l_temporary_smart_ptr;

		ref_table_key_type l_temporary_key = algorithm::utility::exchange<ref_table_key_type>(in_out_other_p.m_key, this->m_key);
		this->m_key = l_temporary_key;
	}


	_FORCE_INLINE_ boolean is_expired() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr == nullptr;
	}

	_FORCE_INLINE_ pointer get_unchecked() const noexcept
	{
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr;
	}

	_FORCE_INLINE_ explicit operator bool() const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator!() const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ element_type& operator*() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is pointing to an invalid object", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return *(this->m_smart_ptr);
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is pointing to an invalid object", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr;
	}


	_FORCE_INLINE_ boolean operator==(std::nullptr_t nullptr_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr == nullptr_p;
	}

	_FORCE_INLINE_ boolean operator!=(std::nullptr_t nullptr_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr != nullptr_p;
	}


	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr == other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr != other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator>(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr > other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator>=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr >= other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator<(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr < other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator<=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr <= other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr == other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr != other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr > other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr >= other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr < other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr <= other_p.m_smart_ptr;
	}
};




template<typename T>
class ptr<T[]> final : public internal::smart_ptr::ptr_base
{
	using base_type = internal::smart_ptr::ptr_base;
	using ref_table_key_type = typename base_type::ref_table_key_type;

public:
	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;

private:
	mutable element_type* m_smart_ptr;
	mutable pointer m_smart_ptr_end;
	mutable ref_table_key_type m_key;

public:
	_CONSTEXPR20_ ptr() noexcept : m_smart_ptr(), m_smart_ptr_end(), m_key(base_type::invalid_key_value) {}

	_CONSTEXPR20_ ptr(const ptr& other_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key))), m_smart_ptr_end(other_p.m_smart_ptr_end), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = other_p.m_key;
		}
	}

	_CONSTEXPR20_ ptr(ptr&& rvalue_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key))), m_smart_ptr_end(rvalue_p.m_smart_ptr_end), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			rvalue_p.m_smart_ptr = nullptr;
			rvalue_p.m_smart_ptr_end = nullptr;

			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
	}

	template<class Allocator>
	_CONSTEXPR20_ ptr(const FE::exclusive_ptr<T[], Allocator>& exclusive_ptr_p) noexcept : m_smart_ptr(reinterpret_cast<pointer>(base_type::__get_property_address(exclusive_ptr_p.get_key()))), m_smart_ptr_end(), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = exclusive_ptr_p.get_key();
			this->m_smart_ptr_end = exclusive_ptr_p.get() + exclusive_ptr_p.capacity();
		}
	}

	_CONSTEXPR20_ ptr& operator=(const ptr& other_p) noexcept
	{
		if (other_p.m_smart_ptr == nullptr)
		{
			return *this;
		}

		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		this->m_smart_ptr_end = other_p.m_smart_ptr_end;
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = other_p.m_key;
		}
		return *this;
	}

	_CONSTEXPR20_ ptr& operator=(ptr&& rvalue_p) noexcept
	{
		if (rvalue_p.m_smart_ptr == nullptr)
		{
			return *this;
		}

		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key));
		this->m_smart_ptr_end = rvalue_p.m_smart_ptr_end;
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
		return *this;
	}

	template<class Allocator>
	_CONSTEXPR20_ ptr& operator=(const FE::exclusive_ptr<T[], Allocator>& exclusive_ptr_p) noexcept
	{
		if (!exclusive_ptr_p)
		{
			return *this;
		}

		ref_table_key_type l_retrieved_ref_table_key = exclusive_ptr_p.get_key();
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(l_retrieved_ref_table_key));
		if (this->m_smart_ptr != nullptr)
		{
			this->m_key = l_retrieved_ref_table_key;
			this->m_smart_ptr_end = this->m_smart_ptr + exclusive_ptr_p.capacity();
		}
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		this->m_smart_ptr = nullptr;
		this->m_smart_ptr_end = nullptr;
		this->m_key = base_type::invalid_key_value;
	}

	_FORCE_INLINE_ ref_table_key_type get_key() const noexcept
	{
		return this->m_key;
	}

	_FORCE_INLINE_ size_t capacity() const noexcept
	{
		return this->m_smart_ptr_end - this->m_smart_ptr;
	}

	_CONSTEXPR20_ void swap(ptr& in_out_other_p) noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		in_out_other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(in_out_other_p.m_key));

		pointer l_temporary_smart_ptr = algorithm::utility::exchange<pointer>(in_out_other_p.m_smart_ptr, this->m_smart_ptr);
		this->m_smart_ptr = l_temporary_smart_ptr;
		
		pointer l_temporary_smart_ptr_end = algorithm::utility::exchange<pointer>(in_out_other_p.m_smart_ptr_end, this->m_smart_ptr_end);
		this->m_smart_ptr_end = l_temporary_smart_ptr_end;

		ref_table_key_type l_temporary_key = algorithm::utility::exchange<ref_table_key_type>(in_out_other_p.m_key, this->m_key);
		this->m_key = l_temporary_key;
	}


	_FORCE_INLINE_ boolean is_expired() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr == nullptr;
	}

	_FORCE_INLINE_ pointer get_unchecked() const noexcept
	{
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr;
	}

	_FORCE_INLINE_ explicit operator bool() const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator!() const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ element_type& operator*() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return *(this->m_smart_ptr);
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr;
	}

	_FORCE_INLINE_ element_type& operator[](index_t index_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ptr == nullptr, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		FE_ASSERT(static_cast<index_t>(this->m_smart_ptr_end - this->m_smart_ptr) <= index_p, "${%s@0}: ${%s@1} exceeds the index boundary. ${%s@1} was ${%lu@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(index_p), &index_p);
		return this->m_smart_ptr[index_p];
	}


	_FORCE_INLINE_ boolean operator==(std::nullptr_t nullptr_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr == nullptr_p;
	}

	_FORCE_INLINE_ boolean operator!=(std::nullptr_t nullptr_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ptr != nullptr_p;
	}


	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr == other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr != other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator>(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr > other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator>=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr >= other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator<(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr < other_p.m_smart_ptr;
	}

	_FORCE_INLINE_ boolean operator<=(const ptr& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ptr <= other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr == other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr != other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr > other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr >= other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr < other_p.m_smart_ptr;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		this->m_smart_ptr = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		 
		return this->m_smart_ptr <= other_p.m_smart_ptr;
	}


	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> begin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> end() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cbegin() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->(); 
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cend() const noexcept
	{
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rend() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crend() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end; 
	}
};


END_NAMESPACE
#endif