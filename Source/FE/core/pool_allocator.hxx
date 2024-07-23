#ifndef _FE_CORE_POOL_ALLOCATOR_HXX_
#define _FE_CORE_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/pool.hxx>
#include <FE/core/private/pool_common.hxx>




BEGIN_NAMESPACE(FE)


template <typename T, class PageCapacity = FE::size_in_bytes<8 KB>, class Alignment = FE::SIMD_auto_alignment>
class pool_allocator : public FE::internal::allocator_base 
{
public:
	using base_type = FE::internal::allocator_base;
	using pool_type = FE::scalable_pool<PageCapacity::size, Alignment>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;
	using alignment_type = Alignment;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<alignment_type, FE::SIMD_auto_alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	std::shared_ptr<pool_type> m_pool;
	
public:
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(const pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) {}

	template <typename U>
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(_MAYBE_UNUSED_ const pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}

	_FORCE_INLINE_ ~pool_allocator() noexcept {};

	_FORCE_INLINE_ _CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&&) noexcept { return *this; };


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		this->m_pool->create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_pool->shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);

		return (T*)this->m_pool->template allocate<var::byte>(sizeof(T) * count_p);
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * prev_count_p);
			return nullptr;
		}

		pointer l_result = (T*)this->m_pool->template allocate<var::byte>(sizeof(T) * new_count_p);

		if (pointer_p != nullptr) _LIKELY_
		{
			FE::memcpy<is_address_aligned, is_address_aligned>(l_result, sizeof(T) * new_count_p, pointer_p, sizeof(T) * prev_count_p);
			this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * prev_count_p);
		}

		return l_result;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));

		this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * count_p);
	}


	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const pool_allocator& other_p) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const pool_allocator& other_p) noexcept
	{
		return false;
	}
#endif
};


template <typename T, class PageCapacity = FE::size_in_bytes<8 KB>, class Alignment = FE::SIMD_auto_alignment>
class new_delete_pool_allocator : public FE::internal::allocator_base 
{
public:
	using base_type = FE::internal::allocator_base;
	using pool_type = FE::scalable_pool<PageCapacity::size, Alignment>;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = var::ptrdiff;
	using size_type = var::size;
	using alignment_type = Alignment;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<alignment_type, FE::SIMD_auto_alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	std::shared_ptr<pool_type> m_pool;
	
public:
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_pool_allocator(const new_delete_pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_pool_allocator(new_delete_pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) {}

	template <typename U>
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_pool_allocator(_MAYBE_UNUSED_ const new_delete_pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}

	_FORCE_INLINE_ ~new_delete_pool_allocator() noexcept {};

	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_pool_allocator& operator=(const new_delete_pool_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_pool_allocator& operator=(const new_delete_pool_allocator&&) noexcept { return *this; };


	_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		this->m_pool->create_pages(count_p);
	}

	_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_pool->shrink_to_fit();
	}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);

		return this->m_pool->template allocate<value_type>(count_p);
	}

	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			this->m_pool->template deallocate<value_type>(pointer_p, prev_count_p);
			return nullptr;
		}

		pointer l_result = this->m_pool->template allocate<value_type>(new_count_p);

		if constexpr (is_trivial == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			if (new_count_p > prev_count_p)
			{
				for (pointer begin = l_result, end = l_result + prev_count_p, input = pointer_p; begin != end; ++begin)
				{
					new(begin) value_type(std::move(*input));
					++input;
				}

				for (pointer begin = l_result + prev_count_p, end = l_result + new_count_p; begin != end; ++begin)
				{
					new(begin) value_type();
				}
			}
			else
			{
				for (pointer begin = l_result, end = l_result + new_count_p, input = pointer_p; begin != end; ++begin)
				{
					new(begin) value_type(std::move(*input));
					++input;
				}
			}
		}

		if (pointer_p != nullptr)
		{
			this->m_pool->template deallocate<value_type>(pointer_p, prev_count_p);
		}

		return l_result;
	}

	_FORCE_INLINE_ void deallocate(pointer const pointer_p, _MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));

		this->m_pool->template deallocate<value_type>(pointer_p, count_p);
	}


	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const new_delete_pool_allocator&) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const new_delete_pool_allocator&) noexcept
	{
		return false;
	}
#endif
};


END_NAMESPACE
#endif