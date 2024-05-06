#ifndef _FE_CORE_POOL_ALLOCATOR_HXX_
#define _FE_CORE_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/allocator.hxx>
#include <FE/core/private/pool_allocator_base.hxx>




BEGIN_NAMESPACE(FE)


template <typename T, class Alignment = FE::SIMD_auto_alignment>
class pool_allocator final : public pool_allocator_base<Alignment>
{
public:
	using base_type = pool_allocator_base<Alignment>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;
	using alignment_type = Alignment;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<alignment_type, FE::SIMD_auto_alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr pool_allocator() noexcept {}

	template <typename U = T>
	constexpr pool_allocator(_MAYBE_UNUSED_ const pool_allocator<U, Alignment>& other_p) noexcept {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool->create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool->shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return (T*)base_type::s_pool->allocate<var::byte>(FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p)).release();
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			base_type::s_pool->deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(prev_count_p));
			return nullptr;
		}

		pointer l_result = (T*)base_type::s_pool->allocate<var::byte>(FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(new_count_p)).release();

		if (pointer_p != nullptr) _LIKELY_
		{
			FE::memcpy<is_address_aligned, is_address_aligned>(l_result, sizeof(T) * new_count_p, pointer_p, sizeof(T) * prev_count_p);
			base_type::s_pool->deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(prev_count_p));
		}

		return l_result;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool->deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p));
	}
};




template <typename T, class Alignment = FE::SIMD_auto_alignment>
class namespace_pool_allocator final : public pool_allocator_base<Alignment>
{
	const FE::memory_region_t m_namespace;

public:
	using base_type = pool_allocator_base<Alignment>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using difference_type = var::ptrdiff_t;
	using size_type = var::size_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, Alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr namespace_pool_allocator(const char* namespace_p = "global") noexcept : m_namespace(namespace_p) {}

	template <typename U = T>
	constexpr namespace_pool_allocator(const namespace_pool_allocator<U, Alignment>& other_p) noexcept : m_namespace(other_p.get_namespace()) {}


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		base_type::s_pool->create_pages(this->m_namespace.data(), count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		base_type::s_pool->shrink_to_fit(this->m_namespace.data());
	}

	_FORCE_INLINE_ const char* get_namespace() const noexcept
	{
		return this->m_namespace.data();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return (T*)base_type::s_pool->allocate<var::byte>(this->m_namespace.data(), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p)).release();
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			base_type::s_pool->deallocate<var::byte>(this->m_namespace.data(), reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(prev_count_p));
			return nullptr;
		}

		pointer l_result = (T*)base_type::s_pool->allocate<var::byte>(this->m_namespace.data(), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(new_count_p)).release();

		if (pointer_p != nullptr) _LIKELY_
		{
			FE::memcpy<is_address_aligned, is_address_aligned>(l_result, sizeof(T) * new_count_p, pointer_p, sizeof(T) * prev_count_p);
			base_type::s_pool->deallocate<var::byte>(this->m_namespace.data(), reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(prev_count_p));
		}

		return l_result;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		base_type::s_pool->deallocate<var::byte>(this->m_namespace.data(), reinterpret_cast<var::byte* const>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p));
	}
};




END_NAMESPACE
#endif