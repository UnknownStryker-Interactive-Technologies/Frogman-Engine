#ifndef _FE_CORE_FQUEUE_HXX_
#define _FE_CORE_FQUEUE_HXX_
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

#pragma warning(push)




BEGIN_NAMESPACE(FE)




template<class T, size Capacity, class Traits = internal::memory_traits<T>>
class fqueue final
{
	FE_NEGATIVE_STATIC_ASSERT((std::is_same<T, typename Traits::value_type>::value == false), "Static Assertion Failed: The template argument T and Traits' value_type have be the same type.");
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Traits>::value == false, "Static Assertion Failed: The template argument Traits is not a class type.");

public:
	using value_type = T;
	using size_type = var::uint64;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using const_iterator = FE::const_iterator<FE::contiguous_iterator<T>>;
	using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<T>>;
	using difference_type = var::ptrdiff;

protected:
	var::byte m_memory[sizeof(value_type) * Capacity];
	pointer m_front_ptr;
	pointer m_back_ptr;
	pointer const m_absolute_begin_pointer;
	size_type m_indirected_element_count;

public:
	_FE_FORCE_INLINE_ fqueue() noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count() {}
	_FE_FORCE_INLINE_ ~fqueue() noexcept { this->pop_all(); }

	_FE_CONSTEXPR20_ fqueue(std::initializer_list<value_type>&& initializer_list_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr + initializer_list_p.size()), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(initializer_list_p.size())
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		Traits::move_construct(this->m_front_ptr, const_cast<value_type*>(initializer_list_p.begin()), initializer_list_p.size());
	}

	template<class InputIterator>
	_FE_CONSTEXPR20_ fqueue(InputIterator first_p, InputIterator last_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr + (last_p - first_p)), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(last_p - first_p)
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<InputIterator>::value == false, "Static Assertion Failure: The template argument InputIterator must be a class type.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value == false), "Static Assertion Failure: InputIterator's value_type has to be the same as fqueue's value_type.");

		FE_NEGATIVE_ASSERT(first_p >= last_p, "${%s@0}: The input iterator ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIterator), TO_STRING(first_p), TO_STRING(last_p));
		FE_NEGATIVE_ASSERT(static_cast<uint64>(last_p - first_p) > Capacity, "${%s@0}: The input size exceeds the fqueue capacity.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow));

		Traits::copy_construct(InputIterator{ this->m_absolute_begin_pointer }, first_p, last_p - first_p);
	}

	_FE_CONSTEXPR20_ fqueue(fqueue& other_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(other_p.m_indirected_element_count)
	{
		if (other_p.is_empty())
		{
			return;
		}

		Traits::copy_construct(this->m_front_ptr, other_p.m_front_ptr, other_p.size());

		this->__jump_front_pointer(other_p.m_front_ptr - other_p.m_absolute_begin_pointer);
		this->__jump_back_pointer(other_p.m_back_ptr - other_p.m_front_ptr);
	}

	_FE_CONSTEXPR20_ fqueue(fqueue&& rvalue_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(rvalue_p.m_indirected_element_count)
	{
		if (rvalue_p.is_empty())
		{
			return;
		}

		Traits::move_construct(this->m_front_ptr, rvalue_p.m_front_ptr, rvalue_p.size());

		this->__jump_front_pointer(rvalue_p.m_front_ptr - rvalue_p.m_absolute_begin_pointer);
		this->__jump_back_pointer(rvalue_p.m_back_ptr - rvalue_p.m_front_ptr);
		rvalue_p.__set_front_pointer_to_zero();
		rvalue_p.__set_back_pointer_to_zero();
	}

	_FE_CONSTEXPR20_ fqueue& operator=(std::initializer_list<value_type>&& initializer_list_p) noexcept
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		this->~fqueue();
		new(this) fqueue(std::move(initializer_list_p));
		return *this;
	}

	_FE_CONSTEXPR20_ fqueue& operator=(fqueue& other_p) noexcept
	{
		if (other_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(other_p);
		return *this;
	}

	_FE_CONSTEXPR20_ fqueue& operator=(fqueue&& rvalue_p) noexcept
	{
		if (rvalue_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(std::move(rvalue_p));
		return *this;
	}

	_FE_CONSTEXPR20_ void push(const value_type& value_p) noexcept
	{
		if (this->m_back_ptr >= this->m_absolute_begin_pointer + Capacity)
		{
			this->__set_back_pointer_to_zero();
			FE_NEGATIVE_ASSERT(this->m_back_ptr >= this->m_front_ptr, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation));
		}

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			new(this->m_back_ptr) T(value_p);
		}
		else if constexpr (Traits::is_trivial == TypeTriviality::_Trivial)
		{
			*this->m_back_ptr = value_p;
		}
		
		++this->m_back_ptr;
		++this->m_indirected_element_count;
	}

	_FE_CONSTEXPR20_ value_type pop() noexcept
	{
		if ((this->m_absolute_begin_pointer + Capacity) == this->m_front_ptr)
		{
			this->m_front_ptr = this->m_absolute_begin_pointer;
			FE_NEGATIVE_ASSERT(this->is_empty() == true, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation));
		}

		T l_return_value_buffer = std::move(*this->m_front_ptr);

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			this->m_front_ptr->~T();
		}

		++this->m_front_ptr;
		--this->m_indirected_element_count;
		return l_return_value_buffer;
	}

	_FE_CONSTEXPR20_ void pop_all() noexcept
	{
		if (this->is_empty() == false)
		{
			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				if (this->m_back_ptr >= this->m_front_ptr)
				{
					Traits::destruct(this->m_front_ptr, this->m_back_ptr);
				}
				else
				{
					Traits::destruct(this->m_absolute_begin_pointer, this->m_back_ptr);
					Traits::destruct(this->m_front_ptr, this->m_absolute_begin_pointer + Capacity);
				}
			}

			this->__set_front_pointer_to_zero();
			this->__set_back_pointer_to_zero();
			this->m_indirected_element_count = 0;
		}
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reference front() const noexcept
	{
		return *this->m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reference back() const noexcept
	{
		return *(this->m_back_ptr - 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ reference front() noexcept
	{
		return *this->m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ reference back() noexcept
	{
		return *(this->m_back_ptr - 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ boolean is_empty() const noexcept
	{
		return this->m_indirected_element_count == 0;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type count() const noexcept
	{
		return this->m_indirected_element_count;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ size_type size() const noexcept
	{
		return this->m_indirected_element_count;
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
		return this->m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_iterator cend() const noexcept
	{
		return this->m_front_ptr + Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		return (this->m_front_ptr + Capacity) - 1;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		return this->m_front_ptr - 1;
	}

	_FE_FORCE_INLINE_ void swap(fqueue& in_out_other_p) noexcept
	{
		std::swap(*this, in_out_other_p);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ boolean operator==(fqueue& other_p) const noexcept
	{
		return FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ boolean operator!=(fqueue& other_p) const noexcept
	{
		return !FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

protected:
	_FE_FORCE_INLINE_ void __jump_front_pointer(difference_type ptrdiff_p) noexcept
	{
		this->m_front_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ void __set_front_pointer_to_zero() noexcept
	{
		this->m_front_ptr = this->m_absolute_begin_pointer;
	}

	_FE_FORCE_INLINE_ void __jump_back_pointer(difference_type ptrdiff_p) noexcept
	{
		this->m_back_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ void __set_back_pointer_to_zero() noexcept
	{
		this->m_back_ptr = this->m_absolute_begin_pointer;
	}
};


END_NAMESPACE
#pragma warning(pop)
#endif