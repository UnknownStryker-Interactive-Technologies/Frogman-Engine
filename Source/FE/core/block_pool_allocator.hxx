#ifndef _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
#define _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool.hxx>
#include <FE/core/private/pool_common.hxx>




BEGIN_NAMESPACE(FE)


template <typename T, class PageCapacity = FE::object_count<128>>
class new_delete_block_pool_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using page_capacity = PageCapacity;
	using pool_type = FE::block_pool<sizeof(T), PageCapacity::size>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;
	using alignment_type = FE::align_custom_bytes<sizeof(T)>;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	std::shared_ptr<pool_type> m_pool;

public:
	_CONSTEXPR20_ new_delete_block_pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_CONSTEXPR20_ new_delete_block_pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_CONSTEXPR20_ new_delete_block_pool_allocator(const new_delete_block_pool_allocator<T, PageCapacity>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_CONSTEXPR20_ new_delete_block_pool_allocator(new_delete_block_pool_allocator<T, PageCapacity>&& other_p) noexcept : m_pool(other_p.m_pool) {}
	_FORCE_INLINE_ ~new_delete_block_pool_allocator() noexcept {};

	template <typename U>
	_CONSTEXPR20_ new_delete_block_pool_allocator(_MAYBE_UNUSED_ const new_delete_block_pool_allocator<U, PageCapacity>& other_p) noexcept : m_pool(base_type::get_default_resource< FE::block_pool<sizeof(U), PageCapacity::size> >()) {}

	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_block_pool_allocator& operator=(const new_delete_block_pool_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_block_pool_allocator& operator=(const new_delete_block_pool_allocator&&) noexcept { return *this; };


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		this->m_pool->create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_pool->shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);

		return this->m_pool->allocate();
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));

		this->m_pool->deallocate(pointer_p);
	}


	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const new_delete_block_pool_allocator& other_p) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const new_delete_block_pool_allocator& other_p) noexcept
	{
		return false;
	}
#endif
};




template <typename T, class PageCapacity = FE::object_count<128>>
class block_pool_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using page_capacity = PageCapacity;
	using pool_type = FE::block_pool<sizeof(T), PageCapacity::size>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;
	using alignment_type = FE::align_custom_bytes<sizeof(T)>;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	std::shared_ptr<pool_type> m_pool;

public:
	_CONSTEXPR20_ block_pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_CONSTEXPR20_ block_pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_CONSTEXPR20_ block_pool_allocator(const block_pool_allocator<T, PageCapacity>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_CONSTEXPR20_ block_pool_allocator(block_pool_allocator<T, PageCapacity>&& other_p) noexcept : m_pool(other_p.m_pool) {}

	_FORCE_INLINE_ ~block_pool_allocator() noexcept {};

	template <typename U>
	_CONSTEXPR20_ block_pool_allocator(_MAYBE_UNUSED_ const block_pool_allocator<U, PageCapacity>& other_p) noexcept : m_pool(base_type::get_default_resource< FE::block_pool<sizeof(U), PageCapacity::size> >()) {}

	_FORCE_INLINE_ _CONSTEXPR17_ block_pool_allocator& operator=(const block_pool_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ block_pool_allocator& operator=(const block_pool_allocator&&) noexcept { return *this; };


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		this->m_pool->create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_pool->shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);

		return (T*)this->m_pool->template allocate<internal::pool::uninitialized_bytes<sizeof(T)>* const>();
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));

		this->m_pool->template deallocate<internal::pool::uninitialized_bytes<sizeof(T)>>(reinterpret_cast<internal::pool::uninitialized_bytes<sizeof(T)>* const>(pointer_p));
	}
	

	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const block_pool_allocator& other_p) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const block_pool_allocator& other_p) noexcept
	{
		return false;
	}
#endif
};


END_NAMESPACE
#endif