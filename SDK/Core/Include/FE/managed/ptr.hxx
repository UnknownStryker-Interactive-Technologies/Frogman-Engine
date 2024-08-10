#ifndef _FE_CORE_PTR_HXX_
#define _FE_CORE_PTR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/algorithm/utility.hxx>
#include <FE/managed/private/ref_block.hxx>
#include <FE/iterator.hxx>




BEGIN_NAMESPACE(FE)


/*
A smart pointer that addresses the dangling pointer issue. It can watch FE::exclusive_ptr's property allocated on heap.
It is a non-owning pointer and its functionality is similar to eastl's smart pointer.
*/
template<typename T>
class ptr final
{
	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ptr_type = ref_block_type*;

public:
	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;
	using const_pointer = const element_type*;

private:
	smart_ptr_type m_ref_block;

public:
	_FORCE_INLINE_ _CONSTEXPR20_ ptr() noexcept : m_ref_block() {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~ptr() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const ptr& other_p) noexcept : m_ref_block(other_p.m_ref_block)
	{
		if (other_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(ptr&& rvalue_p) noexcept : m_ref_block(rvalue_p.m_ref_block)
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			rvalue_p.m_ref_block = nullptr;
		}
	}

	template<class Allocator>
	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const FE::exclusive_ptr<T, Allocator>& exclusive_ptr_p) noexcept : m_ref_block(exclusive_ptr_p.m_ref_block)
	{
		if (exclusive_ptr_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const FE::trackable<T>& trackable_p) noexcept : m_ref_block(trackable_p.m_ref_block)
	{
		if (this->m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(const ptr& other_p) noexcept
	{
		if (other_p.m_ref_block != nullptr)
		{
			this->m_ref_block = other_p.m_ref_block;
			++(this->m_ref_block->_ref_count);
		}
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(ptr&& rvalue_p) noexcept
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			this->m_ref_block = rvalue_p.m_ref_block;
			rvalue_p.m_ref_block = nullptr;
		}
		return *this;
	}

	template<class Allocator>
	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(const FE::exclusive_ptr<T, Allocator>& exclusive_ptr_p) noexcept
	{
		if (exclusive_ptr_p.m_ref_block == nullptr)
		{
			return *this;
		}
		
		this->m_ref_block = exclusive_ptr_p.m_ref_block;
		++(this->m_ref_block->_ref_count);
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(const FE::trackable<T>& trackable_p) noexcept
	{
		if (trackable_p.m_ref_block == nullptr)
		{
			return *this;
		}
		
		this->m_ref_block = trackable_p.m_ref_block;
		++(this->m_ref_block->_ref_count);
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

 	_FORCE_INLINE_ void swap(ptr& in_out_other_p) noexcept
	{
		this->m_ref_block = algorithm::utility::exchange<smart_ptr_type>(in_out_other_p.m_ref_block, nullptr);
	}

	_FORCE_INLINE_ boolean is_expired() const noexcept
	{
		return (this->m_ref_block == nullptr) || (this->m_ref_block->_address == nullptr);
	}

	_FORCE_INLINE_ explicit operator bool() const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator!() const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ element_type& operator*() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ pointer operator->() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ const element_type& operator*() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
	_FORCE_INLINE_ pointer get() noexcept { return (this->m_ref_block == nullptr) ? nullptr : this->m_ref_block->_address; }
	_FORCE_INLINE_ const_pointer get() const noexcept { return (this->m_ref_block == nullptr) ? nullptr : this->m_ref_block->_address; }

	_FORCE_INLINE_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept { return this->is_expired(); }
	_FORCE_INLINE_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept { return !(this->is_expired()); }

	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept { return this->m_ref_block == other_p.m_ref_block; }
	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept { return this->m_ref_block != other_p.m_ref_block; }

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept { return this->m_ref_block == other_p.m_ref_block; }

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept { return this->m_ref_block != other_p.m_ref_block; }

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		--(this->m_ref_block->_ref_count);

		if(this->m_ref_block->_address == nullptr && this->m_ref_block->_ref_count == 0)
		{
			delete this->m_ref_block;
			this->m_ref_block = nullptr;
		}
	}
};


template<typename T>
class ptr<T[]> final
{
	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ptr_type = ref_block_type*;

public:
	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;
	using const_pointer = const element_type*;

private:
	smart_ptr_type m_ref_block;

public:
	_FORCE_INLINE_ _CONSTEXPR20_ ptr() noexcept : m_ref_block() {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~ptr() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const ptr& other_p) noexcept : m_ref_block(other_p.m_ref_block)
	{
		if (other_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(ptr&& rvalue_p) noexcept : m_ref_block(rvalue_p.m_ref_block)
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			rvalue_p.m_ref_block = nullptr;
		}
	}

	template<class Allocator>
	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const FE::exclusive_ptr<T[], Allocator>& exclusive_ptr_p) noexcept : m_ref_block(exclusive_ptr_p.m_ref_block)
	{
		if (exclusive_ptr_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(const ptr& other_p) noexcept
	{
		if (other_p.m_ref_block != nullptr)
		{
			this->m_ref_block = other_p.m_ref_block;
			++(this->m_ref_block->_ref_count);
		}
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(ptr&& rvalue_p) noexcept
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			this->m_ref_block = rvalue_p.m_ref_block;
			rvalue_p.m_ref_block = nullptr;
		}
		return *this;
	}
	
	template<class Allocator>
	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(const FE::exclusive_ptr<T[], Allocator>& exclusive_ptr_p) noexcept
	{
		if (exclusive_ptr_p.m_ref_block == nullptr)
		{
			return *this;
		}
		
		this->m_ref_block = exclusive_ptr_p.m_ref_block;
		++(this->m_ref_block->_ref_count);
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

 	_FORCE_INLINE_ void swap(ptr& in_out_other_p) noexcept
	{
		this->m_ref_block = algorithm::utility::exchange<smart_ptr_type>(in_out_other_p.m_ref_block, nullptr);
	}

	_FORCE_INLINE_ boolean is_expired() const noexcept
	{
		return (this->m_ref_block == nullptr) || (this->m_ref_block->_address == nullptr);
	}

	_FORCE_INLINE_ explicit operator bool() const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator!() const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ element_type& operator*() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ pointer operator->() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ const element_type& operator*() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
	_FORCE_INLINE_ pointer get() noexcept { return (this->m_ref_block == nullptr) ? nullptr : this->m_ref_block->_address; }
	_FORCE_INLINE_ const_pointer get() const noexcept { return (this->m_ref_block == nullptr) ? nullptr : this->m_ref_block->_address; }

	_FORCE_INLINE_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept { return this->is_expired(); }
	_FORCE_INLINE_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept { return !(this->is_expired()); }

	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept { return this->m_ref_block == other_p.m_ref_block; }
	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept { return this->m_ref_block != other_p.m_ref_block; }

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T[], Allocator>& other_p) const noexcept { return this->m_ref_block == other_p.m_ref_block; }

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T[], Allocator>& other_p) const noexcept { return this->m_ref_block != other_p.m_ref_block; }

	_FORCE_INLINE_ element_type& operator[](index_t index_p) noexcept
	{		
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT((this->m_ref_block->_address + index_p) >= this->m_ref_block->_end, "Assertion failure: unable to access nullptr.");
		return this->m_ref_block->_address[index_p];
	}

	_FORCE_INLINE_ const element_type& operator[](index_t index_p) const noexcept
	{		
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT((this->m_ref_block->_address + index_p) >= this->m_ref_block->_end, "Assertion failure: unable to access nullptr.");
		return this->m_ref_block->_address[index_p];
	}

	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> begin() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address;
	}
	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> end() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end;
	}

	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> begin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address;
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> end() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end;
	}

	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cbegin() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address;
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cend() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end;
	}

	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end - 1;
	}
	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rend() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address - 1;
	}

	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end - 1;
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> rend() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address - 1;
	}

	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_end - 1;
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crend() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_ref_block->_address - 1;
	}

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		--(this->m_ref_block->_ref_count);

		if(this->m_ref_block->_address == nullptr && this->m_ref_block->_ref_count == 0)
		{
			delete this->m_ref_block;
			this->m_ref_block = nullptr;
		}
	}
};

END_NAMESPACE
#endif