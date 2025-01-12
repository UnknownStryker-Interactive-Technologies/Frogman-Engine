#ifndef _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
#define _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
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
#include <FE/private/allocator_base.hpp>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/private/pool_common.hxx>




BEGIN_NAMESPACE(FE)

/*
The FE::new_delete_block_pool_allocator is a template class that provides a memory allocator using a block pool strategy
allowing for efficient allocation and deallocation of memory for objects of type T with a specified page capacity.
*/
template <typename T, PoolPageCapacity PageCapacity>
class new_delete_block_pool_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using alignment_type = FE::SIMD_auto_alignment;
	using scalable_pool_type = FE::block_pool<PageCapacity, sizeof(T), alignment_type>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;

	template <typename U>
	struct rebind 
	{
		using other = new_delete_block_pool_allocator<U, PageCapacity>;
	};

	FE_STATIC_ASSERT(FE::is_power_of_two(alignment_type::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

	_FE_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_FE_MAYBE_UNUSED_ static constexpr inline Address is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, alignment_type>::value == true) ? Address::_Aligned : Address::_NotAligned;

private:
	scalable_pool_type* m_scalable_pool;

public:
	_FE_CONSTEXPR20_ new_delete_block_pool_allocator() noexcept : m_scalable_pool() 
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ new_delete_block_pool_allocator(scalable_pool_type* const pool_p) noexcept : m_scalable_pool(pool_p) 
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ new_delete_block_pool_allocator(const new_delete_block_pool_allocator<T, PageCapacity>& other_p) noexcept : m_scalable_pool(other_p.m_scalable_pool)
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ new_delete_block_pool_allocator(new_delete_block_pool_allocator<T, PageCapacity>&& other_p) noexcept : m_scalable_pool(other_p.m_scalable_pool)
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ ~new_delete_block_pool_allocator() noexcept {}

	template <typename U>
	_FE_CONSTEXPR20_ new_delete_block_pool_allocator(_FE_MAYBE_UNUSED_ const new_delete_block_pool_allocator<U, PageCapacity>& other_p) noexcept : m_scalable_pool(base_type::__get_default_pool<scalable_pool_type>())
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ new_delete_block_pool_allocator& operator=(const new_delete_block_pool_allocator&) noexcept = delete;
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ new_delete_block_pool_allocator& operator=(const new_delete_block_pool_allocator&&) noexcept = delete;


	_FE_FORCE_INLINE_ void set_pool(scalable_pool_type* const pool_p) noexcept
	{
		this->m_scalable_pool = pool_p;
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the new_delete_block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ scalable_pool_type* get_pool() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_scalable_pool;
	}

	_FE_FORCE_INLINE_ scalable_pool_type* const get_pool() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_scalable_pool;
	}


	_FE_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_scalable_pool->create_pages(count_p);
	}

	_FE_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_scalable_pool->shrink_to_fit();
	}


	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer allocate(_FE_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT(count_p == 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), &count_p);
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		
		return this->m_scalable_pool->allocate();
	}

	_FE_FORCE_INLINE_ void deallocate(pointer const pointer_p, _FE_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT(count_p == 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), &count_p);
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		
		this->m_scalable_pool->deallocate(pointer_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator==(_FE_MAYBE_UNUSED_ const new_delete_block_pool_allocator<T, PageCapacity>& other_p) noexcept
	{
		return this->m_scalable_pool == other_p.m_scalable_pool;
	}
#ifndef _FE_HAS_CXX23_
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator!=(_FE_MAYBE_UNUSED_ const new_delete_block_pool_allocator<T, PageCapacity>& other_p) noexcept
	{
		return this->m_scalable_pool != other_p.m_scalable_pool;
	}
#endif
};



/*
The FE::block_pool_allocator is a template class that provides a memory allocator for managing blocks of memory in a pool
ensuring efficient allocation and deallocation of objects of type T with a specified page capacity.
*/
template <typename T, PoolPageCapacity PageCapacity>
class block_pool_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using alignment_type = FE::SIMD_auto_alignment;
	using scalable_pool_type = FE::block_pool<PageCapacity, sizeof(T), alignment_type>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;

	template <typename U>
	struct rebind
	{
		using other = block_pool_allocator<U, PageCapacity>;
	};


	FE_STATIC_ASSERT(FE::is_power_of_two(alignment_type::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

	_FE_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_FE_MAYBE_UNUSED_ static constexpr inline Address is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, alignment_type>::value == true) ? Address::_Aligned : Address::_NotAligned;

private:
	scalable_pool_type* m_scalable_pool;

public:
	_FE_CONSTEXPR20_ block_pool_allocator() noexcept : m_scalable_pool() 
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ block_pool_allocator(scalable_pool_type* const pool_p) noexcept : m_scalable_pool(pool_p) 
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ block_pool_allocator(const block_pool_allocator<T, PageCapacity>& other_p) noexcept : m_scalable_pool(other_p.m_scalable_pool)
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_CONSTEXPR20_ block_pool_allocator(block_pool_allocator<T, PageCapacity>&& other_p) noexcept : m_scalable_pool(other_p.m_scalable_pool)
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the block_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ ~block_pool_allocator() noexcept {};

	template <typename U = T>
	_FE_CONSTEXPR20_ block_pool_allocator(_FE_MAYBE_UNUSED_ const block_pool_allocator<U, PageCapacity>& other_p) noexcept : m_scalable_pool(base_type::__get_default_pool<scalable_pool_type>())
	{
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the block_pool_allocator has no address to a memory pool instance.");
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ block_pool_allocator& operator=(const block_pool_allocator&) noexcept = delete;
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ block_pool_allocator& operator=(const block_pool_allocator&&) noexcept = delete;


	_FE_FORCE_INLINE_ void set_pool(scalable_pool_type* const pool_p) noexcept
	{
		this->m_scalable_pool = pool_p;
		FE_LOG_IF(this->m_scalable_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ scalable_pool_type* get_pool() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_scalable_pool;
	}

	_FE_FORCE_INLINE_ scalable_pool_type* const get_pool() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_scalable_pool;
	}


	_FE_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_scalable_pool->create_pages(count_p);
	}

	_FE_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_scalable_pool->shrink_to_fit();
	}


	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer allocate(_FE_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT(count_p == 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), &count_p);
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		
		return (T*)this->m_scalable_pool->template allocate<internal::pool::uninitialized_bytes<sizeof(T)>* const>();
	}

	_FE_FORCE_INLINE_ void deallocate(pointer const pointer_p, _FE_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT(count_p == 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), &count_p);
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		FE_NEGATIVE_ASSERT(this->m_scalable_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		this->m_scalable_pool->template deallocate<internal::pool::uninitialized_bytes<sizeof(T)>>(reinterpret_cast<internal::pool::uninitialized_bytes<sizeof(T)>* const>(pointer_p));
	}
	

	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator==(_FE_MAYBE_UNUSED_ const block_pool_allocator<T, PageCapacity>& other_p) noexcept
	{
		return this->m_scalable_pool == other_p.m_scalable_pool;
	}
#ifndef _FE_HAS_CXX23_
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator!=(_FE_MAYBE_UNUSED_ const block_pool_allocator<T, PageCapacity>& other_p) noexcept
	{
		return this->m_scalable_pool != other_p.m_scalable_pool;
	}
#endif
};


END_NAMESPACE
#endif

