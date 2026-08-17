#ifndef _FE_CORE_FSTACK_HXX_
#define _FE_CORE_FSTACK_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
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




BEGIN_NAMESPACE(FE)




/*
The FE::fstack class template is a fixed-capacity stack implementation that utilizes custom memory traits for managing its elements
providing various constructors and assignment operators for initialization and manipulation of the stack's contents.

m_top_ptr always addresses the top element itself, hence dereferencing it yields the top value rather than the past-the-end slot.
m_begin_ptr is the first element slot of m_memory, and an empty fstack is denoted by m_top_ptr == m_begin_ptr - 1, which is never dereferenced.
*/
template<class T, size Capacity, class Traits = FE::internal::memory_traits<T>>
class fstack final
{
	static_assert((std::is_same<T, typename Traits::value_type>::value), "Static Assertion Failed: The template argument T and Traits' value_type have be the same type.");
	static_assert(std::is_class<Traits>::value, "Static Assertion Failed: The template argument Traits is not a class type.");
	static_assert(Capacity > 0);

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
	pointer const m_begin_ptr;

public:
	fstack() noexcept
		: m_memory(),
		m_top_ptr(reinterpret_cast<pointer>(m_memory) - 1),
		m_begin_ptr(reinterpret_cast<pointer>(m_memory))
	{
	}
	~fstack() noexcept { pop_all(); }

	fstack(std::initializer_list<value_type>&& initializer_list_p) noexcept
		: m_memory(),
		m_top_ptr((reinterpret_cast<pointer>(m_memory) + initializer_list_p.size()) - 1),
		m_begin_ptr(reinterpret_cast<pointer>(m_memory))
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "ERROR!: The length of std::initializer_list exceeds the Capacity");
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		Traits::move_construct(m_begin_ptr, const_cast<value_type*>(initializer_list_p.begin()), initializer_list_p.size());
	}

	template<class InputIterator>
	fstack(InputIterator begin_p, InputIterator end_p) noexcept
		: m_memory(),
		m_top_ptr((reinterpret_cast<pointer>(m_memory) + (end_p - begin_p)) - 1),
		m_begin_ptr(reinterpret_cast<pointer>(m_memory))
	{
		static_assert(std::is_class<InputIterator>::value, "Static Assertion Failure: The template argument InputIterator must be a class type.");
		static_assert((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value), "Static Assertion Failure: InputIterator's value_type has to be the same as fstack's value_type.");

		FE_NEGATIVE_ASSERT(begin_p >= end_p, "${%s@0}: The input iterator ${%s@1} must not be greater than the iterator ${%s@2}.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIterator), TO_STRING(begin_p), TO_STRING(end_p));
		FE_NEGATIVE_ASSERT(static_cast<uint64>(end_p - begin_p) > Capacity, "${%s@0}: The input size exceeds the fstack capacity.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow));

		Traits::copy_construct(InputIterator{ m_begin_ptr }, begin_p, end_p - begin_p);
	}

	fstack(const fstack& other_p) noexcept
		: m_memory(),
		m_top_ptr(reinterpret_cast<pointer>(m_memory) - 1),
		m_begin_ptr(reinterpret_cast<pointer>(m_memory))
	{
		if (other_p.is_empty())
		{
			return;
		}

		Traits::copy_construct(m_begin_ptr, capacity(), other_p.m_begin_ptr, other_p.size());

		__jump_top_pointer(other_p.size());
	}

	fstack(fstack&& rvalue_p) noexcept
		: m_memory(),
		m_top_ptr(reinterpret_cast<pointer>(m_memory) - 1),
		m_begin_ptr(reinterpret_cast<pointer>(m_memory))
	{
		if (rvalue_p.is_empty())
		{
			return;
		}

		Traits::move_construct(m_begin_ptr, capacity(), rvalue_p.m_begin_ptr, rvalue_p.size());

		__jump_top_pointer(rvalue_p.size());
		rvalue_p.pop_all();
	}

	fstack& operator=(std::initializer_list<value_type> initializer_list_p) noexcept
	{
		FE_NEGATIVE_ASSERT(initializer_list_p.size() > Capacity, "ERROR!: The length of std::initializer_list exceeds the Capacity");
		FE_NEGATIVE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize));

		FE::size l_initializer_list_size = initializer_list_p.size();
		if (l_initializer_list_size == 0)
		{
			pop_all();
			return *this;
		}

		__restructure_fstack_with_move_semantics(const_cast<value_type*>(initializer_list_p.begin()), l_initializer_list_size);

		__set_top_pointer_to_zero();
		__jump_top_pointer(l_initializer_list_size);
		return *this;
	}

	fstack& operator=(const fstack& other_p) noexcept
	{
		if (this == &other_p)
		{
			return *this;
		}

		FE::size l_other_size = other_p.size();
		if (l_other_size == 0)
		{
			pop_all();
			return *this;
		}

		__restructure_fstack_with_copy_semantics(other_p.m_begin_ptr, l_other_size);

		__set_top_pointer_to_zero();
		__jump_top_pointer(l_other_size);
		return *this;
	}

	fstack& operator=(fstack&& rvalue_p) noexcept
	{
		if (this == &rvalue_p)
		{
			return *this;
		}

		FE::size l_other_size = rvalue_p.size();
		if (l_other_size == 0)
		{
			pop_all();
			return *this;
		}

		__restructure_fstack_with_move_semantics(rvalue_p.m_begin_ptr, l_other_size);

		__set_top_pointer_to_zero();
		__jump_top_pointer(l_other_size);
		rvalue_p.pop_all();
		return *this;
	}

	void push(const value_type& value_p) noexcept
	{
		FE_NEGATIVE_ASSERT(m_top_ptr >= (m_begin_ptr + Capacity) - 1, "${%s@0}: The fstack top exceeded the index boundary", TO_STRING(ErrorCode::_FatalMemoryError_1XX_AccessViolation));

		++m_top_ptr;

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			new(m_top_ptr) T(value_p);
		}
		else if constexpr (Traits::is_trivial == TypeTriviality::_Trivial)
		{
			std::memcpy(m_top_ptr, &value_p, sizeof(T));
		}
	}

	template<typename... Arguments>
	void emplace(Arguments&&... value_p) noexcept
	{
		FE_NEGATIVE_ASSERT(m_top_ptr >= (m_begin_ptr + Capacity) - 1, "${%s@0}: The fstack top exceeded the index boundary", TO_STRING(ErrorCode::_FatalMemoryError_1XX_AccessViolation));

		++m_top_ptr;

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			new(m_top_ptr) T(std::forward<Arguments&&>(value_p)...);
		}
		else if constexpr (Traits::is_trivial == TypeTriviality::_Trivial)
		{
			*m_top_ptr = T(std::forward<Arguments&&>(value_p)...);
		}
	}

	value_type pop() noexcept
	{
		FE_NEGATIVE_ASSERT(is_empty() == true, "${%s@0}: The fstack top index reached zero. The index value_p must be greater than zero", TO_STRING(ErrorCode::_FatalMemoryError_1XX_AccessViolation));

		T l_return_value_buffer = std::move(*m_top_ptr);

		if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
		{
			m_top_ptr->~T();
		}

		--m_top_ptr;
		return l_return_value_buffer;
	}

	void pop_all() noexcept
	{
		if (is_empty() == false)
		{
			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				Traits::destruct(m_begin_ptr, m_top_ptr + 1);
			}
			__set_top_pointer_to_zero();
		}
	}

	_FE_FORCE_INLINE_ constexpr const_reference top() const noexcept
	{
		return *m_top_ptr;
	}

	_FE_FORCE_INLINE_ constexpr reference top() noexcept
	{
		return *m_top_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr var::boolean is_empty() const noexcept
	{
		return (m_top_ptr < m_begin_ptr) ? true : false;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type count() const noexcept
	{
		return static_cast<size_type>((m_top_ptr - m_begin_ptr) + 1);
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr size_type size() const noexcept
	{
		return static_cast<size_type>((m_top_ptr - m_begin_ptr) + 1);
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
		return m_begin_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_iterator cend() const noexcept
	{
		return m_top_ptr + 1;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reverse_iterator crbegin() const noexcept
	{
		return m_top_ptr;
	}

	_FE_NODISCARD_ _FE_FORCE_INLINE_ constexpr const_reverse_iterator crend() const noexcept
	{
		return m_begin_ptr - 1;
	}

	_FE_FORCE_INLINE_ constexpr void swap(fstack& in_out_other_p) noexcept
	{
		std::swap(*this, in_out_other_p);
	}

	_FE_NODISCARD_ constexpr boolean operator==(const fstack& other_p) const noexcept
	{
		return FE::memcmp(cbegin(), cend(), other_p.cbegin(), other_p.cend());
	}

	_FE_NODISCARD_ constexpr boolean operator!=(const fstack& other_p) const noexcept
	{
		return !FE::memcmp(cbegin(), cend(), other_p.cbegin(), other_p.cend());
	}

private:
	_FE_FORCE_INLINE_ constexpr void __jump_top_pointer(difference_type ptrdiff_p) noexcept
	{
		m_top_ptr += ptrdiff_p;
	}

	_FE_FORCE_INLINE_ constexpr void __set_top_pointer_to_zero() noexcept
	{
		m_top_ptr = m_begin_ptr - 1;
	}

	void __restructure_fstack_with_move_semantics(value_type* const source_begin_p, FE::size source_size_p) noexcept
	{
		FE::size l_this_size = size();

		if ((source_size_p > l_this_size) && (l_this_size != 0))
		{
			FE::size l_count_to_construct = source_size_p - l_this_size;

			Traits::move_assign(m_begin_ptr, source_begin_p, l_this_size);
			Traits::move_construct(m_top_ptr + 1, source_begin_p + l_this_size, l_count_to_construct);
		}
		else if ((source_size_p < l_this_size) && (l_this_size != 0))
		{
			FE::size l_count_to_destruct = l_this_size - source_size_p;

			Traits::move_assign(m_begin_ptr, source_begin_p, source_size_p);

			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				Traits::destruct((m_top_ptr + 1) - l_count_to_destruct, m_top_ptr + 1);
			}
		}
		else
		{
			Traits::move_assign(m_begin_ptr, source_begin_p, source_size_p);
		}
	}

	void __restructure_fstack_with_copy_semantics(value_type* const source_begin_p, FE::size source_size_p) noexcept
	{
		FE::size l_this_size = size();

		if ((source_size_p > l_this_size) && (l_this_size != 0))
		{
			FE::size l_count_to_construct = source_size_p - l_this_size;

			Traits::copy_assign(m_begin_ptr, source_begin_p, l_this_size);
			Traits::copy_construct(m_top_ptr + 1, source_begin_p + l_this_size, l_count_to_construct);
		}
		else if ((source_size_p < l_this_size) && (l_this_size != 0))
		{
			FE::size l_count_to_destruct = l_this_size - source_size_p;

			Traits::copy_assign(m_begin_ptr, source_begin_p, source_size_p);

			if constexpr (Traits::is_trivial == TypeTriviality::_NotTrivial)
			{
				Traits::destruct((m_top_ptr + 1) - l_count_to_destruct, m_top_ptr + 1);
			}
		}
		else
		{
			Traits::copy_assign(m_begin_ptr, source_begin_p, source_size_p);
		}
	}
};

namespace xtl
{
	template<class T, FE::size Capacity, class Traits>
	using fstack = FE::fstack<T, Capacity, Traits>;
}

END_NAMESPACE
#endif