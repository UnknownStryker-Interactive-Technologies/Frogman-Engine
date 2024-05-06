#ifndef _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
#define _FE_CORE_BLOCK_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool.hxx>
#include <FE/core/allocator.hxx>




BEGIN_NAMESPACE(FE)


template <typename T, class ChunkCapacity = object_count<512>>
class block_pool_allocator_base
{
public:
	using global_allocator = FE::aligned_allocator < internal::pool::chunk< T, POOL_TYPE::_BLOCK, ChunkCapacity::size, FE::align_custom_bytes<sizeof(T)> > >;
	using namespace_allocator = FE::aligned_allocator< std::pair< const FE::memory_region_t, internal::pool::chunk< T, POOL_TYPE::_BLOCK, ChunkCapacity::size, FE::align_custom_bytes<sizeof(T)> > > >;

protected:
	static FE::block_pool<T, ChunkCapacity::size, global_allocator, namespace_allocator> s_pool;

};

template <typename T, class ChunkCapacity>
FE::block_pool<T, ChunkCapacity::size, typename block_pool_allocator_base<T, ChunkCapacity>::global_allocator, typename block_pool_allocator_base<T, ChunkCapacity>::namespace_allocator> block_pool_allocator_base<T, ChunkCapacity>::s_pool;




template <typename T, class ChunkCapacity = object_count<512>>
class new_delete_block_pool_allocator final : public block_pool_allocator_base<T, ChunkCapacity>
{
public:
	using base_type = block_pool_allocator_base<T, ChunkCapacity>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr new_delete_block_pool_allocator() noexcept {}

	template<typename U = T>
	constexpr new_delete_block_pool_allocator(_MAYBE_UNUSED_ const new_delete_block_pool_allocator<U, ChunkCapacity>& other_p) noexcept {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool.create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool.shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return base_type::s_pool.allocate().release();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type prev_count_p = 1, _MAYBE_UNUSED_ const size_type new_count_p = 1) noexcept
	{
		FE_ASSERT(new_count_p > 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &new_count_p);
		
		return pointer_p;
	}


	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool.deallocate(pointer_p);
	}
};




template <typename T, class ChunkCapacity = object_count<512>>
class block_pool_allocator final : public block_pool_allocator_base<FE::internal::pool::uninitialized_bytes<sizeof(T)>, ChunkCapacity>
{
public:
	using base_type = block_pool_allocator_base<FE::internal::pool::uninitialized_bytes<sizeof(T)>, ChunkCapacity>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr block_pool_allocator() noexcept {}

	template <typename U = T>
	constexpr block_pool_allocator(_MAYBE_UNUSED_ const block_pool_allocator<U, ChunkCapacity>& other_p) noexcept {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool.create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool.shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return (T*)base_type::s_pool.allocate().release();
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type prev_count_p = 1, _MAYBE_UNUSED_ const size_type new_count_p = 1) noexcept
	{
		FE_ASSERT(new_count_p > 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &new_count_p);

		return pointer_p;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool.deallocate(reinterpret_cast<FE::internal::pool::uninitialized_bytes<sizeof(T)>* const>(pointer_p));
	}
};




template <typename T, class ChunkCapacity = object_count<512>>
class new_delete_namespace_block_pool_allocator final : public block_pool_allocator_base<T, ChunkCapacity>
{
	const FE::memory_region_t m_namespace;

public:
	using base_type = block_pool_allocator_base<T, ChunkCapacity>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr new_delete_namespace_block_pool_allocator(const char* namespace_p = "global") noexcept : m_namespace(namespace_p) {}

	template <typename U = T>
	constexpr new_delete_namespace_block_pool_allocator(const new_delete_namespace_block_pool_allocator<U, ChunkCapacity>& other_p) noexcept : m_namespace(other_p.get_namespace()) {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool.create_pages(this->m_namespace.data(), count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool.shrink_to_fit(this->m_namespace.data());
	}

	_FORCE_INLINE_ const char* get_namespace() const noexcept
	{
		return this->m_namespace.data();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return base_type::s_pool.allocate(this->m_namespace.data()).release();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type prev_count_p = 1, _MAYBE_UNUSED_ const size_type new_count_p = 1) noexcept
	{
		FE_ASSERT(new_count_p > 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &new_count_p);

		return pointer_p;
	}


	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool.deallocate(this->m_namespace.data(), pointer_p);
	}
};




template <typename T, class ChunkCapacity = object_count<512>>
class namespace_block_pool_allocator final : public block_pool_allocator_base<FE::internal::pool::uninitialized_bytes<sizeof(T)>, ChunkCapacity>
{
	const FE::memory_region_t m_namespace;

public:
	using base_type = block_pool_allocator_base<FE::internal::pool::uninitialized_bytes<sizeof(T)>, ChunkCapacity>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, FE::align_custom_bytes<sizeof(T)>>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr namespace_block_pool_allocator(const char* namespace_p = "global") noexcept : m_namespace(namespace_p) {}

	template <typename U = T>
	constexpr namespace_block_pool_allocator(const namespace_block_pool_allocator<U, ChunkCapacity>& other_p) noexcept : m_namespace(other_p.get_namespace()) {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool.create_pages(this->m_namespace.data(), count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool.shrink_to_fit(this->m_namespace.data());
	}

	_FORCE_INLINE_ const char* get_namespace() const noexcept
	{
		return this->m_namespace.data();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(_MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return (T*)base_type::s_pool.allocate(this->m_namespace.data()).release();
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type prev_count_p = 1, _MAYBE_UNUSED_ const size_type new_count_p = 1) noexcept
	{
		FE_ASSERT(new_count_p > 1, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &new_count_p);

		return pointer_p;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p = 1) noexcept
	{
		FE_ASSERT((count_p > 1) || (count_p == 0), "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool.deallocate(this->m_namespace.data(), reinterpret_cast<FE::internal::pool::uninitialized_bytes<sizeof(T)>* const>(pointer_p));
	}
};




END_NAMESPACE
#endif