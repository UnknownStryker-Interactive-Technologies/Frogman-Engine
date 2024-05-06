#ifndef _FE_CORE_ALLOCATOR_HXX_
#define _FE_CORE_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/allocator_base.hpp>




BEGIN_NAMESPACE(FE)


template <class Implementation>
class new_delete_allocator : public allocator_base
{
public:
	using base_type = allocator_base;
	using value_type = typename Implementation::value_type;
	using pointer = typename Implementation::pointer;
	using const_pointer = typename Implementation::const_pointer;
	using reference = typename Implementation::reference;
	using const_reference = typename Implementation::const_reference;
	using size_type = typename Implementation::size_type;
	using difference_type = typename Implementation::difference_type;
	using allocator = Implementation;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = Implementation::is_address_aligned;

private:
	static allocator s_allocator;

public:
	constexpr new_delete_allocator() noexcept {}

	template <typename AnotherImplementation = Implementation>
	constexpr new_delete_allocator(_MAYBE_UNUSED_ const new_delete_allocator<AnotherImplementation>& other_p) noexcept {}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		pointer const l_result = s_allocator.allocate(count_p);

		if constexpr (is_trivial == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			pointer const l_end = l_result + count_p;
			for (pointer begin = l_result; begin != l_end; ++begin)
			{
				new(begin) value_type();
			}
		}
		return l_result;
	}


	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer pointer_p, size_type prev_count_p, size_type new_count_p) noexcept
	{
		pointer const l_result = s_allocator.reallocate(pointer_p, prev_count_p, new_count_p);

		if constexpr (is_trivial == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			if (new_count_p > prev_count_p)
			{
				pointer const l_end = l_result + new_count_p;
				for (pointer begin = l_result + prev_count_p; begin != l_end; ++begin)
				{
					new(begin) value_type();
				}
			}
			else
			{
				pointer const l_end = pointer_p + prev_count_p;
				for (pointer begin = pointer_p + new_count_p; begin != l_end; ++begin)
				{
					begin->~value_type();
				}
			}
		}

		return l_result;
	}


	_FORCE_INLINE_ void deallocate(pointer pointer_p, size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete ${%p@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), pointer_p);
		
		if constexpr (is_trivial == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			pointer const l_end = pointer_p + count_p;
			for (pointer begin = pointer_p; begin != l_end; ++begin)
			{
				begin->~value_type();
			}
		}
		s_allocator.deallocate(pointer_p, count_p);
	}
};

template <class Implementation>
typename new_delete_allocator<Implementation>::allocator new_delete_allocator<Implementation>::s_allocator;




template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
class aligned_allocator : public allocator_base
{
public:
	using base_type = allocator_base;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = var::size_t;
	using difference_type = var::ptrdiff_t;

	_MAYBE_UNUSED_ static constexpr inline auto is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, Alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	constexpr aligned_allocator() noexcept {}

	template <typename U = T>
	constexpr aligned_allocator(_MAYBE_UNUSED_ const aligned_allocator<U, Alignment>& other_p) noexcept {}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);

		return base_type::trackable_alloc<value_type, Alignment>(count_p * sizeof(value_type));
	}


	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer pointer_p, size_type prev_count_p, size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			base_type::trackable_free<value_type, Alignment>(pointer_p, prev_count_p * sizeof(value_type));
			return nullptr;
		}

		return base_type::trackable_realloc<value_type, Alignment>(pointer_p, prev_count_p * sizeof(value_type), new_count_p * sizeof(value_type));
	}


	_FORCE_INLINE_ void deallocate(pointer pointer_p, size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete ${%p@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), pointer_p);
		
		base_type::trackable_free<value_type, Alignment>(pointer_p, count_p * sizeof(value_type));
	}
};


END_NAMESPACE
#endif