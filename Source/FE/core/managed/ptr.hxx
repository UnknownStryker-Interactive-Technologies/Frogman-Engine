#ifndef _FE_CORE_PTR_HXX_
#define _FE_CORE_PTR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/managed/private/ref_table.hpp>
#include <FE/core/iterator.hxx>




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
<<<<<<< HEAD
	using const_pointer = element_type* const;
=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

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

	_FORCE_INLINE_ element_type& operator*() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
<<<<<<< HEAD
	_FORCE_INLINE_ pointer get() noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ const_pointer get() const noexcept
=======
	_FORCE_INLINE_ pointer get() const noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept
	{
		return this->m_ref_block == other_p.m_ref_block;
	}

	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept
	{
		return this->m_ref_block != other_p.m_ref_block;
	}

	_FORCE_INLINE_ boolean operator>(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator>=(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator<(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator<=(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		--(this->m_ref_block->_ref_count);

		if(this->m_ref_block->_address == nullptr && this->m_ref_block->_ref_count == 0)
		{
			internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
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
<<<<<<< HEAD
	using const_pointer = element_type* const;
=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

private:
	mutable smart_ptr_type m_ref_block;
	mutable pointer m_smart_ptr_end;

public:
	_CONSTEXPR20_ ptr() noexcept : m_ref_block(), m_smart_ptr_end() {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~ptr() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const ptr& other_p) noexcept : m_ref_block(other_p.m_ref_block), m_smart_ptr_end(other_p.m_smart_ptr_end)
	{
		if (other_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr(ptr&& rvalue_p) noexcept : m_ref_block(rvalue_p.m_ref_block), m_smart_ptr_end(rvalue_p.m_smart_ptr_end)
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			rvalue_p.m_ref_block = nullptr;
		}
	}

	template<class Allocator>
	_FORCE_INLINE_ _CONSTEXPR20_ ptr(const FE::exclusive_ptr<T[], Allocator>& exclusive_ptr_p) noexcept : m_ref_block(exclusive_ptr_p.m_ref_block), m_smart_ptr_end(exclusive_ptr_p.m_smart_ptr_end)
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
			this->m_smart_ptr_end = other_p.m_smart_ptr_end;
		}
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ptr& operator=(ptr&& rvalue_p) noexcept
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			this->m_ref_block = rvalue_p.m_ref_block;
			rvalue_p.m_ref_block = nullptr;
			this->m_smart_ptr_end = rvalue_p.m_smart_ptr_end;
			rvalue_p.m_smart_ptr_end = nullptr;
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
		this->m_smart_ptr_end = exclusive_ptr_p.m_smart_ptr_end;
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		this->m_smart_ptr_end = nullptr;

		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

 	_FORCE_INLINE_ void swap(ptr& in_out_other_p) noexcept
	{
		this->m_ref_block = algorithm::utility::exchange<smart_ptr_type>(in_out_other_p.m_ref_block, nullptr);
		this->m_smart_ptr_end = algorithm::utility::exchange<pointer>(in_out_other_p.m_smart_ptr_end, nullptr);
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

<<<<<<< HEAD
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
	_FORCE_INLINE_ const_pointer get() const noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ element_type& operator*() noexcept
=======
	_FORCE_INLINE_ element_type& operator*() const noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to dereference nullptr.");
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

<<<<<<< HEAD
	_FORCE_INLINE_ pointer operator->() noexcept
=======
	_FORCE_INLINE_ pointer operator->() const noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
<<<<<<< HEAD
	_FORCE_INLINE_ pointer get() noexcept
=======
	_FORCE_INLINE_ pointer get() const noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}
	
	_FORCE_INLINE_ size capacity() const noexcept
	{
		if(this->m_ref_block == nullptr) { return 0; }
		return this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept
	{
		return this->m_ref_block == other_p.m_ref_block;
	}

	_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept
	{
		return this->m_ref_block != other_p.m_ref_block;
	}

	_FORCE_INLINE_ boolean operator>(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator>=(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator<(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator<=(const ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator==(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator!=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator>=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	template<class Allocator>
	_FORCE_INLINE_ boolean operator<=(const FE::exclusive_ptr<T, Allocator>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ element_type& operator[](index_t index_p) const noexcept
	{		
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT((this->m_ref_block->_address + index_p) >= this->m_smart_ptr_end, "Assertion failure: unable to access nullptr.");
		return static_cast<pointer>(this->m_ref_block->_address)[index_p];
	}

	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> begin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::iterator<FE::contiguous_iterator<element_type>> end() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cbegin() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->(); 
	}
	_FORCE_INLINE_ FE::const_iterator<FE::contiguous_iterator<element_type>> cend() const noexcept
	{
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rend() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end;
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crbegin() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->operator->();
	}
	_FORCE_INLINE_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crend() const noexcept
	{ 
		FE_ASSERT(this->is_expired() == true, "${%s@0}: ${%s@1} is invalid", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_smart_ptr));
		return this->m_smart_ptr_end; 
	}

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		--(this->m_ref_block->_ref_count);

		if(this->m_ref_block->_address == nullptr && this->m_ref_block->_ref_count == 0)
		{
			internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
			this->m_ref_block = nullptr;
		}
	}
};


END_NAMESPACE
#endif