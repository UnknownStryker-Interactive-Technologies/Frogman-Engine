#ifndef _FE_CORE_OWNER_PTR_HXX_
#define _FE_CORE_OWNER_PTR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>

#include <FE/core/algorithm/utility.hxx>
#include <FE/core/iterator.hxx>
#include <FE/core/managed/private/ref_table.hpp>
#include <FE/core/memory.hxx>

#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
#include <FE/core/pool_allocator.hxx>
#endif

// std
#include <initializer_list>




BEGIN_NAMESPACE(FE)


// A smart pointer that has the exclusive ownership over the object it points to.

/* NOTICE: 
   Any objects that contain one of the FE.core.managed pointers and references cannot have static lifetime. (Except for FE::unique_ptr)
   It will result in the segmentation fault(address not mapped to the object) at the end of the program execution.
   
   FE.core.managed pointer and reference: FE::exclusive_ptr, FE::ptr, FE::trackable, and FE::ref.
   FE.core classes associated with those: FE::string, FE::string_view, FE::array, and other FE.core template data containers. 
*/
template <typename T,
#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
	class Allocator = FE::new_delete_pool_allocator<typename std::remove_all_extents<T>::type>
#else
	class Allocator = FE::new_delete_allocator<typename std::remove_all_extents<T>::type>
#endif
>
class exclusive_ptr final
{
	friend class ptr<T>;

	FE_STATIC_ASSERT(std::is_class<Allocator>::value == false, "Static Assertion Failed: The template argument Allocator is not a class type.");
	FE_STATIC_ASSERT((std::is_same<T, typename Allocator::value_type>::value == false), "static assertion failed: enforcing Allocator's value_type to be equivalent to the typename T. The template parameter T must be identical to the value_type of the Allocator.");

	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ptr_type = ref_block_type*;

public:
	using allocator_type = Allocator;
	using element_type = typename allocator_type::value_type;
	using const_pointer = const element_type*;
	using pointer = typename allocator_type::pointer;

private:
	smart_ptr_type m_ref_block;
	_NO_UNIQUE_ADDRESS_ mutable allocator_type m_allocator;

public:
	_FORCE_INLINE_ _CONSTEXPR17_ exclusive_ptr() noexcept : m_ref_block(), m_allocator() {}
	_FORCE_INLINE_ _CONSTEXPR17_ exclusive_ptr(const Allocator& allocator_p) noexcept : m_ref_block(), m_allocator(allocator_p) {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~exclusive_ptr() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(const exclusive_ptr&) noexcept = delete;

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(exclusive_ptr&& rvalue_p) noexcept :  m_ref_block(rvalue_p.m_ref_block), m_allocator(rvalue_p.m_allocator)
	{
		rvalue_p.m_ref_block = nullptr;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(const element_type& value_p, const Allocator& allocator_p) noexcept : m_allocator(allocator_p)
	{
		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(1);
		*static_cast<pointer>(this->m_ref_block->_address) = value_p;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(const element_type& value_p) noexcept : m_allocator()
	{
		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(1);
		*static_cast<pointer>(this->m_ref_block->_address) = value_p;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr& operator=(const exclusive_ptr&) noexcept = delete;

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr& operator=(exclusive_ptr&& rvalue_p) noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}

		this->m_ref_block = algorithm::utility::exchange<smart_ptr_type>(rvalue_p.m_ref_block, nullptr);
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr& operator=(const element_type& value_p) noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			if(this->m_ref_block->_address == nullptr)
			{
				this->m_ref_block->_address = this->m_allocator.allocate(1);
			}
		}
		else
		{
			this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
			this->m_ref_block->_address = this->m_allocator.allocate(1);
		}

		*static_cast<pointer>(this->m_ref_block->_address) = value_p;
		return *this;
	}

	_NODISCARD_ _CONSTEXPR20_ pointer release() noexcept
	{
		pointer l_result = nullptr;
		if (this->m_ref_block != nullptr)
		{
			l_result = static_cast<pointer>(this->m_ref_block->_address);
			this->m_ref_block->_address = nullptr;
			if(this->m_ref_block->_ref_count == 0)
			{
				internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
				this->m_ref_block = nullptr;
			}
		}
		return l_result;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset(const element_type& value_p) noexcept
	{
		*this->operator=(value_p);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void swap(exclusive_ptr& in_out_other_p) noexcept
	{
		exclusive_ptr l_tmp = std::move(in_out_other_p);
		in_out_other_p = std::move(*this);
		*this = std::move(l_tmp);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const allocator_type& get_allocator() const noexcept
	{
		return this->m_allocator;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ allocator_type& get_allocator() noexcept
	{
		return this->m_allocator;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ explicit operator bool() const noexcept
	{
		return ((this->m_ref_block != nullptr) && (this->m_ref_block->_address != nullptr)) ? true : false;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ bool operator!() const noexcept
	{
		return ((this->m_ref_block == nullptr) || (this->m_ref_block->_address == nullptr)) ? true : false;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const element_type& operator*() const noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ element_type& operator*() noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ pointer operator->() noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const_pointer get() const noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ pointer get() noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return this->operator!();
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return !this->operator!();
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}
	
	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ boolean operator>(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>=(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<=(const ptr<T>& other_p) const noexcept
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
		if (this->m_ref_block->_address != nullptr)
		{
			this->m_allocator.deallocate(static_cast<pointer>(this->m_ref_block->_address), 1);
			this->m_ref_block->_address = nullptr;
		}

		if(this->m_ref_block->_ref_count == 0)
		{
			internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
			this->m_ref_block = nullptr;
		}
	}
};

template <typename T,
#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
	class Allocator = FE::new_delete_pool_allocator<typename std::remove_all_extents<T>::type>
#else
	class Allocator = FE::new_delete_allocator<typename std::remove_all_extents<T>::type>
#endif
>
_NODISCARD_ _FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr<T, Allocator> make_exclusive() noexcept
{
	static_assert(std::is_array<T>::value == false, "static assertion failed: The typename T must not be an array type");
	return exclusive_ptr<T, Allocator>(T());
}

template <typename T,
#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
	class Allocator = FE::new_delete_pool_allocator<typename std::remove_all_extents<T>::type>
#else
	class Allocator = FE::new_delete_allocator<typename std::remove_all_extents<T>::type>
#endif
>
_NODISCARD_ _FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr<T, Allocator> make_exclusive(const T& value_p) noexcept
{
	static_assert(std::is_array<T>::value == false, "static assertion failed: The typename T must not be an array type");
	return exclusive_ptr<T, Allocator>(T(value_p));
}




template<typename T, class Allocator>
class exclusive_ptr<T[], Allocator> final
{
	friend class ptr<T[]>;

	FE_STATIC_ASSERT(std::is_class<Allocator>::value == false, "Static Assertion Failed: The template argument Allocator is not a class type.");
	FE_STATIC_ASSERT((std::is_same<T, typename Allocator::value_type>::value == false), "static assertion failed: enforcing Allocator's value_type to be equivalent to the typename T. The template parameter T must be identical to the value_type of the Allocator.");
	
	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ptr_type = ref_block_type*;

public:
	using allocator_type = Allocator;
	using element_type = typename allocator_type::value_type;
	using const_pointer = const element_type*;
	using pointer = typename allocator_type::pointer;

private:	
	smart_ptr_type m_ref_block;
	pointer m_smart_ptr_end;
	_NO_UNIQUE_ADDRESS_ mutable allocator_type m_allocator;

public:
	_FORCE_INLINE_ _CONSTEXPR17_ exclusive_ptr() noexcept : m_ref_block(), m_smart_ptr_end(), m_allocator() {}
	_FORCE_INLINE_ _CONSTEXPR17_ exclusive_ptr(const Allocator& allocator_p) noexcept : m_ref_block(), m_smart_ptr_end(), m_allocator(allocator_p) {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~exclusive_ptr() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_CONSTEXPR20_ exclusive_ptr(const exclusive_ptr&) noexcept = delete;

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(exclusive_ptr&& rvalue_p) noexcept : m_ref_block(rvalue_p.m_ref_block), m_smart_ptr_end(rvalue_p.m_smart_ptr_end), m_allocator(rvalue_p.m_allocator)
	{
		rvalue_p.m_ref_block = nullptr;
		rvalue_p.m_smart_ptr_end = nullptr;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(FE::reserve&& array_size_p, const Allocator& allocator_p) noexcept : m_allocator(allocator_p)
	{
		if (array_size_p == 0)
		{
			return;
		}
		FE_ASSERT(array_size_p == 0, "Assertion failed: the requested array size is zero.");
		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(array_size_p);
		this->m_smart_ptr_end = static_cast<pointer>(this->m_ref_block->_address) + array_size_p;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(FE::reserve&& array_size_p) noexcept : m_allocator()
	{
		if (array_size_p == 0)
		{
			return;
		}
		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(array_size_p);
		this->m_smart_ptr_end = static_cast<pointer>(this->m_ref_block->_address) + array_size_p;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(std::initializer_list<element_type>&& values_p, const Allocator& allocator_p) noexcept : m_allocator(allocator_p)
	{
		size l_initializer_list_size = values_p.size();
		if (l_initializer_list_size == 0)
		{
			return;
		}

		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(l_initializer_list_size);
		this->m_smart_ptr_end = static_cast<pointer>(this->m_ref_block->_address) + l_initializer_list_size;
		this->__copy_from_initializer_list(std::move(values_p));
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr(std::initializer_list<element_type>&& values_p) noexcept : m_allocator()
	{
		size l_initializer_list_size = values_p.size();
		if (l_initializer_list_size == 0)
		{
			return;
		}

		this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		this->m_ref_block->_address = this->m_allocator.allocate(l_initializer_list_size);
		this->m_smart_ptr_end = static_cast<pointer>(this->m_ref_block->_address) + l_initializer_list_size;
		this->__copy_from_initializer_list(std::move(values_p));
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr& operator=(const exclusive_ptr&) noexcept = delete;

	_CONSTEXPR20_ exclusive_ptr& operator=(exclusive_ptr&& rvalue_p) noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}

		this->m_ref_block = algorithm::utility::exchange<smart_ptr_type>(rvalue_p.m_ref_block, nullptr);
		this->m_smart_ptr_end = algorithm::utility::exchange<pointer>(rvalue_p.m_smart_ptr_end, nullptr);
		this->m_allocator = rvalue_p.m_allocator;

		return *this;
	}

	_CONSTEXPR20_ exclusive_ptr& operator=(std::initializer_list<element_type>&& values_p) noexcept
	{
		size l_initializer_list_size = values_p.size();
		if (l_initializer_list_size == 0)
		{
			return *this;
		}

		if(this->m_ref_block == nullptr)
		{
			this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		}

		this->__reallocate(l_initializer_list_size);
		this->__copy_from_initializer_list(std::move(values_p));

		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ exclusive_ptr& operator=(const FE::resize_to new_array_size_p) noexcept
	{
		if(this->m_ref_block == nullptr)
		{
			this->m_ref_block = internal::managed::ref_table::tl_s_ref_block_pool.template allocate<ref_block_type>();
		}

		this->__reallocate(new_array_size_p);

		return *this;
	}

	_NODISCARD_ _CONSTEXPR20_ pointer release() noexcept
	{
		pointer l_result = nullptr;
		if (this->m_ref_block != nullptr)
		{
			l_result = static_cast<pointer>(this->m_ref_block->_address);
			this->m_ref_block->_address = nullptr;
			if(this->m_ref_block->_ref_count == 0)
			{
				internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
				this->m_ref_block = nullptr;
			}
		}
		return l_result;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset(std::initializer_list<element_type>&& values_p) noexcept
	{
		this->operator=(std::move(values_p));
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset(const FE::resize_to new_array_size_p) noexcept
	{
		this->operator=(new_array_size_p);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void swap(exclusive_ptr& in_out_other_p) noexcept
	{
		exclusive_ptr l_tmp = std::move(in_out_other_p);
		in_out_other_p = std::move(*this);
		*this = std::move(l_tmp);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const allocator_type& get_allocator() const noexcept
	{
		return this->m_allocator;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ allocator_type& get_allocator() noexcept
	{
		return this->m_allocator;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ size capacity() const noexcept
	{
		if(this->m_ref_block == nullptr) { return 0; }
		return static_cast<size>(this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address));
	}

	_FORCE_INLINE_ _CONSTEXPR20_ explicit operator bool() const noexcept
	{
		return ((this->m_ref_block != nullptr) && (this->m_ref_block->_address != nullptr)) ? true : false;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ bool operator!() const noexcept
	{
		return ((this->m_ref_block == nullptr) || (this->m_ref_block->_address == nullptr)) ? true : false;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const element_type& operator[](index_t index_p) const noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT(static_cast<index_t>(this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address)) <= index_p, "${%s@0}: ${%s@1} exceeds the index boundary. ${%s@1} was ${%lu@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE), TO_STRING(index_p), &index_p);

		return static_cast<pointer>(this->m_ref_block->_address)[index_p];
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const element_type& operator*() const noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
	_FORCE_INLINE_ _CONSTEXPR20_ const_pointer get() const noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ element_type& operator[](index_t index_p) noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT(static_cast<index_t>(this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address)) <= index_p, "${%s@0}: ${%s@1} exceeds the index boundary. ${%s@1} was ${%lu@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE), TO_STRING(index_p), &index_p);

		return static_cast<pointer>(this->m_ref_block->_address)[index_p];
	}

	_FORCE_INLINE_ _CONSTEXPR20_ element_type& operator*() noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return *static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ pointer operator->() noexcept
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	// this function 'get()' returns nullptr if the current smart pointer is null.
	_FORCE_INLINE_ _CONSTEXPR20_ pointer get() noexcept
	{
		if(this->operator!() == true)
		{
			return nullptr;
		}
		return static_cast<pointer>(this->m_ref_block->_address);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return this->operator!();
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(_MAYBE_UNUSED_ std::nullptr_t nullptr_p) const noexcept
	{
		return !this->operator!();
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}
	
	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<=(const exclusive_ptr& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}


	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address == other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address != other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address > other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator>=(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address >= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address < other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ boolean operator<=(const ptr<T>& other_p) const noexcept
	{
		if(this->m_ref_block == nullptr || other_p->m_ref_block == nullptr)
		{
			return false;
		}
		return this->m_ref_block->_address <= other_p.m_ref_block->_address;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::iterator<FE::contiguous_iterator<element_type>> begin() noexcept
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::iterator<FE::contiguous_iterator<element_type>> end() noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end; 
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_iterator<FE::contiguous_iterator<element_type>> begin() const noexcept
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_iterator<FE::contiguous_iterator<element_type>> end() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end; 
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_iterator<FE::contiguous_iterator<element_type>> cbegin() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_iterator<FE::contiguous_iterator<element_type>> cend() const noexcept 
	{
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end; 
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::reverse_iterator<FE::contiguous_iterator<element_type>> rend() noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> rbegin() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> rend() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crbegin() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return static_cast<pointer>(this->m_ref_block->_address); 
	}
	_FORCE_INLINE_ _CONSTEXPR20_ FE::const_reverse_iterator<FE::contiguous_iterator<element_type>> crend() const noexcept 
	{ 
		FE_ASSERT(this->operator!() == true, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		return this->m_smart_ptr_end; 
	}

private:
	_CONSTEXPR20_ void __copy_from_initializer_list(std::initializer_list<element_type>&& values_p) noexcept
	{
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_TRIVIAL)
		{
			FE::memcpy<allocator_type::is_address_aligned>(static_cast<pointer>(this->m_ref_block->_address), const_cast<pointer>(values_p.begin()), values_p.size() * sizeof(element_type));
		}
		else if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			count_t l_initializer_list_size = values_p.size();

			pointer l_initializer_list_iterator = const_cast<pointer>(values_p.begin());
			pointer l_smart_ptr_iterator = static_cast<pointer>(this->m_ref_block->_address);
			for (var::count_t i = 0; i < l_initializer_list_size; ++i)
			{
				*l_smart_ptr_iterator = std::move(*l_initializer_list_iterator);
				++l_smart_ptr_iterator;
				++l_initializer_list_iterator;
			}
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void __reallocate(size new_count_p) noexcept
	{
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		this->m_ref_block->_address = this->m_allocator.reallocate(static_cast<pointer>(this->m_ref_block->_address), static_cast<size>(this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address)), new_count_p);
		this->m_smart_ptr_end = static_cast<pointer>(this->m_ref_block->_address) + new_count_p;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		if (this->m_ref_block->_address != nullptr)
		{
			this->m_allocator.deallocate(static_cast<pointer>(this->m_ref_block->_address), static_cast<size>(this->m_smart_ptr_end - static_cast<pointer>(this->m_ref_block->_address)));
			this->m_ref_block->_address = nullptr;
		}

		if(this->m_ref_block->_ref_count == 0)
		{
			internal::managed::ref_table::tl_s_ref_block_pool.template deallocate<ref_block_type>(this->m_ref_block);
			this->m_ref_block = nullptr;
		}
	}
};

template <typename T,
#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
	class Allocator = FE::new_delete_pool_allocator<typename std::remove_all_extents<T>::type>
#else
	class Allocator = FE::new_delete_allocator<typename std::remove_all_extents<T>::type>
#endif
>
_NODISCARD_ _FORCE_INLINE_ _CONSTEXPR23_ exclusive_ptr<typename std::remove_all_extents<T>::type[], Allocator> make_exclusive(size array_size_p) noexcept
{
	static_assert(std::is_array<T>::value == true, "static assertion failed: The typename T must be an array type");
	return exclusive_ptr<typename std::remove_all_extents<T>::type[], Allocator>(FE::reserve{ array_size_p });
}

template <typename T,
#ifdef _MEMORY_POOL_FE_EXCLUSIVE_PTR_ALLOCATION_
	class Allocator = FE::new_delete_pool_allocator<typename std::remove_all_extents<T>::type>
#else
	class Allocator = FE::new_delete_allocator<typename std::remove_all_extents<T>::type>
#endif
>
_NODISCARD_ _FORCE_INLINE_ _CONSTEXPR23_ exclusive_ptr<typename std::remove_all_extents<T>::type[], Allocator> make_exclusive(std::initializer_list<typename std::remove_all_extents<T>::type>&& values_p) noexcept
{
	static_assert(std::is_array<T>::value == true, "static assertion failed: The typename T must be an array type");
	return exclusive_ptr<typename std::remove_all_extents<T>::type[], Allocator>(std::move(values_p));
}


END_NAMESPACE
#endif