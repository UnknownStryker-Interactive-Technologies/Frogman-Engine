#ifndef _FE_CORE_LIST_HXX_
#define _FE_CORE_LIST_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/iterator.hxx>
#include <FE/memory.hxx>
#include <FE/pair.hxx>

#include <iterator> // iterator tags and traits
#include <initializer_list>
#include <memory>
#include <utility>




BEGIN_NAMESPACE(FE)


// eXceptionless assertive doubly linked list
template <typename T, class Allocator = FE::polymorphic_allocator<T>>
class list 
{
	static_assert(std::is_same_v<T, typename Allocator::value_type>, "Static assertion failed: list<T, Allocator>" " requires that Allocator's value_type match " "T");
	static_assert(std::is_object_v<T>, "Static assertion failed: the C++ Standard forbids containers of non-object types");
	
	class list_iterator;

	class node
	{
		friend class list;
		friend class list_iterator;

	public:
		T _value;

	private:
		node* m_prev;
		node* m_next;

	public:
		constexpr node() noexcept 
			:	m_prev(nullptr),
				m_next(nullptr),
				_value()
		{
			FE_ASSERT((FE::byte*)&_value < (FE::byte*)&m_prev, "Assertion failed: this breaks the address alignment requirements of the T expected by many dependent classes.");
		}

		constexpr node(const T& value_p) noexcept
			:	m_prev(nullptr),
				m_next(nullptr),
				_value(value_p)
		{
			FE_ASSERT((FE::byte*)&_value < (FE::byte*)&m_prev, "Assertion failed: this breaks the address alignment requirements of the T expected by many dependent classes.");
		}

		constexpr node(T&& value_p) noexcept
			:	m_prev(nullptr),
				m_next(nullptr),
				_value( std::move(value_p) )
		{
			FE_ASSERT((FE::byte*)&_value < (FE::byte*)&m_prev, "Assertion failed: this breaks the address alignment requirements of the T expected by many dependent classes.");
		}

		template <typename... Arguments>
		constexpr node(Arguments&&... arguments_p) noexcept
			:	m_prev(nullptr),
				m_next(nullptr),
				_value( std::forward<Arguments>(arguments_p)... )
		{
			//static_assert(std::is_nothrow_constructible_v<T, Arguments&&...>, "emplace_back requires nothrow-constructible T for the provided Args.");
			FE_ASSERT((FE::byte*)&_value < (FE::byte*)&m_prev, "Assertion failed: this breaks the address alignment requirements of the T expected by many dependent classes.");
		}


		constexpr ~node() noexcept = default;
	};

	class list_iterator
	{
		friend class list;
		friend class list_iterator;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = node;
		using difference_type = var::ptrdiff;
		using pointer = node*;
		using reference = node&;
		using const_pointer = const node*;
		using const_reference = const node&;

	protected:
		pointer m_iterator;

	public:
		constexpr list_iterator() noexcept : m_iterator() {}
		constexpr list_iterator(const_pointer const value_p) noexcept : m_iterator(const_cast<pointer>(value_p)) {}
		constexpr list_iterator(const list_iterator& other_p) noexcept : m_iterator(other_p.m_iterator) {}
		constexpr list_iterator(list_iterator&& other_p) noexcept : m_iterator(other_p.m_iterator) { other_p.m_iterator = nullptr; }
		constexpr ~list_iterator() noexcept {}

		_FE_FORCE_INLINE_ constexpr boolean is_null() const noexcept { return m_iterator == nullptr; }

		_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return *m_iterator;
		}
		_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return m_iterator;
		}

		_FE_FORCE_INLINE_ constexpr const reference operator*() const noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return *m_iterator;
		}
		_FE_FORCE_INLINE_ constexpr const pointer operator->() const noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return m_iterator;
		}


		_FE_FORCE_INLINE_ constexpr void operator++() noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			m_iterator = m_iterator->m_next;
		}
		_FE_FORCE_INLINE_ constexpr void operator--() noexcept
		{
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			m_iterator = m_iterator->m_prev;
		}


		_FE_FORCE_INLINE_ constexpr pointer operator+(const difference_type pointer_offset_p) const noexcept
		{
			node* l_result = m_iterator;
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				FE_NEGATIVE_ASSERT(l_result == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
				l_result = l_result->m_next;
			}
			return l_result;
		}
		_FE_FORCE_INLINE_ constexpr void operator+=(const difference_type pointer_offset_p) noexcept
		{
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
				m_iterator = m_iterator->m_next;
			}
		}


		_FE_FORCE_INLINE_ constexpr pointer operator-(const difference_type pointer_offset_p) const noexcept
		{
			node* l_result = m_iterator;
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				FE_NEGATIVE_ASSERT(l_result == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
				l_result = l_result->m_prev;
			}
			return l_result;
		}
		_FE_FORCE_INLINE_ constexpr void operator-=(const difference_type pointer_offset_p) noexcept
		{
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
				m_iterator = m_iterator->m_prev;
			}
		}
		_FE_FORCE_INLINE_ constexpr difference_type operator-(const list_iterator& value_p) const noexcept
		{
			difference_type l_distance = 0;
			for (node* it = value_p.m_iterator; it != m_iterator; it = it->m_next)
			{
				FE_NEGATIVE_ASSERT(it == nullptr, "${%s@0}: The iterator is transposed.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
				++l_distance;
			}
			return l_distance;
		}


		_FE_DEPRECATED_ constexpr const reference operator[](const difference_type index_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the random access [] index operator.");
			return m_iterator[0];
		}
		_FE_DEPRECATED_ constexpr reference operator[](const difference_type index_p) noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the random access [] index operator.");
			return m_iterator[0];
		}


		_FE_FORCE_INLINE_ constexpr list_iterator& operator=(const list_iterator& other_p) noexcept
		{
			m_iterator = other_p.m_iterator;
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr list_iterator& operator=(list_iterator&& rvalue_p) noexcept
		{
			m_iterator = rvalue_p.m_iterator;
			return *this;
		}


		_FE_DEPRECATED_ constexpr boolean operator<(const list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the operator < .");
			return m_iterator < other_p.m_iterator;
		}
		_FE_DEPRECATED_ constexpr boolean operator<=(const list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the operator <= .");
			return m_iterator <= other_p.m_iterator;
		}

		_FE_DEPRECATED_ constexpr boolean operator>(const list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the operator > .");
			return m_iterator > other_p.m_iterator;
		}
		_FE_DEPRECATED_ constexpr boolean operator>=(const list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: list_iterator does not support the operator >= .");
			return m_iterator >= other_p.m_iterator;
		}

		_FE_FORCE_INLINE_ constexpr boolean operator==(const list_iterator& other_p) const noexcept
		{
			return m_iterator == other_p.m_iterator;
		}
		_FE_FORCE_INLINE_ constexpr boolean operator!=(const list_iterator& other_p) const noexcept
		{
			return m_iterator != other_p.m_iterator;
		}
	};

	template <class WrappedIterator>
	class list_iterator_wrapper
	{
		friend class list;
		friend class list_iterator;

	public:
		using wrapped_iterator_type = WrappedIterator;
		using iterator_category = typename WrappedIterator::iterator_category;
		using value_type = T;
		using difference_type = typename WrappedIterator::difference_type;
		using pointer = T*;
		using reference = T&;
		using const_pointer = const T*;
		using const_reference = const T&;

	private:
		WrappedIterator m_wrapped;

	public:
		constexpr list_iterator_wrapper() noexcept : m_wrapped() {}
		constexpr list_iterator_wrapper(const WrappedIterator& value_p) noexcept : m_wrapped(value_p) {}
		constexpr list_iterator_wrapper(WrappedIterator&& value_p) noexcept : m_wrapped( std::move(value_p) ) {}
		constexpr list_iterator_wrapper(const list_iterator_wrapper& other_p) noexcept : m_wrapped(other_p.m_wrapped) {}
		constexpr list_iterator_wrapper(list_iterator_wrapper&& other_p) noexcept : m_wrapped( std::move(other_p.m_wrapped) ) {}
		constexpr ~list_iterator_wrapper() noexcept = default;

		_FE_FORCE_INLINE_ constexpr const WrappedIterator unwrap() const noexcept { return m_wrapped; }
		_FE_FORCE_INLINE_ constexpr WrappedIterator unwrap() noexcept { return m_wrapped; }

		_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
			requires (!FE::is_const_iterator<WrappedIterator>::value) && (!FE::is_const_reverse_iterator<WrappedIterator>::value)
		{
			return m_wrapped.operator*()._value;
		}
		_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
			requires (!FE::is_const_iterator<WrappedIterator>::value) && (!FE::is_const_reverse_iterator<WrappedIterator>::value)
		{
			return &(m_wrapped->_value);
		}

		_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
		{
			return m_wrapped.operator*()._value;
		}
		_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
		{
			return &(m_wrapped.operator->()->_value);
		}


		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator++() noexcept
		{
			m_wrapped.operator++();
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper operator++(int) noexcept
		{
			list_iterator_wrapper l_temporary = *this;
			m_wrapped.operator++();
			return l_temporary;
		}


		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator--() noexcept
		{
			m_wrapped.operator--();
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator--(int) noexcept
		{
			list_iterator_wrapper l_temporary = *this;
			m_wrapped.operator--();
			return l_temporary;
		}


		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper operator+(const difference_type pointer_offset_p) const noexcept
		{
			return m_wrapped.operator+(pointer_offset_p);
		}
		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator+=(const difference_type pointer_offset_p) noexcept
		{
			m_wrapped.operator+=(pointer_offset_p);
			return *this;
		}


		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper operator-(const difference_type pointer_offset_p) const noexcept
		{
			return m_wrapped.operator-(pointer_offset_p);
		}

		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator-=(const difference_type pointer_offset_p) noexcept
		{
			m_wrapped.operator-=(pointer_offset_p);
			return *this;
		}

		_FE_FORCE_INLINE_ constexpr difference_type operator-(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator-(other_p.m_wrapped);
		}


		_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
		{
			return m_wrapped.operator[](index_p)._value;
		}
		_FE_FORCE_INLINE_ constexpr reference operator[](const difference_type index_p) noexcept
		{
			return m_wrapped.operator[](index_p)._value;
		}


		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator=(typename WrappedIterator::const_pointer value_p) noexcept
		{
			m_wrapped.operator=(value_p);
			return *this;
		}

		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator=(const list_iterator_wrapper& other_p) noexcept
		{
			m_wrapped.operator=(other_p.m_wrapped);
			return *this;
		}

		_FE_FORCE_INLINE_ constexpr list_iterator_wrapper& operator=(list_iterator_wrapper&& other_p) noexcept
		{
			m_wrapped.operator=(other_p.m_wrapped);
			other_p.m_wrapped = nullptr;
			return *this;
		}


		_FE_FORCE_INLINE_ constexpr boolean operator<(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator<(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator<=(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator<=(other_p.m_wrapped);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator>(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator>(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator>=(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator>=(other_p.m_wrapped);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator==(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator==(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator!=(const list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator!=(other_p.m_wrapped);
		}


		_FE_FORCE_INLINE_ constexpr boolean operator<(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator<(value_p);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator<=(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator<=(value_p);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator>(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator>(value_p);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator>=(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator>=(value_p);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator==(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator==(value_p);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator!=(typename WrappedIterator::const_pointer value_p) const noexcept
		{
			return m_wrapped.operator!=(value_p);
		}
	};

public:
	using value_type = T;
	using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<node>;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = list_iterator_wrapper<FE::iterator<list_iterator>>;
	using const_iterator = list_iterator_wrapper<FE::const_iterator<list_iterator>>;
	using reverse_iterator = list_iterator_wrapper<FE::reverse_iterator<list_iterator>>;
	using const_reverse_iterator = list_iterator_wrapper<FE::const_reverse_iterator<list_iterator>>;

private:
	_FE_NO_UNIQUE_ADDRESS_ mutable allocator_type m_allocator;
	node* m_front;
	node* m_back;
	size_type m_size;

public: // Member functions
	constexpr list() noexcept // does not allocate
		:	m_allocator(allocator_type()),
			m_front(nullptr),
			m_back(nullptr),
			m_size(0)
	{}

	explicit constexpr list(const allocator_type& alloc_p) noexcept // does not allocate
		:	m_allocator(alloc_p),
			m_front(nullptr), 
			m_back(nullptr), 
			m_size(0)
	{}

	explicit constexpr list(const size_type count_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	list(count_p, T(), alloc_p)
	{
	}

	explicit constexpr list(const size_type count_p, const T& value_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr), 
			m_back(nullptr), 
			m_size(count_p)
	{
		FE_ASSERT(count_p > 0, "Assertion failed: count must be greater than 0");
		FE::pair<node*, node*> l_nodes = __allocate_and_construct_nodes(count_p, value_p);
		m_front = l_nodes._first;
		m_back = l_nodes._second;
	}

	template <class Iterator>
	constexpr list(Iterator first_p, Iterator last_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr), 
			m_back(nullptr), 
			m_size(std::distance(first_p, last_p))
	{
		static_assert(FE::has_iterator_category<Iterator>::value || std::is_pointer_v<Iterator>, "Static assertion failed: Iterator must be an iterator type.");

		FE_ASSERT(m_size == (size_type)std::distance(first_p, last_p), "Assertion failed: m_size == std::distance(first_p, last_p).");
		if (m_size <= 0)
		{
			return;
		}

		FE::pair<node*, node*> l_nodes = __allocate_and_construct_nodes(m_size);
		m_front = l_nodes._first;
		m_back = l_nodes._second;

		for (node* it = m_front; it != m_back; it = it->m_next)
		{
			it->_value = *first_p;
			++first_p;
		}
		m_back->_value = *first_p;
	}

	constexpr list(const list& other_p)
		:	m_allocator(other_p.m_allocator),
			m_front(nullptr), 
			m_back(nullptr), 
			m_size(other_p.m_size)
	{
		FE_ASSERT(m_front == nullptr, "Assertion failed: the front pointer must not be copy-assigned from the other.");
		FE_ASSERT(m_back == nullptr, "Assertion failed: the back pointer must not be copy-assigned from the other.");
		FE_ASSERT(m_size == other_p.m_size, "Assertion failed: m_size must be equal to other_p.m_size.");
		if (m_size <= 0)
		{
			return;
		}

		FE::pair<node*, node*> l_nodes = __allocate_and_construct_nodes(m_size);
		m_front = l_nodes._first;
		m_back = l_nodes._second;

		node* l_other_it = other_p.m_front;
		for (node* it = m_front; it != m_back; it = it->m_next)
		{
			it->_value = l_other_it->_value;
			l_other_it = l_other_it->m_next;
		}
		m_back->_value = l_other_it->_value;
	}

	constexpr list(list&& other_p) noexcept
		:	m_allocator(other_p.m_allocator),
			m_front(other_p.m_front),
			m_back(other_p.m_back),
			m_size(other_p.m_size)
	{
		other_p.m_front = nullptr;
		other_p.m_back = nullptr;
		other_p.m_size = 0;
	}

	constexpr list(const list& other_p, const allocator_type& alloc_p) noexcept
		:	list(other_p)
	{}

	constexpr list(list&& other_p, const allocator_type& alloc_p) noexcept
		:	m_allocator(alloc_p),
			m_front(other_p.m_front),
			m_back(other_p.m_back),
			m_size(other_p.m_size)
	{
		other_p.m_front = nullptr;
		other_p.m_back = nullptr;
		other_p.m_size = 0;
	}

	constexpr list(std::initializer_list<T> init_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	list(init_p.begin(), init_p.end(), alloc_p)
	{}


	constexpr ~list() noexcept
	{
		if (m_size == 0)
		{
			return;
		}
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when size is greater than 0.");
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when size is greater than 0.");
		__deallocate_and_destruct_all(m_front, m_back);
	}


	constexpr list& operator=(const list& other_p) noexcept
	{
		if (this == &other_p) // prevent self-assignment
		{
			return *this;
		}

		if (other_p.m_size <= 0) // other is empty
		{
			FE_ASSERT(other_p.m_front == nullptr);
			FE_ASSERT(other_p.m_back == nullptr);
			clear(); // clear this list
			return *this;
		}

		resize(other_p.m_size); // resize this list to match the other list size

		node* l_other_it = other_p.m_front;
		for (node* it = m_front; it != m_back; it = it->m_next)
		{
			it->_value = l_other_it->_value;
			l_other_it = l_other_it->m_next;
		} 
		m_back->_value = l_other_it->_value;
		return *this;
	}

	constexpr list& operator=(list&& other_p) noexcept
	{
		if (this == &other_p) // prevent self-assignment
		{
			return *this;
		}

		if (m_size > 0) // deallocate the current resources
		{
			__deallocate_and_destruct_all(m_front, m_back);
		}

		// transfer the ownership
		m_allocator = other_p.m_allocator;
		m_front = other_p.m_front;
		m_back = other_p.m_back;
		m_size = other_p.m_size;

		// nullify the other list
		other_p.m_front = nullptr;
		other_p.m_back = nullptr;
		other_p.m_size = 0;
		return *this;
	}

	constexpr list& operator=(const std::initializer_list<value_type>& ilist_p) noexcept
	{
		if (ilist_p.size() == 0) // empty initializer list, do nothing.
		{
			clear();
			return *this;
		}

		resize(ilist_p.size()); // resize this list to match the initializer list size

		const_pointer l_ilist_it = ilist_p.begin();
		for (node* it = m_front; it != m_back; it = it->m_next)
		{
			it->_value = *l_ilist_it;
			++l_ilist_it;
		}
		m_back->_value = *l_ilist_it;
		return *this;
	}

	constexpr list& operator=(std::initializer_list<value_type>&& ilist_p) noexcept
	{
		if (ilist_p.size() == 0) // empty initializer list, do nothing.
		{
			clear();
			return *this;
		}

		resize(ilist_p.size()); // resize this list to match the initializer list size

		pointer l_ilist_it = const_cast<pointer>(ilist_p.begin());
		for (node* it = m_front; it != m_back; it = it->m_next)
		{
			it->_value = std::move(*l_ilist_it); // steal it, to avoid a copy.
			++l_ilist_it;
		}
		m_back->_value = std::move(*l_ilist_it);
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr const allocator_type& get_allocator() const noexcept { return m_allocator; }



	// Element Access
	_FE_FORCE_INLINE_ constexpr reference front() noexcept
	{
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when accessing front().");
		return m_front->_value; 
	}

	_FE_FORCE_INLINE_ constexpr const_reference front() const noexcept
	{ 
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when accessing front().");
		return m_front->_value; 
	}


	_FE_FORCE_INLINE_ constexpr reference back() noexcept
	{ 
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when accessing back().");
		return m_back->_value; 
	}

	_FE_FORCE_INLINE_ constexpr const_reference back() const noexcept
	{ 
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when accessing back().");
		return m_back->_value; 
	}



	// Iterators
	_FE_FORCE_INLINE_ constexpr iterator begin() noexcept { return iterator{ typename iterator::wrapped_iterator_type(m_front) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator begin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(m_front) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator cbegin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(m_front) }; }


	_FE_FORCE_INLINE_ constexpr iterator end() noexcept { return iterator{ typename iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator end() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator cend() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(nullptr) }; }


	_FE_FORCE_INLINE_ constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{ typename reverse_iterator::wrapped_iterator_type(m_back) }; }
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{ typename const_reverse_iterator::wrapped_iterator_type(m_back) }; }
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ typename const_reverse_iterator::wrapped_iterator_type(m_back) }; }


	_FE_FORCE_INLINE_ constexpr reverse_iterator rend() noexcept { return reverse_iterator{ typename reverse_iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{ typename const_reverse_iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ typename const_reverse_iterator::wrapped_iterator_type(nullptr) }; }



	// Capacity
	_FE_FORCE_INLINE_ constexpr boolean is_empty() const noexcept { return m_size == 0; }
	_FE_FORCE_INLINE_ constexpr size_type size() const noexcept { return m_size; }



	// Modifiers
	constexpr void clear() noexcept
	{
		if (m_size == 0) // already empty, do nothing.
		{
			return;
		}
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when size is greater than 0.");
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when size is greater than 0.");
		__deallocate_and_destruct_all(m_front, m_back);
		m_front = nullptr;
		m_back = nullptr;
		m_size = 0;
	}	


	constexpr iterator insert(const_iterator pos_p, T&& value_p) noexcept
	{
		if (pos_p == cend())
		{
			push_back(std::move(value_p));
			return FE::iterator_cast<iterator>(m_back);
		}
		node*l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node( std::move(value_p) );
		l_new->m_next = FE::iterator_cast<node*>(pos_p);
		l_new->m_prev = FE::iterator_cast<node*>(pos_p)->m_prev;
		if (l_new->m_prev != nullptr)
		{
			l_new->m_prev->m_next = l_new;
		}
		FE::iterator_cast<node*>(pos_p)->m_prev = l_new;

		if (pos_p == m_front) // inserted at the front
		{
			m_front = l_new; // update the front pointer
		}
		++m_size; // increase the size
		return FE::iterator_cast<iterator>(l_new);
	}

	constexpr iterator insert(const_iterator pos_p, const T& value_p) noexcept
	{
		if (pos_p == cend())
		{
			push_back(value_p);
			return FE::iterator_cast<iterator>(m_back);
		}
		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node(value_p);
		l_new->m_next = FE::iterator_cast<node*>(pos_p);
		l_new->m_prev = FE::iterator_cast<node*>(pos_p)->m_prev;
		if (l_new->m_prev != nullptr)
		{
			l_new->m_prev->m_next = l_new;
		}
		FE::iterator_cast<node*>(pos_p)->m_prev = l_new;

		if (pos_p == m_front) // inserted at the front
		{
			m_front = l_new; // update the front pointer
		}
		++m_size; // increase the size
		return FE::iterator_cast<iterator>(l_new);
	}

	constexpr iterator insert(const_iterator pos_p, size_type count_p, const T& value_p) noexcept
	{
		if (count_p == 0) // nothing to insert, return the position as is.
		{
			return FE::iterator_cast<iterator>(pos_p);
		}

		iterator l_begin = insert(pos_p, value_p);
		for (size_type i = 1; i < count_p; ++i)
		{
			insert(pos_p, value_p);
		}
		return l_begin;
	}

	template <class Iterator>
	constexpr iterator insert(const_iterator pos_p, Iterator first_p, Iterator last_p) noexcept
	{
		static_assert(FE::has_iterator_category<Iterator>::value || std::is_pointer_v<Iterator>, "Static assertion failed: Iterator must be an iterator type.");

		if (first_p == last_p) // nothing to insert, return the position as is.
		{
			return FE::iterator_cast<iterator>(pos_p);
		}

		iterator l_begin = insert(pos_p, *first_p);
		++first_p;
		for (; first_p != last_p; ++first_p)
		{
			insert(pos_p, *first_p);
		}
		return l_begin;
	}

	constexpr iterator insert(const_iterator pos_p, std::initializer_list<T> ilist_p) noexcept
	{
		FE_ASSERT(pos_p != nullptr, "Assertion failed: pos_p must not be null.");
		if (ilist_p.size() == 0) // nothing to insert, return the position as is.
		{
			return FE::iterator_cast<iterator>(pos_p);
		}

		pointer l_value_ptr = const_cast<pointer>(ilist_p.begin());
		iterator l_begin = insert(pos_p, *l_value_ptr);
		++l_value_ptr;
		for (size_type i = 1; i < ilist_p.size(); ++i)
		{
			insert(pos_p, *l_value_ptr);
		}
		return l_begin;
	}


	template <typename... Arguments>
	constexpr iterator emplace(const_iterator pos_p, Arguments&&... arguments_p) noexcept
	{
		//static_assert(std::is_nothrow_constructible_v<T, Arguments&&...>, "emplace_back requires nothrow-constructible T for the provided Args.");

		if (pos_p == cend())
		{
			emplace_back( std::forward<Arguments>(arguments_p)... );
			return FE::iterator_cast<iterator>(m_back);
		}
		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node( std::forward<Arguments>(arguments_p)... );
		l_new->m_next = FE::iterator_cast<node*>(pos_p);
		l_new->m_prev = FE::iterator_cast<node*>(pos_p)->m_prev;
		if (l_new->m_prev != nullptr)
		{
			l_new->m_prev->m_next = l_new;
		}
		FE::iterator_cast<node*>(pos_p)->m_prev = l_new;

		if (pos_p == m_front) // inserted at the front
		{
			m_front = l_new; // update the front pointer
		}
		++m_size; // increase the size
		return FE::iterator_cast<iterator>(l_new);
	}


	constexpr iterator erase(const_iterator pos_p) noexcept
	{
		FE_ASSERT(pos_p != nullptr, "Assertion failed: pos_p must not be null.");
		node* l_prev = FE::iterator_cast<node*>(pos_p)->m_prev;
		node* l_next = FE::iterator_cast<node*>(pos_p)->m_next;

		if (l_prev != nullptr)
		{
			l_prev->m_next = l_next;
		}
		else
		{
			FE_ASSERT(pos_p == m_front, "Assertion failed: cannot erase() an alien node.");
			m_front = l_next; // update the front pointer
		}

		if (l_next != nullptr)
		{
			l_next->m_prev = l_prev;
		}
		else
		{
			FE_ASSERT(pos_p == m_back, "Assertion failed: cannot erase() an alien node.");
			m_back = l_prev; // update the back pointer
		}

		FE::boolean l_is_last = (pos_p == m_back);
		FE::iterator_cast<node*>(pos_p)->~node();
		m_allocator.deallocate(FE::iterator_cast<node*>(pos_p), 1);
		--m_size; // decrease the size

		if (l_is_last == true)
		{
			return end(); // return end() if the last element was erased.
		}
		return FE::iterator_cast<iterator>(l_next); // return the next element as the new position.
	}

	constexpr iterator erase(const_iterator first_p, const_iterator last_p) noexcept
	{
		if (first_p == last_p) // nothing to erase, return last_p as is.
		{
			return FE::iterator_cast<iterator>(last_p);
		}
		FE_ASSERT(first_p != nullptr, "Assertion failed: first_p must not be null.");
		FE_ASSERT(last_p != nullptr, "Assertion failed: last_p must not be null.");

		const_iterator l_to_erase;
		const_iterator l_it = first_p;
		for (; l_it != last_p;)
		{
			l_to_erase = l_it;
			++l_it;
			erase(l_to_erase);
		}
		return erase(l_it);
	}
	
	
	constexpr void push_back(const T& value_p) noexcept 
	{
		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node( value_p );

		if (m_size == 0)
		{
			m_front = l_new;
			m_back = l_new;
			++m_size;
			return;
		}

		FE_ASSERT(m_back->m_next == nullptr, "Assertion failed: m_back->m_next must be nullptr when emplacing back.");
		m_back->m_next = l_new;
		l_new->m_prev = m_back;
		m_back = l_new;
		++m_size;
	}

	constexpr void push_back(T&& value_p) noexcept
	{
		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node( std::move(value_p) );
		
		if (m_size == 0)
		{
			m_front = l_new;
			m_back = l_new;
			++m_size;
			return;
		}

		FE_ASSERT(m_back->m_next == nullptr, "Assertion failed: m_back->m_next must be nullptr when emplacing back.");
		m_back->m_next = l_new;
		l_new->m_prev = m_back;
		m_back = l_new;
		++m_size;
	}


	template <typename... Arguments>
	constexpr reference emplace_back(Arguments&&... arguments_p) noexcept
	{
		//static_assert(std::is_nothrow_constructible_v<T, Arguments&&...>, "emplace_back requires nothrow-constructible T for the provided Args.");

		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node( std::forward<Arguments>(arguments_p)... );

		if (m_size == 0)
		{
			m_front = l_new;
			m_back = l_new;
			++m_size;
			return m_back->_value;
		}

		FE_ASSERT(m_back->m_next == nullptr, "Assertion failed: m_back->m_next must be nullptr when emplacing back.");
		m_back->m_next = l_new;
		l_new->m_prev = m_back;
		m_back = l_new;
		++m_size;
		return m_back->_value;
	}


	_FE_FORCE_INLINE_ constexpr void pop_back() noexcept
	{
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when popping back.");
		const_iterator l_back{ m_back };
		erase(l_back);
	}


	_FE_FORCE_INLINE_ constexpr void push_front(const T& value_p) noexcept
	{
		const_iterator l_front{ m_front };
		insert(l_front, value_p);
	}

	_FE_FORCE_INLINE_ constexpr void push_front(T&& value_p) noexcept
	{
		const_iterator l_front{ m_front };
		insert(l_front, std::move(value_p));
	}


	template <typename... Arguments>
	constexpr reference emplace_front(Arguments&&... arguments_p) noexcept
	{
		//static_assert(std::is_nothrow_constructible_v<T, Arguments&&...>, "emplace_back requires nothrow-constructible T for the provided Args.");

		if (m_front == nullptr)
		{
			return emplace_back(std::forward<Arguments>(arguments_p)...);
		}
		node* l_new = m_allocator.allocate(1);
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node(std::forward<Arguments>(arguments_p)...);
		l_new->m_next = m_front;
		l_new->m_prev = m_front->m_prev;
		if (l_new->m_prev != nullptr)
		{
			l_new->m_prev->m_next = l_new;
		}
		m_front->m_prev = l_new;
		m_front = l_new; // update the front pointer
		++m_size; // increase the size
		return l_new->_value;
	}


	_FE_FORCE_INLINE_ constexpr void pop_front() noexcept
	{
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when popping front.");
		const_iterator l_front{ m_front };
		erase(l_front);
	}


	constexpr void resize(size_type count_p) noexcept
	{
		if (m_size == count_p) // sizes are equal, do nothing.
		{
			return;
		} 

		// different sizes, modify the list.
		if (m_size > count_p) // this list is larger than the request, trim this list.
		{
			node* l_deletion_range_end = m_front;
			const size_type l_diff = m_size - count_p;
			for (size_type i = 0; i < l_diff; ++i) // calculate the end of the deletion range
			{
				l_deletion_range_end = l_deletion_range_end->m_next;
			}
			__deallocate_and_destruct_from_begin_to_before_end(m_front, l_deletion_range_end); // trim this list.
			m_front = l_deletion_range_end; // update the front pointer
		}
		else if (m_size < count_p) // this list is smaller than the request, append new nodes.
		{
			FE::pair<node*, node*> l_new_nodes = __allocate_and_construct_nodes(count_p - m_size);
			if (m_size == 0)
			{
				m_front = l_new_nodes._first;
				m_back = l_new_nodes._second;
			}
			else
			{
				FE_ASSERT(m_back->m_next == nullptr, "Assertion failed: m_back->m_next must be nullptr before appending new nodes.");
				m_back->m_next = l_new_nodes._first;
				l_new_nodes._first->m_prev = m_back;
				m_back = l_new_nodes._second;
			}
		}

		m_size = count_p;
	}


	_FE_FORCE_INLINE_ void swap(list& other_p) noexcept
	{
		std::swap(m_allocator, other_p.m_allocator);
		std::swap(m_front, other_p.m_front);
		std::swap(m_back, other_p.m_back);
		std::swap(m_size, other_p.m_size);
	}



	// Operations
	void swap_extremes() noexcept
	{
		if (m_size < 2)
		{
			return; // nothing to swap
		}
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when swapping boundaries.");
		FE_ASSERT(m_back != nullptr, "Assertion failed: m_back must not be nullptr when swapping boundaries.");

		node* l_tmp_front = m_front->m_next;
		node* l_tmp_back = m_back->m_prev;

		FE_ASSERT(l_tmp_front != nullptr, "Assertion failed: tmp_front must not be nullptr when swapping boundaries.");
		FE_ASSERT(l_tmp_back != nullptr, "Assertion failed: tmp_back must not be nullptr when swapping boundaries.");

		if (m_size == 2)
		{
			m_front = l_tmp_front;
			m_back = l_tmp_back;

			m_front->m_next = m_back;
			m_back->m_prev = m_front;

			m_front->m_prev = nullptr;
			m_back->m_next = nullptr;
			return;
		}

		l_tmp_back->m_next = m_front;
		m_front->m_prev = l_tmp_back;

		l_tmp_front->m_prev = m_back;
		m_back->m_next = l_tmp_front;

		node* l_tmp = m_front;
		m_front = m_back;
		m_back = l_tmp;
		
		m_front->m_prev = nullptr;
		m_back->m_next = nullptr;
	}

	constexpr void _FE_VECTOR_CALL_ splice(const_iterator pos_p, list& other_p, const_iterator it_p) noexcept
	{
		FE_ASSERT(other_p.is_empty() == false, "Assertion failed: the input source container must not be empty.");
		FE_ASSERT(it_p != other_p.cend(), "Assertion failed: it_p must not be other_p.cend().");

#ifdef _DEBUG_
		if (is_empty() == false)
		{
			auto l_it = cbegin();
			for (; l_it != cend(); ++l_it)
			{
				if (l_it == pos_p)
				{
					break;
				}
			}

			FE_ASSERT(l_it != cend(), "Assertion failed: pos_p does not belong to *this; please refer to: https://en.cppreference.com/w/cpp/container/list/splice.html");
		}
#endif

		if ( ( pos_p == it_p ) ||
			 ( pos_p == (it_p+1) ) ) _FE_UNLIKELY_
		{
			if ( ((pos_p == nullptr) && (is_empty() == true)) == false ) _FE_UNLIKELY_
			{
				return; // has no effect; see: https://en.cppreference.com/w/cpp/container/list/splice.html
			}
		}

		other_p.m_size -= 1;
		node* l_input = nullptr;

		if (&(other_p.front()) == &(*it_p))
		{
			l_input = other_p.m_front;

			other_p.m_front = other_p.m_front->m_next;

			if (other_p.m_front != nullptr) 
			{
				other_p.m_front->m_prev = nullptr;
			}
			else // the list is emptied; other_p.m_front is nullptr.
			{
				other_p.m_back = nullptr;
			}

			l_input->m_next = nullptr;
			l_input->m_prev = nullptr;
		}
		else if (&(other_p.back()) == &(*it_p))
		{
			l_input = other_p.m_back;

			other_p.m_back = other_p.m_back->m_prev;

			if (other_p.m_back != nullptr)
			{
				other_p.m_back->m_next = nullptr;
			}
			else // the list is emptied; other_p.m_back is nullptr.
			{
				other_p.m_front = nullptr;
			}

			l_input->m_next = nullptr;
			l_input->m_prev = nullptr;
		}
		else // in between
		{
			l_input = FE::iterator_cast<node*>(it_p);

			l_input->m_prev->m_next = l_input->m_next;
			l_input->m_next->m_prev = l_input->m_prev;

			l_input->m_next = nullptr;
			l_input->m_prev = nullptr;
		}


		FE_ASSERT(l_input != nullptr);
		if (is_empty() == true)
		{
			m_front = l_input;
			m_back = l_input;
			++m_size;
			return;
		}
		else if (&front() == &(*pos_p))
		{
			m_front->m_prev = l_input;
			l_input->m_next = m_front;

			m_front = l_input; // update the front pointer
			++m_size;
			return;
		}
		else if (&back() == &(*pos_p))
		{
			m_back->m_next = l_input;
			l_input->m_prev = m_back;

			m_back = l_input; // update the back pointer
			++m_size;
			return;
		}
		else // in between
		{
			node* l_destination = FE::iterator_cast<node*>(pos_p);
			l_destination->m_next->m_prev = l_input;
			l_input->m_next = l_destination->m_next;

			l_destination->m_next = l_input;
			l_input->m_prev = l_destination;
			++m_size;
		}
	}


private: // these are private internal functions mean to operate on the given arguments; these never directly modify the member variables, except for the m_allocator's state.
	FE::pair<node*, node*> __allocate_and_construct_nodes(const size_type count_p, const T& value_p = T()) const noexcept
	{
		FE_ASSERT(count_p > 0, "Assertion failed: count must be greater than 0");

		node* l_front = m_allocator.allocate(1);
		FE_ASSERT(l_front != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_front) node( value_p );

		node* l_node = l_front;
		for (size_type i = 1; i < count_p; ++i)
		{
			node* l_new = m_allocator.allocate(1);
			FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
			::new(l_new) node( value_p );

			l_node->m_next = l_new;
			l_new->m_prev = l_node;
			l_node = l_new;
		}
		return FE::pair<node*, node*>{ l_front, l_node };
	}
	// this does not modify the m_size
	void __deallocate_and_destruct_from_begin_to_before_end(node* const begin_p, node* const end_p) const noexcept
	{
		FE_ASSERT(begin_p != nullptr, "Assertion failed: front_p must not be nullptr.");
		FE_ASSERT(end_p != nullptr, "Assertion failed: back_p must not be nullptr.");

		for (node* it = begin_p; it != end_p;)
		{
			node* l_to_delete = it;
			it = it->m_next;

			l_to_delete->~node();
			m_allocator.deallocate(l_to_delete, 1);
		}
	}
	// this does not modify the m_size
	_FE_FORCE_INLINE_ void __deallocate_and_destruct_all(node* const first_p, node* const last_p) const noexcept
	{
		FE_ASSERT(first_p != nullptr, "Assertion failed: first_p must not be nullptr.");
		FE_ASSERT(last_p != nullptr, "Assertion failed: last_p must not be nullptr.");
		__deallocate_and_destruct_from_begin_to_before_end(first_p, last_p);
		last_p->~node();
		m_allocator.deallocate(last_p, 1);
	}
};

END_NAMESPACE
#endif
