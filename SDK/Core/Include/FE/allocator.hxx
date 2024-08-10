#ifndef _FE_CORE_ALLOCATOR_HXX_
#define _FE_CORE_ALLOCATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/private/allocator_base.hpp>
#include <FE/type_traits.hxx>

// std
#include <memory>




BEGIN_NAMESPACE(FE)


template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
class new_delete_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using alignment_type = Alignment;

	_MAYBE_UNUSED_ static constexpr inline boolean is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, Alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;

private:

public:
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_allocator() noexcept {}
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_allocator(_MAYBE_UNUSED_ const new_delete_allocator&) noexcept {}
	_FORCE_INLINE_ _CONSTEXPR20_ new_delete_allocator(_MAYBE_UNUSED_ new_delete_allocator&&) noexcept {}

	template<typename U>
	_FORCE_INLINE_ constexpr new_delete_allocator(_MAYBE_UNUSED_ const new_delete_allocator<U, Alignment>&) noexcept {}
	
	_FORCE_INLINE_ ~new_delete_allocator() noexcept {};

	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_allocator& operator=(const new_delete_allocator&) noexcept { return *this; };
	_FORCE_INLINE_ _CONSTEXPR17_ new_delete_allocator& operator=(const new_delete_allocator&&) noexcept { return *this; };


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		
		return new T[count_p];
	}

	_NODISCARD_ pointer reallocate(pointer const pointer_p, size_type prev_count_p, size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			delete[] pointer_p;
			return nullptr;
		}

		pointer const l_result = new T[new_count_p];

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
			delete[] pointer_p;
		}

		return l_result;
	}

	_FORCE_INLINE_ void deallocate(pointer pointer_p, _MAYBE_UNUSED_ size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried deallocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete ${%p@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), pointer_p);
		
		delete[] pointer_p;
	}


	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const new_delete_allocator&) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const new_delete_allocator&) noexcept
	{
		return false;
	}
#endif
};




template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
class aligned_allocator : public FE::internal::allocator_base
{
public:
	using base_type = FE::internal::allocator_base;
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using alignment_type = Alignment;

	_MAYBE_UNUSED_ static constexpr inline boolean is_trivial = FE::is_trivial<value_type>::value;
	_MAYBE_UNUSED_ static constexpr inline ADDRESS is_address_aligned = (std::is_same<FE::SIMD_auto_alignment, Alignment>::value == true) ? ADDRESS::_ALIGNED : ADDRESS::_NOT_ALIGNED;


	_FORCE_INLINE_ constexpr aligned_allocator() noexcept {}

	template <typename U = T>
	_FORCE_INLINE_ constexpr aligned_allocator(_MAYBE_UNUSED_ const aligned_allocator<U, Alignment>&) noexcept {}


	_NODISCARD_ _FORCE_INLINE_ pointer allocate(size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried allocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);

		return base_type::trackable_alloc<value_type, Alignment>(count_p * sizeof(value_type));
	}


	_NODISCARD_ _FORCE_INLINE_ pointer reallocate(pointer pointer_p, size_type prev_count_p, size_type new_count_p) noexcept
	{
		if (new_count_p == 0)
		{
			base_type::trackable_free<value_type, Alignment>(pointer_p, prev_count_p * sizeof(value_type));
			return nullptr;
		}

		pointer const l_result = base_type::trackable_realloc<value_type, Alignment>(pointer_p, prev_count_p * sizeof(value_type), new_count_p * sizeof(value_type));
		
		if constexpr (is_trivial == false)
		{
			if(pointer_p == l_result)
			{
				if (new_count_p > prev_count_p)
				{
					for (pointer begin = l_result + prev_count_p, end = l_result + new_count_p; begin != end; ++begin)
					{
						new(begin) value_type();
					}
				}
				else
				{
					for (pointer begin = pointer_p + new_count_p, end = pointer_p + prev_count_p; begin != end; ++begin)
					{
						begin->~value_type();
					}
				}

				return l_result;
			}
			
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

		return l_result;
	}


	_FORCE_INLINE_ void deallocate(pointer pointer_p, size_type count_p) noexcept
	{
		FE_ASSERT(count_p == 0, "${%s@0}: queried deallocation size is ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), &count_p);
		FE_ASSERT(pointer_p == nullptr, "${%s@0}: attempted to delete ${%p@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), pointer_p);
		
		base_type::trackable_free<value_type, Alignment>(pointer_p, count_p * sizeof(value_type));
	}


	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator==(_MAYBE_UNUSED_ const aligned_allocator&) noexcept
	{
		return true;
	}
#ifndef _HAS_CXX23_
	_FORCE_INLINE_ _CONSTEXPR17_ boolean operator!=(_MAYBE_UNUSED_ const aligned_allocator&) noexcept
	{
		return false;
	}
#endif
};


END_NAMESPACE
#endif