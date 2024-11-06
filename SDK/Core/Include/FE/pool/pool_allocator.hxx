#ifndef _FE_CORE_POOL_ALLOCATOR_HXX_
#define _FE_CORE_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/pool/pool.hxx>

#include <FE/private/allocator_base.hpp>




BEGIN_NAMESPACE(FE)


template <typename T, class PageCapacity = FE::size_in_bytes<8192>, class Alignment = FE::SIMD_auto_alignment>
class pool_allocator : public FE::internal::allocator_base 
{
	FE_NEGATIVE_STATIC_ASSERT(PageCapacity::size == 0, "Static Assertion Failure: The PageCapacity is 0.");
	FE_STATIC_ASSERT((PageCapacity::size % Alignment::size) == 0, "Static Assertion Failure: The PageCapacity must be a multiple of Alignment::size.");
	FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

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

	_FE_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_FE_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<alignment_type, FE::SIMD_auto_alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	pool_type* m_pool;
	
public:
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pool_allocator() noexcept : m_pool() 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pool_allocator(pool_type* const pool_p) noexcept : m_pool(pool_p) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pool_allocator(const pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pool_allocator(pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	template <typename U>
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pool_allocator(_FE_MAYBE_UNUSED_ const pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.get_pool()) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ ~pool_allocator() noexcept {}


	_FE_FORCE_INLINE_ pool_allocator& operator=(const pool_allocator&) noexcept = delete;
	_FE_FORCE_INLINE_ pool_allocator& operator=(const pool_allocator&&) noexcept = delete;


	_FE_FORCE_INLINE_ void set_pool(pool_type* const pool_p) noexcept 
	{
		this->m_pool = pool_p; 
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ pool_type* get_pool() noexcept 
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_pool;
	}

	_FE_FORCE_INLINE_ pool_type* const get_pool() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_pool;
	}


	_FE_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept 
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_pool->create_pages(count_p); 
	}

	_FE_FORCE_INLINE_ void shrink_to_fit() noexcept 
	{ 
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_pool->shrink_to_fit();
	}


	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		return (T*)this->m_pool->template allocate<var::byte>(sizeof(T) * count_p);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		if (new_count_p == 0)
		{
			this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * prev_count_p);
			return nullptr;
		}

		pointer l_result = (T*)this->m_pool->template allocate<var::byte>(sizeof(T) * new_count_p);

		if (pointer_p != nullptr) _FE_LIKELY_
		{
			FE::memcpy<is_address_aligned, is_address_aligned>(l_result, sizeof(T) * new_count_p, pointer_p, sizeof(T) * prev_count_p);
			this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * prev_count_p);
		}

		return l_result;
	}

	_FE_FORCE_INLINE_ void deallocate(pointer const pointer_p, _FE_MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		this->m_pool->template deallocate<var::byte>(reinterpret_cast<var::byte* const>(pointer_p), sizeof(T) * count_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator==(_FE_MAYBE_UNUSED_ const pool_allocator& other_p) noexcept
	{
		return true;
	}
#ifndef _FE_HAS_CXX23_
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator!=(_FE_MAYBE_UNUSED_ const pool_allocator& other_p) noexcept
	{
		return false;
	}
#endif
};




template <typename T, class PageCapacity = FE::size_in_bytes<8192>, class Alignment = FE::SIMD_auto_alignment>
class new_delete_pool_allocator : public FE::internal::allocator_base 
{
	FE_NEGATIVE_STATIC_ASSERT(PageCapacity::size == 0, "Static Assertion Failure: The PageCapacity is 0.");
	FE_STATIC_ASSERT((PageCapacity::size % Alignment::size) == 0, "Static Assertion Failure: The PageCapacity must be a multiple of Alignment::size.");
	FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

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

	_FE_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_FE_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<alignment_type, FE::SIMD_auto_alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:
	pool_type* m_pool;
	
public:
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ new_delete_pool_allocator() noexcept : m_pool() 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ new_delete_pool_allocator(pool_type* const pool_p) noexcept : m_pool(pool_p) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ new_delete_pool_allocator(const new_delete_pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ new_delete_pool_allocator(new_delete_pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) 
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	template <typename U>
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ new_delete_pool_allocator(_FE_MAYBE_UNUSED_ const new_delete_pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.get_pool())
	{
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ ~new_delete_pool_allocator() noexcept {}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ new_delete_pool_allocator& operator=(const new_delete_pool_allocator&) noexcept = delete;
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ new_delete_pool_allocator& operator=(const new_delete_pool_allocator&&) noexcept = delete;


	_FE_FORCE_INLINE_ void set_pool(pool_type* const pool_p) noexcept
	{
		this->m_pool = pool_p;
		FE_LOG_IF(this->m_pool == nullptr, "Warning: the new_delete_pool_allocator has no address to a memory pool instance.");
	}

	_FE_FORCE_INLINE_ pool_type* get_pool() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_pool;
	}

	_FE_FORCE_INLINE_ pool_type* const get_pool() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		return this->m_pool;
	}


	_FE_FORCE_INLINE_ void create_pages(const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_pool->create_pages(count_p);
	}

	_FE_FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		this->m_pool->shrink_to_fit();
	}


	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer allocate(const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		return this->m_pool->template allocate<value_type>(count_p);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ pointer reallocate(pointer const pointer_p, const size_type prev_count_p, const size_type new_count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");
		if (new_count_p == 0)
		{
			this->m_pool->template deallocate<value_type>(pointer_p, prev_count_p);
			return nullptr;
		}

		pointer l_result = this->m_pool->template allocate<value_type>(new_count_p);

		if constexpr (is_trivial == false)
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

	_FE_FORCE_INLINE_ void deallocate(pointer const pointer_p, _FE_MAYBE_UNUSED_ const size_type count_p) noexcept
	{
		FE_NEGATIVE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_NEGATIVE_ASSERT(this->m_pool == nullptr, "Assertion failure: Unable to dereference a null pointer.");

		this->m_pool->template deallocate<value_type>(pointer_p, count_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator==(_FE_MAYBE_UNUSED_ const new_delete_pool_allocator&) noexcept
	{
		return true;
	}
#ifndef _FE_HAS_CXX23_
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean operator!=(_FE_MAYBE_UNUSED_ const new_delete_pool_allocator&) noexcept
	{
		return false;
	}
#endif
};


END_NAMESPACE
#endif