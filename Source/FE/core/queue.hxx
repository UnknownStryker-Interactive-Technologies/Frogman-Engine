#ifndef _FE_CORE_QUEUE_HXX_
#define _FE_CORE_QUEUE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/memory_traits.hxx>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/iterator.hxx>

// std
#include <initializer_list>

#pragma warning(push)




BEGIN_NAMESPACE(FE)




template<class T, size Capacity, class Traits = FE::internal::memory_traits<T>>
class fqueue final
{
	FE_STATIC_ASSERT((std::is_same<T, typename Traits::value_type>::value == false), "Static Assertion Failed: The template argument T and Traits' value_type have be the same type.");
	FE_STATIC_ASSERT(std::is_class<Traits>::value == false, "Static Assertion Failed: The template argument Traits is not a class type.");

public:
	using value_type = T;
	using length_type = var::count_t;
	using size_type = var::count_t;
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
	_FORCE_INLINE_ fqueue() noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count() {}
	_FORCE_INLINE_ ~fqueue() noexcept { this->pop_all(); }

	_CONSTEXPR20_ fqueue(std::initializer_list<value_type>&& initializer_list_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr + initializer_list_p.size()), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(initializer_list_p.size())
	{
		FE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
		FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));

		Traits::move_construct(this->m_front_ptr, const_cast<value_type*>(initializer_list_p.begin()), initializer_list_p.size());
	}

	template<class InputIterator>
	_CONSTEXPR20_ fqueue(InputIterator first_p, InputIterator last_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr + (last_p - first_p)), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(last_p - first_p)
	{
		FE_STATIC_ASSERT(std::is_class<InputIterator>::value == false, "Static Assertion Failure: The template argument InputIterator must be a class type.");
		FE_STATIC_ASSERT((std::is_same<typename std::remove_const<typename InputIterator::value_type>::type, typename std::remove_const<value_type>::type>::value == false), "Static Assertion Failure: InputIterator's value_type has to be the same as fqueue's value_type.");

		FE_ASSERT(first_p >= last_p, "${%s@0}: The input iterator ${%s@1} must not be greater than ${%s@2}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(first_p), TO_STRING(last_p));
		FE_ASSERT(static_cast<uint64>(last_p - first_p) > Capacity, "${%s@0}: The input size exceeds the fqueue capacity.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY));

		Traits::copy_construct(InputIterator{ this->m_absolute_begin_pointer }, first_p, last_p - first_p);
	}

	_CONSTEXPR20_ fqueue(fqueue& other_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(other_p.m_indirected_element_count)
	{
		if (other_p.is_empty())
		{
			return;
		}

		Traits::copy_construct(this->m_front_ptr, other_p.m_front_ptr, other_p.size());

		this->__jump_front_pointer(other_p.m_front_ptr - other_p.m_absolute_begin_pointer);
		this->__jump_back_pointer(other_p.m_back_ptr - other_p.m_front_ptr);
	}

	_CONSTEXPR20_ fqueue(fqueue&& rvalue_p) noexcept : m_memory(), m_front_ptr(reinterpret_cast<pointer>(m_memory)), m_back_ptr(m_front_ptr), m_absolute_begin_pointer(m_front_ptr), m_indirected_element_count(rvalue_p.m_indirected_element_count)
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

	_CONSTEXPR20_ fqueue& operator=(std::initializer_list<value_type>&& initializer_list_p) noexcept
	{
		FE_ASSERT(initializer_list_p.size() > Capacity, "${%s@0}!: The length of std::initializer_list exceeds the Capacity", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));
		FE_ASSERT(initializer_list_p.size() == 0, "${%s@0}!: Cannot assign an empty initializer_list", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE));

		this->~fqueue();
		new(this) fqueue(std::move(initializer_list_p));
		return *this;
	}

	_CONSTEXPR20_ fqueue& operator=(fqueue& other_p) noexcept
	{
		if (other_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(other_p);
		return *this;
	}

	_CONSTEXPR20_ fqueue& operator=(fqueue&& rvalue_p) noexcept
	{
		if (rvalue_p.is_empty())
		{
			return *this;
		}

		this->~fqueue();
		new(this) fqueue(std::move(rvalue_p));
		return *this;
	}

	_CONSTEXPR20_ void push(const value_type& value_p) noexcept
	{
		if (this->m_back_ptr >= this->m_absolute_begin_pointer + Capacity)
		{
			this->__set_back_pointer_to_zero();
			FE_ASSERT(this->m_back_ptr >= this->m_front_ptr, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE));
		}

		Traits::construct(*this->m_back_ptr, value_p);
		++this->m_back_ptr;
		++this->m_indirected_element_count;
	}

	_CONSTEXPR20_ value_type pop() noexcept
	{
		if ((this->m_absolute_begin_pointer + Capacity) == this->m_front_ptr)
		{
			this->m_front_ptr = this->m_absolute_begin_pointer;
			FE_ASSERT(this->is_empty() == true, "${%s@0}: Exceeded the queue index boundary", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_RANGE));
		}

		T l_return_value_buffer = std::move(*this->m_front_ptr);

		if constexpr (Traits::is_trivial == TYPE_TRIVIALITY::_NOT_TRIVIAL)
		{
			Traits::destruct(*this->m_front_ptr);
		}

		++this->m_front_ptr;
		--this->m_indirected_element_count;
		return l_return_value_buffer;
	}

	_CONSTEXPR20_ void pop_all() noexcept
	{
		if (this->is_empty() == false)
		{
			if constexpr (Traits::is_trivial == TYPE_TRIVIALITY::_NOT_TRIVIAL)
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

	_NODISCARD_ _FORCE_INLINE_ const_reference front() const noexcept
	{
		return *this->m_front_ptr;
	}

	_NODISCARD_ _FORCE_INLINE_ const_reference back() const noexcept
	{
		return *(this->m_back_ptr - 1);
	}

	_NODISCARD_ _FORCE_INLINE_ reference front() noexcept
	{
		return *this->m_front_ptr;
	}

	_NODISCARD_ _FORCE_INLINE_ reference back() noexcept
	{
		return *(this->m_back_ptr - 1);
	}

	_NODISCARD_ _FORCE_INLINE_ boolean is_empty() const noexcept
	{
		return this->m_indirected_element_count == 0;
	}

	_NODISCARD_ _FORCE_INLINE_ size_type count() const noexcept
	{
		return this->m_indirected_element_count;
	}

	_NODISCARD_ _FORCE_INLINE_ size_type size() const noexcept
	{
		return this->m_indirected_element_count;
	}

	_NODISCARD_ _FORCE_INLINE_ size_type max_size() const noexcept
	{
		return Capacity;
	}

	_NODISCARD_ _FORCE_INLINE_ size_type capacity() const noexcept
	{
		return Capacity;
	}

	_NODISCARD_ _FORCE_INLINE_ const_iterator cbegin() const noexcept
	{
		return this->m_front_ptr;
	}

	_NODISCARD_ _FORCE_INLINE_ const_iterator cend() const noexcept
	{
		return this->m_front_ptr + Capacity;
	}

	_NODISCARD_ _FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		return (this->m_front_ptr + Capacity) - 1;
	}

	_NODISCARD_ _FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		return this->m_front_ptr - 1;
	}

	_FORCE_INLINE_ void swap(fqueue& in_out_other_p) noexcept
	{
		algorithm::utility::swap(*this, in_out_other_p);
	}

	_NODISCARD_ _FORCE_INLINE_ boolean operator==(fqueue& other_p) const noexcept
	{
		return FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

	_NODISCARD_ _FORCE_INLINE_ boolean operator!=(fqueue& other_p) const noexcept
	{
		return !FE::memcmp(this->cbegin(), this->cend(), other_p.cbegin(), other_p.cend());
	}

protected:
	_FORCE_INLINE_ void __jump_front_pointer(difference_type ptrdiff_p) noexcept
	{
		this->m_front_ptr += ptrdiff_p;
	}

	_FORCE_INLINE_ void __set_front_pointer_to_zero() noexcept
	{
		this->m_front_ptr = this->m_absolute_begin_pointer;
	}

	_FORCE_INLINE_ void __jump_back_pointer(difference_type ptrdiff_p) noexcept
	{
		this->m_back_ptr += ptrdiff_p;
	}

	_FORCE_INLINE_ void __set_back_pointer_to_zero() noexcept
	{
		this->m_back_ptr = this->m_absolute_begin_pointer;
	}
};


END_NAMESPACE
#pragma warning(pop)
#endif