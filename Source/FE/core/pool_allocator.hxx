#ifndef _FE_CORE_POOL_ALLOCATOR_HXX_
#define _FE_CORE_POOL_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/pool.hxx>
#include <FE/core/private/pool_common.hxx>




BEGIN_NAMESPACE(FE)


<<<<<<< HEAD
template <typename T, class PageCapacity = FE::size_in_bytes<4 KB>, class Alignment = FE::SIMD_auto_alignment>
=======
template <typename T, class PageCapacity = FE::size_in_bytes<1 MB>, class Alignment = FE::SIMD_auto_alignment>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
class pool_allocator : public FE::internal::allocator_base 
{
public:
	using base_type = FE::internal::allocator_base;
<<<<<<< HEAD
	using pool_type = FE::scalable_pool<PageCapacity::size, Alignment>;
=======
	using pool_type = FE::dynamic_pool<PageCapacity::size, Alignment>;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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
<<<<<<< HEAD
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(const pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) {}

	template <typename U>
	_FORCE_INLINE_ _CONSTEXPR20_ pool_allocator(_MAYBE_UNUSED_ const pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}

	_FORCE_INLINE_ ~pool_allocator() noexcept {};

	_FORCE_INLINE_ _CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&&) noexcept { return *this; };
=======
	_CONSTEXPR20_ pool_allocator() noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}
	_CONSTEXPR20_ pool_allocator(const std::shared_ptr<pool_type>& pool_p) noexcept : m_pool(pool_p) {}
	_CONSTEXPR20_ pool_allocator(const pool_allocator<T, PageCapacity, Alignment>& other_p) noexcept : m_pool(other_p.m_pool) {}
	_CONSTEXPR20_ pool_allocator(pool_allocator<T, PageCapacity, Alignment>&& other_p) noexcept : m_pool(other_p.m_pool) {}

	template <typename U>
	_CONSTEXPR20_ pool_allocator(_MAYBE_UNUSED_ const pool_allocator<U, PageCapacity, Alignment>& other_p) noexcept : m_pool(base_type::get_default_resource<pool_type>()) {}

	_FORCE_INLINE_ ~pool_allocator() noexcept {};

	_CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&) noexcept { return *this; };
	_CONSTEXPR17_ pool_allocator& operator=(const pool_allocator&&) noexcept { return *this; };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948


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

		pointer l_result = (T*)this->m_pool->template allocate<var::byte>(sizeof(T) * new_count_p).release();

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

END_NAMESPACE
#endif