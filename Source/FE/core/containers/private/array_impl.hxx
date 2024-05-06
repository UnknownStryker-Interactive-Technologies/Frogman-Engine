#ifndef _FE_CORE_CONTAINERS_PRIVATE_ARRAY_IMPL_HXX_
#define _FE_CORE_CONTAINERS_PRIVATE_ARRAY_IMPL_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/allocator.hxx>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/iterator.hxx>
#include <FE/core/memory.hxx>
#include <FE/core/smart_ptrs.h>
#include <initializer_list>




BEGIN_NAMESPACE(FE::internal)


template<class T, class Allocator, FE::TYPE_TRIVIALITY IsTrivial> 
class array_impl;

template<class T, class Allocator>
class array_impl<T, Allocator, FE::TYPE_TRIVIALITY::_NOT_TRIVIAL> 
{
	FE_STATIC_ASSERT((std::is_same<T, Allocator::value_type>::value == false), "static assertion failed: enforcing Allocator's value_type to be equivalent to the typename T. The template parameter T must be identical to the value_type of the Allocator.");

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = typename Allocator::size_type;
	using difference_type = typename Allocator::difference_type;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = FE::iterator<FE::contiguous_iterator<T>>;
	using const_iterator = FE::const_iterator<FE::contiguous_iterator<T>>;
	using reverse_iterator = FE::reverse_iterator<FE::contiguous_iterator<T>>;
	using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<T>>;

private:
	FE::exclusive_ptr<T[], Allocator> m_smart_array;
	size_type m_array_size;

public:
	_CONSTEXPR20_ array_impl(const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size() {}

	_CONSTEXPR20_ array_impl(const size_type count_p, const T& value_p = T(), const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size()
	{
		FE_ASSERT(count_p == 0, "Assertion Failure: ${%s@0} must not be zero", TO_STRING(count_p));

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(count_p);
		this->m_array_size = count_p;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + count_p;

		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(value_p);
			++l_ptr;
		}
	}

	_CONSTEXPR20_ array_impl(const array_impl& other_p) noexcept : m_smart_array(), m_array_size()
	{
		if (other_p.m_array_size == 0)
		{
			return;
		}

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(other_p.m_array_size);
		this->m_array_size = other_p.m_array_size;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + this->m_array_size;

		pointer l_input_ptr = other_p.m_smart_array.get();
		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(*l_input_ptr);
			++l_ptr;
			++l_input_ptr;
		}
	}

	_CONSTEXPR20_ array_impl(array_impl&& rvalue_p) noexcept : m_smart_array(), m_array_size()
	{
		if (rvalue_p.m_array_size == 0)
		{
			return;
		}

		this->m_smart_array = std::move(rvalue_p.m_smart_array);
		this->m_array_size = algorithm::utility::exchange<size_type>(rvalue_p.m_array_size, 0);
	}

	template <class InputIterator>
	_CONSTEXPR20_ array_impl(InputIterator first_p, InputIterator last_p, const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size()
	{
		FE_STATIC_ASSERT(((std::is_class<InputIterator>::value == false) && (std::is_pointer<InputIterator>::value == false)), "Static Assertion Failure: The template argument InputIterator must be a class or a struct");
		FE_ASSERT(first_p == nullptr, "Assertion Failure: first_p must not be nullptr.");
		FE_ASSERT(last_p == nullptr, "Assertion Failure: last_p must not be nullptr.");
		FE_ASSERT(first_p >= last_p, "Assertion Failure: first_p cannot be greater than or equal to last_p.");

		const size_type l_input_size = last_p - first_p;
		if (l_input_size == 0)
		{
			return;
		}

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(l_input_size);
		this->m_array_size = l_input_size;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + l_input_size;

		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(*first_p);
			++l_ptr;
			++first_p;
		}
	}

	_CONSTEXPR20_ array_impl(std::initializer_list<T>&& initializer_list_p, const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size(initializer_list_p.size())
	{
		if (this->m_array_size == 0)
		{
			return;
		}

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(this->m_array_size);
		
		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + this->m_array_size;
		pointer l_input_ptr = const_cast<pointer>(initializer_list_p.begin());

		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(std::move(*l_input_ptr));
			++l_ptr;
			++l_input_ptr;
		}
	}

	template<class Container>
	_CONSTEXPR20_ array_impl(container::range<Container>&& range_p, const Allocator& allocator_p = Allocator()) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		const size_type l_input_size = range_p._end - range_p._begin;

		if (l_input_size == 0)
		{
			return;
		}

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(l_input_size);
		this->m_array_size = l_input_size;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + this->m_array_size;
		pointer l_input_ptr = range_p._container.get().begin().operator->() + range_p._begin;

		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(*l_input_ptr);
			++l_ptr;
			++l_input_ptr;
		}
	}

	_CONSTEXPR20_ ~array_impl() noexcept = default;

	_CONSTEXPR20_ array_impl& operator=(const array_impl& other_p) noexcept
	{
		if (other_p.m_array_size == 0)
		{
			return *this;
		}

		pointer l_begin_ptr = this->m_smart_array.get();
		if (other_p.m_array_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ other_p.m_array_size });
			l_begin_ptr = this->m_smart_array.get();

			pointer l_ptr = l_begin_ptr + this->m_array_size;
			pointer const l_ptr_end = l_begin_ptr + other_p.m_array_size;
			pointer l_input_ptr = other_p.m_smart_array.get() + this->m_array_size;
			while (l_ptr != l_ptr_end)
			{
				new(l_ptr) T(*l_input_ptr);
				++l_ptr;
				++l_input_ptr;
			}
		}
		else
		{
			if (other_p.m_array_size > this->m_array_size)
			{
				pointer l_ptr = l_begin_ptr + this->m_array_size;
				pointer const l_ptr_end = l_begin_ptr + other_p.m_array_size;
				pointer l_input_ptr = other_p.m_smart_array.get() + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					new(l_ptr) T(*l_input_ptr);
					++l_ptr;
					++l_input_ptr;
				}
			}
			else
			{
				pointer l_ptr = l_begin_ptr + other_p.m_array_size;
				pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					l_ptr->~T();
					++l_ptr;
				}
				this->m_array_size = other_p.m_array_size;
			}
		}

		pointer l_ptr = l_begin_ptr;
		pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
		pointer l_input_ptr = other_p.m_smart_array.get();
		while (l_ptr != l_ptr_end)
		{
			*l_ptr = *l_input_ptr;
			++l_ptr;
			++l_input_ptr;
		}

		this->m_array_size = other_p.m_array_size;
	
		return *this;
	}

	_CONSTEXPR20_ array_impl& operator=(array_impl&& rvalue_p) noexcept
	{
		if (rvalue_p.m_array_size == 0)
		{
			return *this;
		}

		this->m_smart_array = std::move(rvalue_p.m_smart_array);
		this->m_array_size = algorithm::utility::exchange<size_type>(rvalue_p.m_array_size, 0);
		return *this;
	}

	_CONSTEXPR20_ void assign(const size_type count_p, const T& value_p) noexcept
	{
		FE_ASSERT(count_p == 0, "Assertion Failure: ${%s@0} must not be zero.", TO_STRING(count_p));

		pointer l_begin_ptr = this->m_smart_array.get();
		if (count_p > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ count_p });
			l_begin_ptr = this->m_smart_array.get();

			pointer l_ptr = l_begin_ptr + this->m_array_size;
			pointer const l_ptr_end = l_begin_ptr + count_p;
			while (l_ptr != l_ptr_end)
			{
				new(l_ptr) T(value_p);
				++l_ptr;
			}
		}
		else
		{
			if (count_p > this->m_array_size)
			{
				pointer l_ptr = l_begin_ptr + this->m_array_size;
				pointer const l_ptr_end = l_begin_ptr + count_p;
				while (l_ptr != l_ptr_end)
				{
					new(l_ptr) T(value_p);
					++l_ptr;
				}
			}
			else
			{
				pointer l_ptr = l_begin_ptr + count_p;
				pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					l_ptr->~T();
					++l_ptr;
				}
				this->m_array_size = count_p;
			}
		}

		pointer l_ptr = l_begin_ptr;
		pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
		while (l_ptr != l_ptr_end)
		{
			*l_ptr = value_p;
			++l_ptr;
		}

		this->m_array_size = count_p;

	}

	template<class InputIterator>
	_CONSTEXPR20_ void assign(InputIterator first_p, InputIterator last_p) noexcept
	{
		FE_STATIC_ASSERT(((std::is_class<InputIterator>::value == false) && (std::is_pointer<InputIterator>::value == false)), "Static Assertion Failure: The template argument InputIterator must be a class or a struct");
		FE_ASSERT(first_p == nullptr, "Assertion Failure: first_p must not be nullptr.");
		FE_ASSERT(last_p == nullptr, "Assertion Failure: last_p must not be nullptr.");
		FE_ASSERT(first_p >= last_p, "Assertion Failure: first_p cannot be greater than or equal to last_p.");

		const size_type l_input_size = last_p - first_p;
		FE_ASSERT(l_input_size == 0, "Assertion Failure: ${%s@0} must not be zero.", TO_STRING(count_p));
		
		pointer l_begin_ptr = this->m_smart_array.get();
		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
			l_begin_ptr = this->m_smart_array.get();

			pointer l_ptr = l_begin_ptr + this->m_array_size;
			pointer const l_ptr_end = l_begin_ptr + l_input_size;
			InputIterator l_input_ptr = first_p + this->m_array_size;
			while (l_ptr != l_ptr_end)
			{
				new(l_ptr) T(*l_input_ptr);
				++l_ptr;
				++l_input_ptr;
			}
		}
		else
		{
			if (l_input_size > this->m_array_size)
			{
				pointer l_ptr = l_begin_ptr + this->m_array_size;
				pointer const l_ptr_end = l_begin_ptr + l_input_size;
				InputIterator l_input_ptr = first_p + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					new(l_ptr) T(*l_input_ptr);
					++l_ptr;
					++l_input_ptr;
				}
			}
			else
			{
				pointer l_ptr = l_begin_ptr + l_input_size;
				pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					l_ptr->~T();
					++l_ptr;
				}
				this->m_array_size = l_input_size;
			}
		}

		pointer l_ptr = l_begin_ptr;
		pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
		InputIterator l_input_ptr = first_p;
		while (l_ptr != l_ptr_end)
		{
			*l_ptr = *l_input_ptr;
			++l_ptr;
			++l_input_ptr;
		}

		this->m_array_size = l_input_size;
	}

	_CONSTEXPR20_ void assign(std::initializer_list<T>&& initializer_list_p) noexcept
	{
		const size_type l_input_size = initializer_list_p.size();
		FE_ASSERT(l_input_size == 0, "Assigning an empty initializer is not allowed.", TO_STRING(count_p));

		pointer l_begin_ptr = this->m_smart_array.get();
		pointer const l_initializer_ptr = const_cast<pointer>(initializer_list_p.begin());

		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
			l_begin_ptr = this->m_smart_array.get();

			pointer l_ptr = l_begin_ptr + this->m_array_size;
			pointer const l_ptr_end = l_begin_ptr + l_input_size;
			pointer l_input_ptr = l_initializer_ptr + this->m_array_size;
			while (l_ptr != l_ptr_end)
			{
				new(l_ptr) T(std::move(*l_input_ptr));
				++l_ptr;
				++l_input_ptr;
			}
		}
		else
		{
			if (l_input_size > this->m_array_size)
			{
				pointer l_ptr = l_begin_ptr + this->m_array_size;
				pointer const l_ptr_end = l_begin_ptr + l_input_size;
				pointer l_input_ptr = l_initializer_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					new(l_ptr) T(std::move(*l_input_ptr));
					++l_ptr;
					++l_input_ptr;
				}
			}
			else
			{
				pointer l_ptr = l_begin_ptr + l_input_size;
				pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					l_ptr->~T();
					++l_ptr;
				}
				this->m_array_size = l_input_size;
			}
		}

		pointer l_ptr = l_begin_ptr;
		pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
		pointer l_input_ptr = l_initializer_ptr;
		while (l_ptr != l_ptr_end)
		{
			*l_ptr = std::move(*l_input_ptr);
			++l_ptr;
			++l_input_ptr;
		}

		this->m_array_size = l_input_size;
	}

	template<class Container>
	_CONSTEXPR20_ void assign_range(container::range<Container>&& range_p)
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		const size_type l_input_size = range_p._end - range_p._begin;
		FE_ASSERT(l_input_size == 0, "Assigning an empty initializer is not allowed.", TO_STRING(count_p));
		pointer l_begin_ptr = this->m_smart_array.get();
		pointer const l_container_data_ptr = range_p._container.get().begin().operator->() + range_p._begin;

		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
			l_begin_ptr = this->m_smart_array.get();

			pointer l_ptr = l_begin_ptr + this->m_array_size;
			pointer const l_ptr_end = l_begin_ptr + l_input_size;
			pointer l_input_ptr = l_container_data_ptr + this->m_array_size;
			while (l_ptr != l_ptr_end)
			{
				new(l_ptr) T(std::move(*l_input_ptr));
				++l_ptr;
				++l_input_ptr;
			}
		}
		else
		{
			if (l_input_size > this->m_array_size)
			{
				pointer l_ptr = l_begin_ptr + this->m_array_size;
				pointer const l_ptr_end = l_begin_ptr + l_input_size;
				pointer l_input_ptr = l_container_data_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					new(l_ptr) T(std::move(*l_input_ptr));
					++l_ptr;
					++l_input_ptr;
				}
			}
			else
			{
				pointer l_ptr = l_begin_ptr + l_input_size;
				pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
				while (l_ptr != l_ptr_end)
				{
					l_ptr->~T();
					++l_ptr;
				}
				this->m_array_size = l_input_size;
			}
		}

		pointer l_ptr = l_begin_ptr;
		pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
		pointer l_input_ptr = l_container_data_ptr;
		while (l_ptr != l_ptr_end)
		{
			*l_ptr = std::move(*l_input_ptr);
			++l_ptr;
			++l_input_ptr;
		}

		this->m_array_size = l_input_size;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ allocator_type& get_allocator() noexcept
	{
		return this->m_smart_array.get_allocator();
	}

	_FORCE_INLINE_ T& operator[](const size_type index_p) noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		FE_ASSERT(index_p >= this->m_array_size, "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(index_p), TO_STRING(this->m_array_size));

		return this->m_smart_array[index_p];
	}

	_FORCE_INLINE_ T& front() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array[0];
	}

	_FORCE_INLINE_ T& back() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array[this->m_array_size - 1];
	}

	_FORCE_INLINE_ T* data() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array.get();
	}

	_FORCE_INLINE_ iterator begin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return iterator(this->m_smart_array.get());
	}

	_FORCE_INLINE_ const_iterator cbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_iterator(this->m_smart_array.get());
	}

	_FORCE_INLINE_ iterator end() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return iterator(this->m_smart_array.get() + this->m_array_size);
	}

	_FORCE_INLINE_ const_iterator cend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_iterator(this->m_smart_array.get() + this->m_array_size);
	}

	_FORCE_INLINE_ reverse_iterator rbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return reverse_iterator(this->end().operator->() - 1);
	}

	_FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_reverse_iterator(this->cend().operator->() - 1);
	}

	_FORCE_INLINE_ reverse_iterator rend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return reverse_iterator(this->begin() - 1);
	}

	_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_reverse_iterator(this->cbegin() - 1);
	}

	_FORCE_INLINE_ var::boolean is_empty() const noexcept
	{
		return this->m_smart_array.get() == nullptr;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type size() const noexcept
	{
		return this->m_array_size;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type max_size() const noexcept { return FE::max_value<size_type> / sizeof(T); }

	_CONSTEXPR20_ _FORCE_INLINE_ void reserve(const size_type new_capacity_p) noexcept
	{
		FE_ASSERT(new_capacity_p == 0, "${%s@0}: Unable to reserve(). ${%s@1} was zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(new_capacity_p));
		FE_ASSERT(new_capacity_p > this->max_size(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(new_capacity_p), TO_STRING(this->max_size()));

		if (new_capacity_p <= this->m_smart_array.capacity())
		{
			return;
		}

		this->m_smart_array.reset(FE::resize_to{ new_capacity_p });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void extend(const size_type extra_capacity_p) noexcept
	{
		FE_ASSERT(extra_capacity_p == 0, "${%s@0}: Unable to extend(). ${%s@1} was zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(extra_capacity_p));
		FE_ASSERT(extra_capacity_p + this->m_smart_array.capacity() > this->max_size(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(extra_capacity_p + this->m_smart_string.capacity()), TO_STRING(this->max_size()));

		this->m_smart_array.reset(FE::resize_to{ extra_capacity_p + this->m_smart_array.capacity() });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type capacity() const noexcept
	{
		return this->m_smart_array.capacity();
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_smart_array.reset(FE::resize_to{ this->m_array_size });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void clear() noexcept
	{
		pointer const ptr_end = this->m_smart_array.get() + this->m_array_size;
		for (pointer ptr = this->m_smart_array.get(); ptr != ptr_end; ++ptr)
		{
			ptr->~T();
		}
		this->m_array_size = 0;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void emplace_back(const T& value_p) noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to emplace_back() to an uninitialized array");
		FE_ASSERT(this->m_array_size >= this->capacity(), "Unable to emplace_back(); memory reallocation must be done explicitly.");

		new(this->m_smart_array.get() + this->m_array_size) T(value_p);
		++this->m_array_size;
	}

	template<class Container>
	_CONSTEXPR23_ _FORCE_INLINE_ void append_range(container::range<Container>&& range_p) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		const size_type l_input_size = range_p._end - range_p._begin;
		FE_ASSERT(l_input_size + this->m_array_size > this->m_smart_array.capacity(), "Unable to append elements to the outside of an array boundary");

		if (l_input_size == 0)
		{
			return;
		}

		if (l_input_size + this->m_array_size > this->m_smart_array.capacity())
		{
			this->extend(l_input_size);
		}

		pointer l_ptr = this->m_smart_array.get() + this->m_array_size;
		pointer const l_ptr_end = l_ptr + l_input_size;
		pointer l_input_ptr = range_p._container.get().begin().operator->() + range_p._begin;
		while (l_ptr != l_ptr_end)
		{
			new(l_ptr) T(*l_input_ptr);
			++l_ptr;
			++l_input_ptr;
		}
		this->m_array_size += l_input_size;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void pop_back() noexcept
	{
		FE_ASSERT(this->m_array_size == 0, "Unable to pop_back() an empty array");
		--this->m_array_size;
		this->m_smart_array[this->m_array_size].~T();
	}

	_FORCE_INLINE_ void resize(const size_type new_size_p) noexcept
	{
		FE_ASSERT(new_size_p == 0, "Assertion Failure: ${%s@0} must not be zero.", TO_STRING(new_size_p));
		
		pointer const l_begin_ptr = this->m_smart_array.get();
		if(new_size_p < this->m_array_size)
		{
			pointer l_ptr = l_begin_ptr + new_size_p;
			pointer const l_ptr_end = l_begin_ptr + this->m_array_size;
			while (l_ptr != l_ptr_end)
			{
				l_ptr->~T();
				++l_ptr;
			}
			this->m_array_size = new_size_p;
		}

		this->m_smart_array.reset(FE::resize_to{ new_size_p });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void swap(array_impl& other_p) noexcept
	{
		algorithm::utility::swap(this->m_smart_array, other_p.m_smart_array);
		algorithm::utility::swap(this->m_array_size, other_p.m_array_size);
	}

	_FORCE_INLINE_ container::range<array_impl> make_range(index_t first_p, index_t last_p) noexcept
	{
		FE_ASSERT(first_p >= last_p, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));
		FE_ASSERT(last_p > this->m_array_size, "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(last_p), TO_STRING(this->m_array_size));

		return container::range<array_impl>(*this, first_p, last_p);
	}
};


template<class T, class Allocator>
class array_impl<T, Allocator, FE::TYPE_TRIVIALITY::_TRIVIAL>
{
	FE_STATIC_ASSERT((std::is_same<T, Allocator::value_type>::value == false), "static assertion failed: enforcing Allocator's value_type to be equivalent to the typename T. The template parameter T must be identical to the value_type of the Allocator.");

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = typename Allocator::size_type;
	using difference_type = typename Allocator::difference_type;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = FE::iterator<FE::contiguous_iterator<T>>;
	using const_iterator = FE::const_iterator<FE::contiguous_iterator<T>>;
	using reverse_iterator = FE::reverse_iterator<FE::contiguous_iterator<T>>;
	using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<T>>;

private:
	FE::exclusive_ptr<T[], Allocator> m_smart_array;
	size_type m_array_size;

public:
	_CONSTEXPR20_ array_impl(const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size() {}

	_CONSTEXPR20_ array_impl(const size_type count_p, const T& value_p = T(), const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size() 
	{
		FE_ASSERT(count_p == 0, "Assertion Failure: ${%s@0} must not be zero", TO_STRING(count_p));

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(count_p);
		this->m_array_size = count_p;
		
		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + count_p;

		while (l_ptr != l_ptr_end)
		{
			*l_ptr = value_p;
			++l_ptr;
		}
	}

	_CONSTEXPR20_ array_impl(const array_impl& other_p) noexcept : m_smart_array(), m_array_size() 
	{
		if (other_p.m_array_size == 0)
		{
			return;
		}

		this->m_smart_array = FE::make_exclusive<T[], Allocator>(other_p.m_array_size);
		this->m_array_size = other_p.m_array_size;

		FE::memcpy<allocator_type::is_address_aligned, allocator_type::is_address_aligned>(this->m_smart_array.get(), other_p.m_smart_array.get(), other_p.m_array_size * sizeof(T));
	}

	_CONSTEXPR20_ array_impl(array_impl&& rvalue_p) noexcept : m_smart_array(), m_array_size() 
	{
		if (rvalue_p.m_array_size == 0)
		{
			return;
		}

		this->m_smart_array = std::move(rvalue_p.m_smart_array);
		this->m_array_size = algorithm::utility::exchange<size_type>(rvalue_p.m_array_size, 0);
	}

	template <class InputIterator>
	_CONSTEXPR20_ array_impl(InputIterator first_p, InputIterator last_p, const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size() 
	{
		FE_STATIC_ASSERT(((std::is_class<InputIterator>::value == false) && (std::is_pointer<InputIterator>::value == false)), "Static Assertion Failure: The template argument InputIterator must be a class or a struct");
		FE_ASSERT(first_p == nullptr, "Assertion Failure: first_p must not be nullptr.");
		FE_ASSERT(last_p == nullptr, "Assertion Failure: last_p must not be nullptr.");
		FE_ASSERT(first_p >= last_p, "Assertion Failure: first_p cannot be greater than or equal to last_p.");

		this->assign(first_p, last_p);
	}

	_CONSTEXPR20_ array_impl(std::initializer_list<T>&& initializer_list_p, const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size(initializer_list_p.size())
	{
		this->assign(std::move(initializer_list_p));
	}

	template<class Container>
	_CONSTEXPR20_ array_impl(container::range<Container>&& range_p, const Allocator& allocator_p = Allocator()) noexcept : m_smart_array(allocator_p), m_array_size()
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		this->assign_range(range_p);
	}

	_CONSTEXPR20_ ~array_impl() noexcept = default;

	_CONSTEXPR20_ array_impl& operator=(const array_impl& other_p) noexcept
	{
		if (other_p.m_array_size == 0)
		{
			return *this;
		}

		if (other_p.m_array_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ other_p.m_array_size });
		}
		this->m_array_size = other_p.m_array_size;

		FE::memcpy<allocator_type::is_address_aligned, allocator_type::is_address_aligned>(this->m_smart_array.get(), other_p.m_smart_array.get(), other_p.m_array_size * sizeof(T));
		return *this;
	}

	_CONSTEXPR20_ array_impl& operator=(array_impl&& rvalue_p) noexcept
	{
		if (rvalue_p.m_array_size == 0)
		{
			return *this;
		}

		this->m_smart_array = std::move(rvalue_p.m_smart_array);
		this->m_array_size = algorithm::utility::exchange<size_type>(rvalue_p.m_array_size, 0);
		return *this;
	}

	_CONSTEXPR20_ void assign(const size_type count_p, const T& value_p) noexcept
	{
		FE_ASSERT(count_p == 0, "Assertion Failure: ${%s@0} must not be zero.", TO_STRING(count_p));

		if (count_p > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ count_p });
		}

		this->m_array_size = count_p;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + count_p;

		while (l_ptr != l_ptr_end)
		{
			*l_ptr = value_p;
			++l_ptr;
		}
	}

	template<class InputIterator>
	_CONSTEXPR20_ void assign(InputIterator first_p, InputIterator last_p) noexcept
	{
		FE_STATIC_ASSERT(((std::is_class<InputIterator>::value == false) && (std::is_pointer<InputIterator>::value == false)), "Static Assertion Failure: The template argument InputIterator must be a class or a struct");
		FE_ASSERT(first_p == nullptr, "Assertion Failure: first_p must not be nullptr.");
		FE_ASSERT(last_p == nullptr, "Assertion Failure: last_p must not be nullptr.");
		FE_ASSERT(first_p >= last_p, "Assertion Failure: first_p cannot be greater than or equal to last_p.");

		const size_type l_input_size = last_p - first_p;
		if (l_input_size == 0)
		{
			return;
		}

		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
		}

		this->m_array_size = l_input_size;

		pointer l_ptr = this->m_smart_array.get();
		pointer const l_ptr_end = l_ptr + l_input_size;

		while (l_ptr != l_ptr_end)
		{
			*l_ptr = *first_p;
			++l_ptr;
			++first_p;
		}
	}

	_CONSTEXPR20_ void assign(std::initializer_list<T>&& initializer_list_p) noexcept
	{
		const size_type l_input_size = initializer_list_p.size();
		if (l_input_size == 0)
		{
			return;
		}

		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
		}
		this->m_array_size = l_input_size;

		FE::memcpy<allocator_type::is_address_aligned>(this->m_smart_array.get(), initializer_list_p.begin(), l_input_size * sizeof(T));
	}
	
	template<class Container>
	_CONSTEXPR20_ void assign_range(container::range<Container>&& range_p)
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		const size_type l_input_size = range_p._end - range_p._begin;
		if (l_input_size == 0)
		{
			return;
		}

		if (l_input_size > this->m_smart_array.capacity())
		{
			this->m_smart_array.reset(FE::resize_to{ l_input_size });
		}
		this->m_array_size = l_input_size;

		FE::memcpy<allocator_type::is_address_aligned>(this->m_smart_array.get(), range_p._container.get().begin().operator->() + range_p._begin, l_input_size * sizeof(T));
	}

	_CONSTEXPR20_ _FORCE_INLINE_ allocator_type& get_allocator() noexcept
	{
		return this->m_smart_array.get_allocator();
	}

	_FORCE_INLINE_ T& operator[](const size_type index_p) noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		FE_ASSERT(index_p >= this->m_array_size, "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(index_p), TO_STRING(this->m_array_size));

		return this->m_smart_array[index_p];
	}

	_FORCE_INLINE_ T& front() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array[0];
	}
	
	_FORCE_INLINE_ T& back() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array[this->m_array_size - 1];
	}

	_FORCE_INLINE_ T* data() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return this->m_smart_array.get();
	}

	_FORCE_INLINE_ iterator begin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return iterator(this->m_smart_array.get());
	}

	_FORCE_INLINE_ const_iterator cbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_iterator(this->m_smart_array.get());
	}

	_FORCE_INLINE_ iterator end() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return iterator(this->m_smart_array.get() + this->m_array_size);
	}

	_FORCE_INLINE_ const_iterator cend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_iterator(this->m_smart_array.get() + this->m_array_size);
	}

	_FORCE_INLINE_ reverse_iterator rbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return reverse_iterator(this->end().operator->() - 1);
	}

	_FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_reverse_iterator(this->cend().operator->() - 1);
	}

	_FORCE_INLINE_ reverse_iterator rend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return reverse_iterator(this->begin() -1);
	}

	_NODISCARD_ _CONSTEXPR20_ _FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to access an uninitialized container.");
		return const_reverse_iterator(this->cbegin() - 1);
	}

	_FORCE_INLINE_ var::boolean is_empty() const noexcept
	{
		return this->m_smart_array.get() == nullptr;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type size() const noexcept
	{
		return this->m_array_size;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type max_size() const noexcept { return FE::max_value<size_type> / sizeof(T); }

	_CONSTEXPR20_ _FORCE_INLINE_ void reserve(const size_type new_capacity_p) noexcept
	{
		FE_ASSERT(new_capacity_p == 0, "${%s@0}: Unable to reserve(). ${%s@1} was zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(new_capacity_p));
		FE_ASSERT(new_capacity_p > this->max_size(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(new_capacity_p), TO_STRING(this->max_size()));

		if (new_capacity_p <= this->m_smart_array.capacity())
		{
			return;
		}

		this->m_smart_array.reset(FE::resize_to{ new_capacity_p });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void extend(const size_type extra_capacity_p) noexcept
	{
		FE_ASSERT(extra_capacity_p == 0, "${%s@0}: Unable to extend(). ${%s@1} was zero.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(extra_capacity_p));
		FE_ASSERT(extra_capacity_p + this->m_smart_array.capacity() > this->max_size(), "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(extra_capacity_p + this->m_smart_string.capacity()), TO_STRING(this->max_size()));

		this->m_smart_array.reset(FE::resize_to{ extra_capacity_p + this->m_smart_array.capacity() });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ size_type capacity() const noexcept
	{
		return this->m_smart_array.capacity();
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void shrink_to_fit() noexcept
	{
		this->m_smart_array.reset(FE::resize_to{ this->m_array_size });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void clear() noexcept
	{
		this->m_array_size = 0;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void emplace_back(const T& value_p) noexcept
	{
		FE_ASSERT(this->m_smart_array.get() == nullptr, "Unable to emplace_back() to an uninitialized array");
		FE_ASSERT(this->m_array_size >= this->capacity(), "Unable to emplace_back(); memory reallocation has to be done explicitly.");
		this->m_smart_array[this->m_array_size] = value_p;
		++this->m_array_size;
	}

	template<class Container>
	_CONSTEXPR23_ _FORCE_INLINE_ void append_range(container::range<Container>&& range_p) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<Container>::value == false, "Static Assertion Failure: The template argument Container must be a class or a struct type.");
		FE_ASSERT(range_p._begin >= range_p._end, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));

		const size_type l_input_size = range_p._end - range_p._begin;
		FE_ASSERT(l_input_size + this->m_array_size > this->m_smart_array.capacity(), "Unable to append elements to the outside of an array boundary");

		if (l_input_size == 0)
		{
			return;
		}

		if (l_input_size + this->m_array_size > this->m_smart_array.capacity())
		{
			this->extend(l_input_size);
		}

		FE::memcpy(this->m_smart_array.get() + this->m_array_size, range_p._container.get().begin().operator->() + range_p._begin, l_input_size * sizeof(T));
		this->m_array_size += l_input_size;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void pop_back() noexcept
	{
		FE_ASSERT(this->m_array_size == 0, "Unable to pop_back() an empty array");
		--this->m_array_size;
	}

	_FORCE_INLINE_ void resize(const size_type new_size_p) noexcept
	{
		FE_ASSERT(new_size_p == 0, "Assertion Failure: ${%s@0} must not be zero.", TO_STRING(new_size_p));

		if (new_size_p < this->m_array_size)
		{
			this->m_array_size = new_size_p;
		}

		this->m_smart_array.reset(FE::resize_to{ new_size_p });
	}

	_CONSTEXPR20_ _FORCE_INLINE_ void swap(array_impl& other_p) noexcept
	{
		algorithm::utility::swap(this->m_smart_array, other_p.m_smart_array);
		algorithm::utility::swap(this->m_array_size, other_p.m_array_size);
	}

	_FORCE_INLINE_ container::range<array_impl> make_range(index_t first_p, index_t last_p) noexcept
	{
		FE_ASSERT(first_p >= last_p, "Assertion Failure: ${%s@0} cannot be greater than or equal to ${%s@1}.", TO_STRING(range_p._begin), TO_STRING(range_p._end));
		FE_ASSERT(last_p > this->m_array_size, "Assertion Failure: ${%s@0} must not be greater than ${%s@1}.", TO_STRING(last_p), TO_STRING(this->m_array_size));

		return container::range<array_impl>(*this, first_p, last_p);
	}
};


END_NAMESPACE
#endif 