#ifndef _FE_CORE_FSTACK_HXX_
#define _FE_CORE_FSTACK_HXX_
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
#include <FE/private/memory_traits.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/iterator.hxx>

// std
#include <initializer_list>
#include <utility>




BEGIN_NAMESPACE(FE)


template<class T, size Capacity, class Traits = FE::internal::memory_traits<T>>
class fstack final
{
	FE_NEGATIVE_STATIC_ASSERT((std::is_same<T, typename Traits::value_type>::value == false), "Static Assertion Failed: The template argument T and Traits' value_type have be the same type.");
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Traits>::value == false, "Static Assertion Failed: The template argument Traits is not a class type.");

public:
	using value_type = T;
	using length_type = var::size;
	using size_type = var::size;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using const_iterator = FE::const_iterator<FE::contiguous_iterator<T>>;
	using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<T>>;
	using difference_type = ptrdiff;

private:
	var::byte m_memory[sizeof(value_type) * Capacity];
	pointer m_top_ptr;
	pointer const m_absolute_begin_pointer;

public:
	_FE_FORCE_INLINE_ fstack() noexcept : m_memory(), m_top_ptr(reinterpret_cast<pointer>(m_memory)), m_absolute_begin_pointer(m_top_ptr) {}
	_FE_FORCE_INLINE_ ~fstack() noexcept { this->pop_all(); }

	_FE_CONSTEXPR20_ fstack(std::initializer_list<value_type>&& initializer_list_p) noexcept : m_memory(), m_top_ptr(reinterpret_cast<pointer>(m_memory) + initializer_list_p.size()), m_absolute_begin_pointer(reinterpret_cast<pointer>(m_memory))
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "ERROR!: The length of std::initializer_list exceeds the Capacity");
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));

		Traits::move_construct(this->m_absolute_begin_pointer, const_cast<value_type*>(initializer_list_p.begin()), initializer_list_p.size());
	}

	template<class InputIterator>
	_FE_CONSTEXPR20_ fstack(InputIterator first_p, InputIterator last_p) noexcept : m_memory(), m_top_ptr(reinterpret_cast<pointer>(m_memory) + (last_p - first_p)), m_absolute_begin_pointer(reinterpret_cast<pointer>(m_memory))
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<InputIterator>::value == false, "Static Assertion Failure: The template argument InputIterator must be a class type.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value == false), "Static Assertion Failure: InputIterator's value_type has to be the same as fstack's value_type.");

		FE_NEGATIVE_ASSERT(first_p >= last_p, "${%s@0}: The input iterator ${%s@1} must not be greater than the iterator ${%s@2}.", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_INVALID_ITERATOR), TO_STRING(first_p), TO_STRING(last_p));
		FE_NEGATIVE_ASSERT(static_cast<uint64>(last_p - first_p) > Capacity, "${%s@0}: The input size exceeds the fstack capacity.", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW));

		Traits::copy_construct(InputIterator{ this->m_absolute_begin_pointer }, first_p, last_p - first_p);
	}

	_FE_CONSTEXPR20_ fstack(fstack& other_p) noexcept : m_memory(), m_top_ptr(reinterpret_cast<pointer>(m_memory)), m_absolute_begin_pointer(m_top_ptr)
	{
		if (other_p.is_empty())
		{
			return;
		}

		Traits::copy_construct(this->m_absolute_begin_pointer, this->capacity(), other_p.m_absolute_begin_pointer, other_p.size());

		this->__jump_top_pointer(other_p.m_top_ptr - other_p.m_absolute_begin_pointer);
	}

	_FE_CONSTEXPR20_ fstack(fstack&& rvalue_p) noexcept : m_memory(), m_top_ptr(reinterpret_cast<pointer>(m_memory)), m_absolute_begin_pointer(m_top_ptr)
	{
		if (rvalue_p.is_empty())
		{
			return;
		}

		Traits::move_construct(this->m_absolute_begin_pointer, this->capacity(), rvalue_p.m_absolute_begin_pointer, rvalue_p.size());

		this->__jump_top_pointer(rvalue_p.m_top_ptr - rvalue_p.m_absolute_begin_pointer);
		rvalue_p.__set_top_pointer_to_zero();
	}

	_FE_CONSTEXPR20_ fstack& operator=(std::initializer_list<value_type> initializer_list_p) noexcept
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "ERROR!: The length of std::initializer_list exceeds the Capacity");
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));

		if (this->is_empty())
		{
			this->~fstack();
			new(this) fstack(std::move(initializer_list_p));
			return *this;
		}

		FE::size l_initializer_list_size = initializer_list_p.size();
		this->__restructrue_fstack_with_move_semantics(const_cast<value_type*>(initializer_list_p.begin()), l_initializer_list_size);
		this->__set_top_pointer_to_zero();
		this->__jump_top_pointer(l_initializer_list_size);
		return *this;
	}

	_FE_CONSTEXPR20_ fstack& operator=(fstack& other_p) noexcept
	{
		FE::size l_other_size = other_p.size();
		if (l_other_size == 0)
		{
			return *this;
		}

		if (this->is_empty())
		{
			this->~fstack();
			new(this) fstack(other_p);
			return *this;
		}

		this->__restructrue_fstack_with_copy_semantics(other_p.m_absolute_begin_pointer, l_other_size);

		this->__set_top_pointer_to_zero();
		this->__jump_top_pointer(other_p.m_top_ptr - other_p.m_absolute_begin_pointer);
		return *this;
	}

	_FE_CONSTEXPR20_ fstack& operator=(fstack&& rvalue_p) noexcept
	{
		FE::size l_other_size = rvalue_p.size();
		if (l_other_size == 0)
		{
			return *this;
		}

		if (this->is_empty())
		{
			this->~fstack();
			new(this) fstack(std::move(rvalue_p));
			return *this;
		}

		this->__restructrue_fstack_with_move_semantics(rvalue_p.m_absolute_begin_pointer, l_other_size);

		this->__set_top_pointer_to_zero();
		this->__jump_top_pointer(rvalue_p.m_top_ptr - rvalue_p.m_absolute_begin_pointer);
		rvalue_p.__set_top_pointer_to_zero();
		return *this;
	}

	_FE_FORCE_INLINE_ void push(const value_type& value_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_top_ptr >= this->m_absolute_begin_pointer + Capacity, "${%s@0}: The fstack top exceeded the index boundary", TO_STRING(ErrorCode::_FATAL_MEMORY_ERROR_1XX_ACCESS_VIOLATION));

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			new(this->m_top_ptr) T(value_p);
		}
		else if constexpr (Traits::is_trivial == TypeTriviality::_Trivial)
		{
			*this->m_top_ptr = value_p;
		}

		++this->m_top_ptr;
	}

	_FE_CONSTEXPR20_ value_type pop() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_empty() == true, "${%s@0}: The fstack top index reached zero. The index value_p must be greater than zero", TO_STRING(ErrorCode::_FATAL_MEMORY_ERROR_1XX_ACCESS_VIOLATION));

		--this->m_top_ptr;
		T l_return_value_buffer = std::move(*this->m_top_ptr);

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			this->m_top_ptr->~T();
		}

		return l_return_value_buffer;
	}
	
	_FE_FORCE_INLINE_ void pop_all() noexcept
	{
		if (this->is_empty() == false)
		{
			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial) 
			{
				Traits::destruct(this->m_absolute_begin_pointer, this->m_top_ptr);
			}
			this->__set_top_pointer_to_zero();
		}
	}

	_FE_FORCE_INLINE_ const_reference top() const noexcept
	{
		return *(this->m_top_ptr - 1);
	}

	_FE_FORCE_INLINE_ reference top() noexcept
	{
		return *(this->m_top_ptr - 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::boolean is_empty() const noexcept
	{
		return (this->m_top_ptr == this->m_absolute_begin_pointer) ? true : false;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type count() const noexcept
	{
		return this->m_top_ptr - this->m_absolute_begin_pointer;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type size() const noexcept
	{
		return this->m_top_ptr - this->m_absolute_begin_pointer;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type max_size() const noexcept
	{
		return Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type capacity() const noexcept
	{
		return Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator cbegin() const noexcept
	{
		return this->m_absolute_begin_pointer;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator cend() const noexcept
	{
		return this->m_top_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		return this->m_top_ptr - 1;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		return this->m_absolute_begin_pointer - 1;
	}

	_FE_FORCE_INLINE_ void swap(fstack& in_out_other_p) noexcept
	{
		std::swap(*this, in_out_other_p);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ boolean operator==(const fstack& other_p) const noexcept
	{
		return FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ boolean operator!=(const fstack& other_p) const noexcept
	{
		return !FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

private:
	_FE_FORCE_INLINE_ void __jump_top_pointer(difference_type ptrdiff_p) noexcept
	{
		this->m_top_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ void __set_top_pointer_to_zero() noexcept
	{
		this->m_top_ptr = this->m_absolute_begin_pointer;
	}

	_FE_CONSTEXPR20_ void __restructrue_fstack_with_move_semantics(value_type* const source_begin_p, FE::size source_size_p) noexcept
	{
		FE::size l_this_size = this->size();

		if (source_size_p > l_this_size)
		{
			FE::size l_count_to_construct = source_size_p - l_this_size;
			Traits::move_construct(this->m_top_ptr, source_begin_p - l_count_to_construct, l_count_to_construct);

			Traits::move_assign(this->m_absolute_begin_pointer, source_begin_p, l_this_size);
		}
		else if (source_size_p < l_this_size)
		{
			FE::size l_count_to_destruct = l_this_size - source_size_p;

			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				Traits::destruct(this->m_top_ptr - l_count_to_destruct, this->m_top_ptr);
			}

			Traits::move_assign(this->m_absolute_begin_pointer, source_begin_p, source_size_p);
		}
		else
		{
			Traits::move_assign(this->m_absolute_begin_pointer, source_begin_p, source_size_p);
		}
	}

	_FE_CONSTEXPR20_ void __restructrue_fstack_with_copy_semantics(value_type* const source_begin_p, FE::size source_size_p) noexcept
	{
		FE::size l_this_size = this->size();

		if (source_size_p > l_this_size)
		{
			FE::size l_count_to_construct = source_size_p - l_this_size;
			Traits::copy_construct(this->m_top_ptr, source_begin_p - l_count_to_construct, l_count_to_construct);

			Traits::copy_assign(this->m_absolute_begin_pointer, source_begin_p, l_this_size);
		}
		else if (source_size_p < l_this_size)
		{
			FE::size l_count_to_destruct = l_this_size - source_size_p;

			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				Traits::destruct(this->m_top_ptr - l_count_to_destruct, this->m_top_ptr);
			}

			Traits::copy_assign(this->m_absolute_begin_pointer, source_begin_p, source_size_p);
		}
		else
		{
			Traits::copy_assign(this->m_absolute_begin_pointer, source_begin_p, source_size_p);
		}
	}
};

END_NAMESPACE
#endif
