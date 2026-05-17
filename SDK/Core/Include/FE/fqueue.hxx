#ifndef _FE_CORE_FQUEUE_HXX_
#define _FE_CORE_FQUEUE_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/private/memory_traits.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/iterator.hxx>

// std
#include <initializer_list>
#include <utility>

#pragma warning(push)




BEGIN_NAMESPACE(FE)



/*
fqueue is a fixed-capacity, circular queue implementation that provides efficient FIFO operations.
It uses a contiguous memory block to store elements and manages front and back pointers to simulate a queue.
Supports both trivial and non-trivial types through the Traits template parameter.
Capacity is fixed at compile-time, and operations are noexcept where possible.
*/
template<class T, size Capacity, class Traits = internal::memory_traits<T>>
class fqueue final
{
	static_assert((std::is_same<T, typename Traits::value_type>::value), "Static Assertion Failed: The template argument T and Traits' value_type have be the same type.");
	static_assert(std::is_class<Traits>::value, "Static Assertion Failed: The template argument Traits is not a class type.");

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
	pointer const m_begin_ptr;
	size_type m_size;

public:
	fqueue() noexcept 
		:	m_memory(), 
			m_front_ptr(reinterpret_cast<pointer>(m_memory)), 
			m_back_ptr(m_front_ptr), 
			m_begin_ptr(m_front_ptr),
			m_size() 
	{}
	~fqueue() noexcept { pop_all(); }

	fqueue(std::initializer_list<value_type>&& initializer_list_p) noexcept 
		:	m_memory(), 
			m_front_ptr(reinterpret_cast<pointer>(m_memory)), 
			m_back_ptr(m_front_ptr + initializer_list_p.size()), 
			m_begin_ptr(m_front_ptr), 
			m_size(initializer_list_p.size())
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		Traits::move_construct(m_front_ptr, const_cast<value_type*>(initializer_list_p.begin()), initializer_list_p.size());
	}

	template<class InputIterator>
	fqueue(InputIterator begin_p, InputIterator end_p) noexcept 
		:	m_memory(), 
			m_front_ptr(reinterpret_cast<pointer>(m_memory)),
			m_back_ptr(m_front_ptr + (end_p - begin_p)), 
			m_begin_ptr(m_front_ptr), 
			m_size(end_p - begin_p)
	{
		static_assert(std::is_class<InputIterator>::value, "Static Assertion Failure: The template argument InputIterator must be a class type.");
		static_assert((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value), "Static Assertion Failure: InputIterator's value_type has to be the same as fqueue's value_type.");

		FE_NEGATIVE_ASSERT(begin_p >= end_p, "${%s@0}: The input iterator ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIterator), TO_STRING(begin_p), TO_STRING(end_p));
		FE_NEGATIVE_ASSERT(static_cast<uint64>(end_p - begin_p) > Capacity, "${%s@0}: The input size exceeds the fqueue capacity.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow));

		Traits::copy_construct(InputIterator{ m_begin_ptr }, begin_p, end_p - begin_p);
	}

	fqueue(const fqueue& other_p) noexcept
		:	m_memory(), 
			m_front_ptr(reinterpret_cast<pointer>(m_memory)), 
			m_back_ptr(m_front_ptr),
			m_begin_ptr(m_front_ptr), 
			m_size(other_p.m_size)
	{
		if (other_p.is_empty())
		{
			return;
		}

		Traits::copy_construct(m_front_ptr, other_p.m_front_ptr, other_p.size());

		__jump_front_pointer(other_p.m_front_ptr - other_p.m_begin_ptr);
		__jump_back_pointer(other_p.m_back_ptr - other_p.m_front_ptr);
	}

	constexpr fqueue(fqueue&& rvalue_p) noexcept
		:	m_memory(),
			m_front_ptr(reinterpret_cast<pointer>(m_memory)), 
			m_back_ptr(m_front_ptr), 
			m_begin_ptr(m_front_ptr), 
			m_size(rvalue_p.m_size)
	{
		if (rvalue_p.is_empty())
		{
			return;
		}

		Traits::move_construct(m_front_ptr, rvalue_p.m_front_ptr, rvalue_p.size());

		__jump_front_pointer(rvalue_p.m_front_ptr - rvalue_p.m_begin_ptr);
		__jump_back_pointer(rvalue_p.m_back_ptr - rvalue_p.m_front_ptr);
		rvalue_p.__set_front_pointer_to_zero();
		rvalue_p.__set_back_pointer_to_zero();
	}

	fqueue& operator=(std::initializer_list<value_type>&& initializer_list_p) noexcept
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		this->~fqueue();
		new(this) fqueue(std::move(initializer_list_p));
		return *this;
	}

	constexpr fqueue& operator=(const fqueue& other_p) noexcept
	{
		if (other_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(other_p);
		return *this;
	}

	constexpr fqueue& operator=(fqueue&& rvalue_p) noexcept
	{
		if (rvalue_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(std::move(rvalue_p));
		return *this;
	}

	template <typename... Arguments>
	constexpr void push(Arguments&&... value_p) noexcept
	{
		if (m_back_ptr >= m_begin_ptr + Capacity)
		{
			__set_back_pointer_to_zero();
			FE_NEGATIVE_ASSERT(m_back_ptr >= m_front_ptr, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation));
		}

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			new(m_back_ptr) T( std::forward<Arguments&&>(value_p)... );
		}
		else if constexpr (Traits::is_trivial == TypeTriviality::_Trivial)
		{
			*m_back_ptr = T(std::forward<Arguments&&>(value_p)...);
		}
		
		++m_back_ptr;
		++m_size;
	}

	constexpr value_type pop() noexcept
	{
		if ((m_begin_ptr + Capacity) == m_front_ptr)
		{
			m_front_ptr = m_begin_ptr;
			FE_NEGATIVE_ASSERT(is_empty() == true, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation));
		}

		T l_return_value_buffer = std::move(*m_front_ptr);

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			m_front_ptr->~T();
		}

		++m_front_ptr;
		--m_size;
		return l_return_value_buffer;
	}

	constexpr void pop_all() noexcept
	{
		if (is_empty() == false)
		{
			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				if (m_back_ptr >= m_front_ptr)
				{
					Traits::destruct(m_front_ptr, m_back_ptr);
				}
				else
				{
					Traits::destruct(m_begin_ptr, m_back_ptr);
					Traits::destruct(m_front_ptr, m_begin_ptr + Capacity);
				}
			}

			__set_front_pointer_to_zero();
			__set_back_pointer_to_zero();
			m_size = 0;
		}
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reference front() const noexcept
	{
		return *m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reference back() const noexcept
	{
		return *(m_back_ptr - 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr reference front() noexcept
	{
		return *m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr reference back() noexcept
	{
		return *(m_back_ptr - 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr boolean is_empty() const noexcept
	{
		return m_size == 0;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type count() const noexcept
	{
		return m_size;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type size() const noexcept
	{
		return m_size;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type max_size() const noexcept
	{
		return Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type capacity() const noexcept
	{
		return Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_iterator cbegin() const noexcept
	{
		return m_front_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_iterator cend() const noexcept
	{
		return m_front_ptr + Capacity;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reverse_iterator crbegin() const noexcept
	{
		return (m_front_ptr + Capacity) - 1;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reverse_iterator crend() const noexcept
	{
		return m_front_ptr - 1;
	}

	_FE_FORCE_INLINE_ constexpr void swap(fqueue& in_out_other_p) noexcept
	{
		std::swap(*this, in_out_other_p);
	}

	_FE_NODISCARD_ constexpr boolean operator==(fqueue& other_p) const noexcept
	{
		return FE::memcmp(cbegin(), cend(), other_p.cbegin(), other_p.cend());
	}

	_FE_NODISCARD_ constexpr boolean operator!=(fqueue& other_p) const noexcept
	{
		return !FE::memcmp(cbegin(), cend(), other_p.cbegin(), other_p.cend());
	}

protected:
	_FE_FORCE_INLINE_ constexpr void __jump_front_pointer(difference_type ptrdiff_p) noexcept
	{
		m_front_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ constexpr void __set_front_pointer_to_zero() noexcept
	{
		m_front_ptr = m_begin_ptr;
	}

	_FE_FORCE_INLINE_ constexpr void __jump_back_pointer(difference_type ptrdiff_p) noexcept
	{
		m_back_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ constexpr void __set_back_pointer_to_zero() noexcept
	{
		m_back_ptr = m_begin_ptr;
	}
};


END_NAMESPACE
#pragma warning(pop)
#endif