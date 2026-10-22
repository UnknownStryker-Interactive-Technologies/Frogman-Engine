#ifndef _FE_CORE_FORWARD_LIST_HXX_
#define _FE_CORE_FORWARD_LIST_HXX_
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
#include <FE/iterator.hxx>
#include <FE/memory.hxx>
#include <FE/pair.hxx>

#include <concepts>
#include <functional> // std::less<>, std::equal_to<>
#include <iterator> // iterator tags and traits
#include <initializer_list>
#include <limits>
#include <memory>
#include <ranges> // std::ranges::input_range, std::from_range_t
#include <type_traits>
#include <utility>
#include <version> // __cpp_lib_containers_ranges




BEGIN_NAMESPACE(FE)

namespace internal
{
	// container-compatible-range<T>: an input range whose elements are convertible to T; see: https://en.cppreference.com/w/cpp/ranges/to.html
	template <class Range, typename T>
	concept __container_compatible_range = std::ranges::input_range<Range> && std::convertible_to<std::ranges::range_reference_t<Range>, T>;

	/*
	LegacyInputIterator, as the standard containers constrain their iterator-pair overloads: std::iterator_traits<Iterator>::iterator_category is at least input_iterator_tag.
	Unlike FE::has_iterator_category, this also accepts the iterators whose traits are supplied by a std::iterator_traits specialization (e.g. std::common_iterator),
	and it rejects the integers, so that forward_list(5, 3) keeps selecting (count, value).
	*/
	template <class Iterator>
	concept __legacy_input_iterator = requires { typename std::iterator_traits<Iterator>::iterator_category; } && std::derived_from<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>;
}


// eXceptionless assertive singly linked list
template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
class forward_list
{
	static_assert(std::is_same_v<T, typename Allocator::value_type>, "Static assertion failed: forward_list<T, Allocator>" " requires that Allocator's value_type match " "T");
	static_assert(std::is_object_v<T>, "Static assertion failed: the C++ Standard forbids containers of non-object types");

	class forward_list_iterator;

	/*
	Like the standard containers, the element is constructed and destroyed through std::allocator_traits, not by the node.
	e.g. std::pmr::polymorphic_allocator passes its memory resource down to the element (uses-allocator construction).
	The anonymous union keeps _value uninitialized until then.
	*/
	class node
	{
		friend class forward_list;
		friend class forward_list_iterator;

	public:
		union
		{
			T _value;
		};

	private:
		node* m_next;

	public:
		constexpr node() noexcept
			:	m_next(nullptr)
		{
			FE_ASSERT((FE::byte*)std::addressof(_value) < (FE::byte*)&m_next, "Assertion failed: this breaks the address alignment requirements of the T expected by many dependent classes.");
		}

		constexpr ~node() noexcept {} // _value is destroyed by std::allocator_traits before this.
	};

	/*
	A singly linked list has no node in front of the first node, so the before_begin() iterator cannot point to a node.
	Instead, it holds the address of the owner's m_front, and its m_iterator stays nullptr.
	m_owner_front is non-null if and only if the iterator is a before_begin() iterator.
	*/
	class forward_list_iterator
	{
		friend class forward_list;

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = node;
		using difference_type = var::ptrdiff;
		using pointer = node*;
		using reference = node&;
		using const_pointer = const node*;
		using const_reference = const node&;

	protected:
		pointer m_iterator;
		node* const* m_owner_front;

	public:
		constexpr forward_list_iterator() noexcept : m_iterator(), m_owner_front() {}
		constexpr forward_list_iterator(const_pointer const value_p) noexcept : m_iterator(const_cast<pointer>(value_p)), m_owner_front() {}
		constexpr forward_list_iterator(const_pointer const value_p, node* const* const owner_front_p) noexcept : m_iterator(const_cast<pointer>(value_p)), m_owner_front(owner_front_p) {}
		constexpr forward_list_iterator(const forward_list_iterator& other_p) noexcept : m_iterator(other_p.m_iterator), m_owner_front(other_p.m_owner_front) {}
		constexpr forward_list_iterator(forward_list_iterator&& other_p) noexcept : m_iterator(other_p.m_iterator), m_owner_front(other_p.m_owner_front) { other_p.m_iterator = nullptr; other_p.m_owner_front = nullptr; }
		constexpr ~forward_list_iterator() noexcept {}

		_FE_FORCE_INLINE_ constexpr boolean is_null() const noexcept { return (m_iterator == nullptr) && (m_owner_front == nullptr); }
		_FE_FORCE_INLINE_ constexpr boolean is_before_begin() const noexcept { return m_owner_front != nullptr; }

		_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
		{
			FE_NEGATIVE_ASSERT(m_owner_front != nullptr, "${%s@0}: Unable to dereference the before_begin() iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps));
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return *m_iterator;
		}
		_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
		{
			FE_NEGATIVE_ASSERT(m_owner_front != nullptr, "${%s@0}: Unable to access the before_begin() iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps));
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return m_iterator;
		}

		_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
		{
			FE_NEGATIVE_ASSERT(m_owner_front != nullptr, "${%s@0}: Unable to dereference the before_begin() iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps));
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return *m_iterator;
		}
		_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
		{
			FE_NEGATIVE_ASSERT(m_owner_front != nullptr, "${%s@0}: Unable to access the before_begin() iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps));
			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			return m_iterator;
		}


		_FE_FORCE_INLINE_ constexpr void operator++() noexcept
		{
			if (m_owner_front != nullptr) // before_begin() -> begin()
			{
				m_iterator = *m_owner_front;
				m_owner_front = nullptr;
				return;
			}

			FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
			m_iterator = m_iterator->m_next;
		}
		_FE_DEPRECATED_ constexpr void operator--() noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator -- .");
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator operator+(const difference_type pointer_offset_p) const noexcept
		{
			FE_ASSERT(pointer_offset_p >= 0, "Assertion failed: forward_list_iterator cannot move backward.");
			forward_list_iterator l_result = *this;
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				l_result.operator++();
			}
			return l_result;
		}
		_FE_FORCE_INLINE_ constexpr void operator+=(const difference_type pointer_offset_p) noexcept
		{
			FE_ASSERT(pointer_offset_p >= 0, "Assertion failed: forward_list_iterator cannot move backward.");
			for (difference_type i = 0; i < pointer_offset_p; ++i)
			{
				operator++();
			}
		}


		_FE_DEPRECATED_ constexpr forward_list_iterator operator-(const difference_type pointer_offset_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator - .");
			return *this;
		}
		_FE_DEPRECATED_ constexpr void operator-=(const difference_type pointer_offset_p) noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator -= .");
		}
		// operator-(iterator) is intentionally absent: the standard forward_list iterator has none. an O(n) difference would make the iterator model std::sized_sentinel_for,
		// which promises a constant-time distance, and std::ranges::next(it, n, bound) and std::ranges::distance() would silently degrade to O(n) per call. use std::distance().


		_FE_DEPRECATED_ constexpr const_reference operator[](const difference_type index_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the random access [] index operator.");
			return m_iterator[0];
		}
		_FE_DEPRECATED_ constexpr reference operator[](const difference_type index_p) noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the random access [] index operator.");
			return m_iterator[0];
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator& operator=(const forward_list_iterator& other_p) noexcept
		{
			m_iterator = other_p.m_iterator;
			m_owner_front = other_p.m_owner_front;
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr forward_list_iterator& operator=(forward_list_iterator&& rvalue_p) noexcept
		{
			m_iterator = rvalue_p.m_iterator;
			m_owner_front = rvalue_p.m_owner_front;
			return *this;
		}


		_FE_DEPRECATED_ constexpr boolean operator<(const forward_list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator < .");
			return m_iterator < other_p.m_iterator;
		}
		_FE_DEPRECATED_ constexpr boolean operator<=(const forward_list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator <= .");
			return m_iterator <= other_p.m_iterator;
		}

		_FE_DEPRECATED_ constexpr boolean operator>(const forward_list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator > .");
			return m_iterator > other_p.m_iterator;
		}
		_FE_DEPRECATED_ constexpr boolean operator>=(const forward_list_iterator& other_p) const noexcept
		{
			FE_EXIT_IF(true, ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "Assertion failed: forward_list_iterator does not support the operator >= .");
			return m_iterator >= other_p.m_iterator;
		}

		_FE_FORCE_INLINE_ constexpr boolean operator==(const forward_list_iterator& other_p) const noexcept
		{
			return (m_iterator == other_p.m_iterator) && (m_owner_front == other_p.m_owner_front);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator!=(const forward_list_iterator& other_p) const noexcept
		{
			return (m_iterator != other_p.m_iterator) || (m_owner_front != other_p.m_owner_front);
		}
	};

	template <class WrappedIterator>
	class forward_list_iterator_wrapper
	{
		friend class forward_list;
		friend class forward_list_iterator;

	public:
		using wrapped_iterator_type = WrappedIterator;
		using iterator_category = typename WrappedIterator::iterator_category;
		using value_type = T;
		using difference_type = typename WrappedIterator::difference_type;
		using pointer = std::conditional_t<FE::is_const_iterator<WrappedIterator>::value, const T*, T*>;
		using reference = std::conditional_t<FE::is_const_iterator<WrappedIterator>::value, const T&, T&>;
		using const_pointer = const T*;
		using const_reference = const T&;

	private:
		WrappedIterator m_wrapped;

	public:
		constexpr forward_list_iterator_wrapper() noexcept : m_wrapped() {}
		constexpr forward_list_iterator_wrapper(const WrappedIterator& value_p) noexcept : m_wrapped(value_p) {}
		constexpr forward_list_iterator_wrapper(WrappedIterator&& value_p) noexcept : m_wrapped( std::move(value_p) ) {}
		constexpr forward_list_iterator_wrapper(const forward_list_iterator_wrapper& other_p) noexcept : m_wrapped(other_p.m_wrapped) {}
		constexpr forward_list_iterator_wrapper(forward_list_iterator_wrapper&& other_p) noexcept : m_wrapped( std::move(other_p.m_wrapped) ) {}

		// iterator -> const_iterator; e.g. insert_after(before_begin(), value) on a non-const forward_list.
		template <class OtherWrappedIterator>
			requires (FE::is_const_iterator<WrappedIterator>::value == true) && (FE::is_const_iterator<OtherWrappedIterator>::value == false)
		constexpr forward_list_iterator_wrapper(const forward_list_iterator_wrapper<OtherWrappedIterator>& other_p) noexcept : m_wrapped( other_p.unwrap() ) {}

		constexpr ~forward_list_iterator_wrapper() noexcept = default;

		_FE_FORCE_INLINE_ constexpr const WrappedIterator unwrap() const noexcept { return m_wrapped; }
		_FE_FORCE_INLINE_ constexpr WrappedIterator unwrap() noexcept { return m_wrapped; }

		/*
		Like the standard iterators, the constness of the iterator object does not propagate to the element:
		iterator yields T& and const_iterator yields const T& even if the iterator object itself is const.
		This is required to model std::forward_iterator (std::indirectly_readable); the nodes are never const objects.
		*/
		_FE_FORCE_INLINE_ constexpr reference operator*() const noexcept
		{
			return const_cast<reference>(m_wrapped.operator*()._value);
		}
		_FE_FORCE_INLINE_ constexpr pointer operator->() const noexcept
		{
			return const_cast<pointer>(std::addressof(m_wrapped.operator->()->_value)); // T may overload the unary operator &.
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator++() noexcept
		{
			m_wrapped.operator++();
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper operator++(int) noexcept
		{
			forward_list_iterator_wrapper l_temporary = *this;
			m_wrapped.operator++();
			return l_temporary;
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator--() noexcept
		{
			m_wrapped.operator--();
			return *this;
		}
		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper operator--(int) noexcept
		{
			forward_list_iterator_wrapper l_temporary = *this;
			m_wrapped.operator--();
			return l_temporary;
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper operator+(const difference_type pointer_offset_p) const noexcept
		{
			return m_wrapped.operator+(pointer_offset_p);
		}
		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator+=(const difference_type pointer_offset_p) noexcept
		{
			m_wrapped.operator+=(pointer_offset_p);
			return *this;
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper operator-(const difference_type pointer_offset_p) const noexcept
		{
			return m_wrapped.operator-(pointer_offset_p);
		}

		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator-=(const difference_type pointer_offset_p) noexcept
		{
			m_wrapped.operator-=(pointer_offset_p);
			return *this;
		}


		_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
		{
			return m_wrapped.operator[](index_p)._value;
		}
		_FE_FORCE_INLINE_ constexpr reference operator[](const difference_type index_p) noexcept
		{
			return m_wrapped.operator[](index_p)._value;
		}


		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator=(typename WrappedIterator::const_pointer value_p) noexcept
		{
			m_wrapped.operator=(value_p);
			return *this;
		}

		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator=(const forward_list_iterator_wrapper& other_p) noexcept
		{
			m_wrapped.operator=(other_p.m_wrapped);
			return *this;
		}

		_FE_FORCE_INLINE_ constexpr forward_list_iterator_wrapper& operator=(forward_list_iterator_wrapper&& other_p) noexcept
		{
			if (this == std::addressof(other_p)) // self move-assignment must leave the iterator as it was, not nullify it.
			{
				return *this;
			}

			m_wrapped.operator=(other_p.m_wrapped);
			other_p.m_wrapped = nullptr;
			return *this;
		}


		_FE_FORCE_INLINE_ constexpr boolean operator<(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator<(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator<=(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator<=(other_p.m_wrapped);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator>(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator>(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator>=(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator>=(other_p.m_wrapped);
		}

		_FE_FORCE_INLINE_ constexpr boolean operator==(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator==(other_p.m_wrapped);
		}
		_FE_FORCE_INLINE_ constexpr boolean operator!=(const forward_list_iterator_wrapper& other_p) const noexcept
		{
			return m_wrapped.operator!=(other_p.m_wrapped);
		}

		// iterator == const_iterator and vice versa; C++20 does not synthesize the reversed operator== when the matching operator!= is declared.
		template <class OtherWrappedIterator>
		_FE_FORCE_INLINE_ constexpr boolean operator==(const forward_list_iterator_wrapper<OtherWrappedIterator>& other_p) const noexcept
		{
			return m_wrapped.operator==( other_p.unwrap() );
		}
		template <class OtherWrappedIterator>
		_FE_FORCE_INLINE_ constexpr boolean operator!=(const forward_list_iterator_wrapper<OtherWrappedIterator>& other_p) const noexcept
		{
			return m_wrapped.operator!=( other_p.unwrap() );
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
	using allocator_type = Allocator;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = forward_list_iterator_wrapper<FE::iterator<forward_list_iterator>>;
	using const_iterator = forward_list_iterator_wrapper<FE::const_iterator<forward_list_iterator>>;

private:
	using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
	using alloc_traits = std::allocator_traits<node_allocator_type>;

	_FE_NO_UNIQUE_ADDRESS_ mutable node_allocator_type m_allocator;
	node* m_front;
	size_type m_size;

public: // Member functions
	constexpr forward_list() noexcept // does not allocate
		:	m_allocator(allocator_type()),
			m_front(nullptr),
			m_size(0)
	{}

	explicit constexpr forward_list(const allocator_type& alloc_p) noexcept // does not allocate
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(0)
	{}

	explicit constexpr forward_list(const size_type count_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(count_p)
	{
		if (count_p == 0)
		{
			return;
		}

		m_front = __allocate_and_construct_nodes(count_p)._front; // value-initializes the elements.
	}

	constexpr forward_list(const size_type count_p, const T& value_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(count_p)
	{
		if (count_p == 0)
		{
			return;
		}

		m_front = __allocate_and_construct_nodes(count_p, value_p)._front;
	}

	template <class Iterator>
	constexpr forward_list(Iterator first_p, Iterator last_p, const allocator_type& alloc_p = allocator_type()) noexcept
		requires internal::__legacy_input_iterator<Iterator>
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(0)
	{
		node_chain l_chain = __allocate_and_construct_nodes_from(first_p, last_p);
		m_front = l_chain._front;
		m_size = l_chain._size;
	}

#ifdef __cpp_lib_containers_ranges
	template <internal::__container_compatible_range<T> Range>
	constexpr forward_list(std::from_range_t, Range&& range_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(0)
	{
		node_chain l_chain = __allocate_and_construct_nodes_from(std::ranges::begin(range_p), std::ranges::end(range_p));
		m_front = l_chain._front;
		m_size = l_chain._size;
	}
#endif

	// the allocator is obtained by std::allocator_traits<Allocator>::select_on_container_copy_construction(); e.g. std::pmr::polymorphic_allocator yields the default resource.
	constexpr forward_list(const forward_list& other_p) noexcept
		:	forward_list(other_p, std::allocator_traits<Allocator>::select_on_container_copy_construction(other_p.get_allocator()))
	{}

	constexpr forward_list(forward_list&& other_p) noexcept
		:	m_allocator(std::move(other_p.m_allocator)), // the allocator is obtained by move construction.
			m_front(other_p.m_front),
			m_size(other_p.m_size)
	{
		other_p.m_front = nullptr;
		other_p.m_size = 0;
	}

	constexpr forward_list(const forward_list& other_p, const std::type_identity_t<Allocator>& alloc_p) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(0)
	{
		node_chain l_chain = __allocate_and_construct_nodes_from(other_p.cbegin(), other_p.cend());
		FE_ASSERT(l_chain._size == other_p.m_size, "Assertion failed: the size of the copy must be equal to other_p.m_size.");
		m_front = l_chain._front;
		m_size = l_chain._size;
	}

	constexpr forward_list(forward_list&& other_p, const std::type_identity_t<Allocator>& alloc_p) noexcept
		:	m_allocator(alloc_p),
			m_front(nullptr),
			m_size(0)
	{
		if (m_allocator == other_p.m_allocator) // this allocator can deallocate the nodes of the other; steal them.
		{
			m_front = other_p.m_front;
			m_size = other_p.m_size;

			other_p.m_front = nullptr;
			other_p.m_size = 0;
			return;
		}

		// the allocators differ, move the elements one by one into the nodes allocated by this allocator.
		// other_p keeps its moved-from elements as libstdc++, libc++ and the MSVC STL do; the standard leaves it valid but unspecified.
		node_chain l_chain = __allocate_and_construct_nodes_from(std::make_move_iterator(other_p.begin()), std::make_move_iterator(other_p.end()));
		m_front = l_chain._front;
		m_size = l_chain._size;
	}

	constexpr forward_list(std::initializer_list<T> init_p, const allocator_type& alloc_p = allocator_type()) noexcept
		:	forward_list(init_p.begin(), init_p.end(), alloc_p)
	{}


	constexpr ~forward_list() noexcept
	{
		if (m_size == 0)
		{
			return;
		}
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when size is greater than 0.");
		__deallocate_and_destruct_chain(m_front);
	}


	constexpr forward_list& operator=(const forward_list& other_p) noexcept
	{
		if (this == &other_p) // prevent self-assignment
		{
			return *this;
		}

		if constexpr (alloc_traits::propagate_on_container_copy_assignment::value == true)
		{
			if (m_allocator != other_p.m_allocator) // the other's allocator cannot deallocate the current nodes.
			{
				clear();
			}
			m_allocator = other_p.m_allocator;
		}

		assign(other_p.cbegin(), other_p.cend()); // reuses the current nodes.
		return *this;
	}

	constexpr forward_list& operator=(forward_list&& other_p) noexcept
	{
		if (this == &other_p) // prevent self-assignment
		{
			return *this;
		}

		if constexpr (alloc_traits::propagate_on_container_move_assignment::value == true)
		{
			clear(); // deallocate the current resources
			m_allocator = std::move(other_p.m_allocator); // the allocator is replaced by a move assignment.
		}
		else
		{
			if (m_allocator != other_p.m_allocator) // this allocator cannot deallocate the nodes of the other, move-assign the elements one by one into the current nodes.
			{
				// other_p keeps its moved-from elements as libstdc++, libc++ and the MSVC STL do; the standard leaves it valid but unspecified.
				__assign_from(std::make_move_iterator(other_p.begin()), std::make_move_iterator(other_p.end()));
				return *this;
			}
			clear(); // deallocate the current resources
		}

		// transfer the ownership
		m_front = other_p.m_front;
		m_size = other_p.m_size;

		// nullify the other list
		other_p.m_front = nullptr;
		other_p.m_size = 0;
		return *this;
	}

	constexpr forward_list& operator=(std::initializer_list<value_type> ilist_p) noexcept
	{
		__assign_from(ilist_p.begin(), ilist_p.end());
		return *this;
	}


	constexpr void assign(const size_type count_p, const T& value_p) noexcept
	{
		node** l_link = &m_front;
		size_type l_assigned = 0;
		for (; (*l_link != nullptr) && (l_assigned < count_p); ++l_assigned) // reuse the current nodes.
		{
			(*l_link)->_value = value_p;
			l_link = &((*l_link)->m_next);
		}

		if (*l_link != nullptr) // this list is longer than the request, trim the remainder.
		{
			__deallocate_and_destruct_chain(*l_link);
			*l_link = nullptr;
		}
		else if (l_assigned < count_p) // this list is shorter than the request, append new nodes.
		{
			*l_link = __allocate_and_construct_nodes(count_p - l_assigned, value_p)._front;
		}

		m_size = count_p;
	}

	template <class Iterator>
	_FE_FORCE_INLINE_ constexpr void assign(Iterator first_p, Iterator last_p) noexcept
		requires internal::__legacy_input_iterator<Iterator>
	{
		__assign_from(first_p, last_p);
	}

	_FE_FORCE_INLINE_ constexpr void assign(std::initializer_list<T> ilist_p) noexcept
	{
		__assign_from(ilist_p.begin(), ilist_p.end());
	}


	template <internal::__container_compatible_range<T> Range>
	_FE_FORCE_INLINE_ constexpr void assign_range(Range&& range_p) noexcept
	{
		__assign_from(std::ranges::begin(range_p), std::ranges::end(range_p));
	}


	_FE_FORCE_INLINE_ constexpr allocator_type get_allocator() const noexcept { return allocator_type(m_allocator); }



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



	// Iterators
	_FE_FORCE_INLINE_ constexpr iterator before_begin() noexcept { return iterator{ typename iterator::wrapped_iterator_type( forward_list_iterator(nullptr, &m_front) ) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator before_begin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type( forward_list_iterator(nullptr, &m_front) ) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator cbefore_begin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type( forward_list_iterator(nullptr, &m_front) ) }; }


	_FE_FORCE_INLINE_ constexpr iterator begin() noexcept { return iterator{ typename iterator::wrapped_iterator_type(m_front) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator begin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(m_front) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator cbegin() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(m_front) }; }


	_FE_FORCE_INLINE_ constexpr iterator end() noexcept { return iterator{ typename iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator end() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(nullptr) }; }
	_FE_FORCE_INLINE_ constexpr const_iterator cend() const noexcept { return const_iterator{ typename const_iterator::wrapped_iterator_type(nullptr) }; }



	// Capacity
	_FE_FORCE_INLINE_ constexpr boolean is_empty() const noexcept { return m_size == 0; }
	_FE_FORCE_INLINE_ constexpr size_type size() const noexcept { return m_size; } // not in std::forward_list; O(1) here.

	// at most std::numeric_limits<difference_type>::max(); see: https://en.cppreference.com/w/cpp/container/forward_list/max_size.html
	_FE_FORCE_INLINE_ constexpr size_type max_size() const noexcept
	{
		const size_type l_allocator_limit = alloc_traits::max_size(m_allocator);
		const size_type l_difference_limit = static_cast<size_type>( (std::numeric_limits<difference_type>::max)() );
		return (l_allocator_limit < l_difference_limit) ? l_allocator_limit : l_difference_limit;
	}



	// Modifiers
	constexpr void clear() noexcept
	{
		if (m_size == 0) // already empty, do nothing.
		{
			return;
		}
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when size is greater than 0.");
		__deallocate_and_destruct_chain(m_front);
		m_front = nullptr;
		m_size = 0;
	}


	_FE_FORCE_INLINE_ constexpr iterator insert_after(const_iterator pos_p, const T& value_p) noexcept
	{
		return emplace_after(pos_p, value_p);
	}

	_FE_FORCE_INLINE_ constexpr iterator insert_after(const_iterator pos_p, T&& value_p) noexcept
	{
		return emplace_after(pos_p, std::move(value_p));
	}

	constexpr iterator insert_after(const_iterator pos_p, const size_type count_p, const T& value_p) noexcept
	{
		node** const l_link = __get_next_link(pos_p);
		if (count_p == 0) // nothing to insert, return the position as is.
		{
			return FE::iterator_cast<iterator>(pos_p);
		}

		node_chain l_chain = __allocate_and_construct_nodes(count_p, value_p);
		l_chain._back->m_next = *l_link;
		*l_link = l_chain._front; // link the new nodes after pos_p
		m_size += l_chain._size; // increase the size
		return FE::iterator_cast<iterator>(l_chain._back); // return the last inserted element.
	}

	template <class Iterator>
	_FE_FORCE_INLINE_ constexpr iterator insert_after(const_iterator pos_p, Iterator first_p, Iterator last_p) noexcept
		requires internal::__legacy_input_iterator<Iterator>
	{
		return __insert_after_from(pos_p, first_p, last_p);
	}

	_FE_FORCE_INLINE_ constexpr iterator insert_after(const_iterator pos_p, std::initializer_list<T> ilist_p) noexcept
	{
		return __insert_after_from(pos_p, ilist_p.begin(), ilist_p.end());
	}


	template <internal::__container_compatible_range<T> Range>
	_FE_FORCE_INLINE_ constexpr iterator insert_range_after(const_iterator pos_p, Range&& range_p) noexcept
	{
		return __insert_after_from(pos_p, std::ranges::begin(range_p), std::ranges::end(range_p));
	}


	template <typename... Arguments>
	constexpr iterator emplace_after(const_iterator pos_p, Arguments&&... arguments_p) noexcept
	{
		node** const l_link = __get_next_link(pos_p);
		node* const l_new = __allocate_and_construct_node( std::forward<Arguments>(arguments_p)... );
		l_new->m_next = *l_link;
		*l_link = l_new; // link the new node after pos_p
		++m_size; // increase the size
		return FE::iterator_cast<iterator>(l_new);
	}


	constexpr iterator erase_after(const_iterator pos_p) noexcept
	{
		node** const l_link = __get_next_link(pos_p);
		FE_ASSERT(*l_link != nullptr, "Assertion failed: there is no element after pos_p to erase.");

		node* const l_to_erase = *l_link;
		*l_link = l_to_erase->m_next; // unlink the node
		__deallocate_and_destruct_node(l_to_erase);
		--m_size; // decrease the size
		return FE::iterator_cast<iterator>(*l_link); // return the element that followed the erased one, or end().
	}

	constexpr iterator erase_after(const_iterator first_p, const_iterator last_p) noexcept
	{
		if (first_p == last_p) // nothing to erase, return last_p as is.
		{
			return FE::iterator_cast<iterator>(last_p);
		}

		node** const l_link = __get_next_link(first_p);
		node* const l_last = __get_node(last_p);

		for (node* it = *l_link; it != l_last;) // erase (first_p, last_p)
		{
			FE_ASSERT(it != nullptr, "Assertion failed: last_p is not reachable from first_p.");
			node* const l_to_erase = it;
			it = it->m_next;

			__deallocate_and_destruct_node(l_to_erase);
			--m_size; // decrease the size
		}
		*l_link = l_last; // relink
		return FE::iterator_cast<iterator>(last_p);
	}


	_FE_FORCE_INLINE_ constexpr void push_front(const T& value_p) noexcept
	{
		emplace_front(value_p);
	}

	_FE_FORCE_INLINE_ constexpr void push_front(T&& value_p) noexcept
	{
		emplace_front(std::move(value_p));
	}


	template <typename... Arguments>
	constexpr reference emplace_front(Arguments&&... arguments_p) noexcept
	{
		node* const l_new = __allocate_and_construct_node( std::forward<Arguments>(arguments_p)... );
		l_new->m_next = m_front;
		m_front = l_new; // update the front pointer
		++m_size; // increase the size
		return l_new->_value;
	}


	template <internal::__container_compatible_range<T> Range>
	_FE_FORCE_INLINE_ constexpr void prepend_range(Range&& range_p) noexcept
	{
		__insert_after_from(cbefore_begin(), std::ranges::begin(range_p), std::ranges::end(range_p));
	}


	_FE_FORCE_INLINE_ constexpr void pop_front() noexcept
	{
		FE_ASSERT(m_front != nullptr, "Assertion failed: m_front must not be nullptr when popping front.");
		node* const l_to_erase = m_front;
		m_front = l_to_erase->m_next; // update the front pointer
		__deallocate_and_destruct_node(l_to_erase);
		--m_size; // decrease the size
	}


	constexpr void resize(const size_type count_p) noexcept
	{
		if (m_size == count_p) // sizes are equal, do nothing.
		{
			return;
		}

		// different sizes, modify the list.
		if (m_size > count_p) // this list is larger than the request, trim the tail of this list.
		{
			node** const l_link = __get_link_at(count_p);
			__deallocate_and_destruct_chain(*l_link);
			*l_link = nullptr;
		}
		else if (m_size < count_p) // this list is smaller than the request, append new nodes.
		{
			node** const l_link = __get_link_at(m_size);
			FE_ASSERT(*l_link == nullptr, "Assertion failed: the last node's m_next must be nullptr before appending new nodes.");
			*l_link = __allocate_and_construct_nodes(count_p - m_size)._front; // value-initializes the new elements.
		}

		m_size = count_p;
	}

	constexpr void resize(const size_type count_p, const T& value_p) noexcept
	{
		if (m_size == count_p) // sizes are equal, do nothing.
		{
			return;
		}

		// different sizes, modify the list.
		if (m_size > count_p) // this list is larger than the request, trim the tail of this list.
		{
			node** const l_link = __get_link_at(count_p);
			__deallocate_and_destruct_chain(*l_link);
			*l_link = nullptr;
		}
		else if (m_size < count_p) // this list is smaller than the request, append new nodes.
		{
			node** const l_link = __get_link_at(m_size);
			FE_ASSERT(*l_link == nullptr, "Assertion failed: the last node's m_next must be nullptr before appending new nodes.");
			*l_link = __allocate_and_construct_nodes(count_p - m_size, value_p)._front;
		}

		m_size = count_p;
	}


	_FE_FORCE_INLINE_ constexpr void swap(forward_list& other_p) noexcept
	{
		if constexpr (alloc_traits::propagate_on_container_swap::value == true)
		{
			using std::swap; // the allocators are exchanged by an unqualified call, so that the allocator's own swap() found by ADL wins.
			swap(m_allocator, other_p.m_allocator);
		}
		else
		{
			FE_ASSERT(m_allocator == other_p.m_allocator, "Assertion failed: the allocators must be equal to swap(); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/swap.html");
		}
		std::swap(m_front, other_p.m_front);
		std::swap(m_size, other_p.m_size);
	}



	// Operations
	template <class Compare>
	constexpr void merge(forward_list& other_p, Compare compare_p) noexcept
	{
		if (this == &other_p) // has no effect; see: https://en.cppreference.com/w/cpp/container/forward_list/merge.html
		{
			return;
		}
		FE_ASSERT(m_allocator == other_p.m_allocator, "Assertion failed: the allocators must be equal to merge(); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/merge.html");

		if (other_p.is_empty() == true) // nothing to merge.
		{
			return;
		}

		FE::pair<node*, node*> l_merged = __merge_chains(m_front, other_p.m_front, compare_p);
		m_front = l_merged._first; // update the front pointer
		m_size += other_p.m_size;

		// nullify the other list
		other_p.m_front = nullptr;
		other_p.m_size = 0;
	}

	template <class Compare>
	_FE_FORCE_INLINE_ constexpr void merge(forward_list&& other_p, Compare compare_p) noexcept
	{
		merge(other_p, std::move(compare_p)); // Compare does not have to be copyable.
	}

	_FE_FORCE_INLINE_ constexpr void merge(forward_list& other_p) noexcept
	{
		merge(other_p, std::less<>());
	}

	_FE_FORCE_INLINE_ constexpr void merge(forward_list&& other_p) noexcept
	{
		merge(other_p, std::less<>());
	}


	constexpr void splice_after(const_iterator pos_p, forward_list& other_p) noexcept
	{
		FE_ASSERT(this != &other_p, "Assertion failed: cannot splice_after() the entire list into itself.");
		FE_ASSERT(m_allocator == other_p.m_allocator, "Assertion failed: the allocators must be equal to splice_after(); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");

#ifdef _DEBUG_
		FE_ASSERT(__has_position(pos_p) == true, "Assertion failed: pos_p does not belong to *this; please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
#endif

		node** const l_link = __get_next_link(pos_p);
		if (other_p.is_empty() == true) // nothing to splice.
		{
			return;
		}

		node* const l_other_back = *(other_p.__get_link_at(other_p.m_size - 1));
		l_other_back->m_next = *l_link;
		*l_link = other_p.m_front; // link the entire other list after pos_p
		m_size += other_p.m_size;

		// nullify the other list
		other_p.m_front = nullptr;
		other_p.m_size = 0;
	}

	_FE_FORCE_INLINE_ constexpr void splice_after(const_iterator pos_p, forward_list&& other_p) noexcept
	{
		splice_after(pos_p, other_p);
	}

	// moves the element after it_p from other_p to after pos_p. other_p may be *this.
	constexpr void splice_after(const_iterator pos_p, forward_list& other_p, const_iterator it_p) noexcept
	{
		FE_ASSERT(m_allocator == other_p.m_allocator, "Assertion failed: the allocators must be equal to splice_after(); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");

#ifdef _DEBUG_
		FE_ASSERT(__has_position(pos_p) == true, "Assertion failed: pos_p does not belong to *this; please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
		FE_ASSERT(other_p.__has_position(it_p) == true, "Assertion failed: it_p does not belong to other_p; please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
#endif

		node** const l_source_link = other_p.__get_next_link(it_p);
		FE_ASSERT(*l_source_link != nullptr, "Assertion failed: there is no element after it_p to splice.");
		node** const l_destination_link = __get_next_link(pos_p);

		if ( ( l_destination_link == l_source_link ) ||
			 ( l_destination_link == &((*l_source_link)->m_next) ) ) _FE_UNLIKELY_
		{
			return; // pos_p == it_p or pos_p == ++it_p; has no effect. see: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html
		}

		node* const l_input = *l_source_link;
		*l_source_link = l_input->m_next; // unlink the node from the other

		l_input->m_next = *l_destination_link;
		*l_destination_link = l_input; // link the node after pos_p

		if (this != &other_p)
		{
			--(other_p.m_size);
			++m_size;
		}
	}

	_FE_FORCE_INLINE_ constexpr void splice_after(const_iterator pos_p, forward_list&& other_p, const_iterator it_p) noexcept
	{
		splice_after(pos_p, other_p, it_p);
	}

	// moves the elements in (first_p, last_p) from other_p to after pos_p. other_p may be *this, if pos_p is not in (first_p, last_p).
	constexpr void splice_after(const_iterator pos_p, forward_list& other_p, const_iterator first_p, const_iterator last_p) noexcept
	{
		FE_ASSERT(m_allocator == other_p.m_allocator, "Assertion failed: the allocators must be equal to splice_after(); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");

#ifdef _DEBUG_
		FE_ASSERT(__has_position(pos_p) == true, "Assertion failed: pos_p does not belong to *this; please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
		FE_ASSERT(other_p.__has_position(first_p) == true, "Assertion failed: first_p does not belong to other_p; please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
#endif

		if (first_p == last_p) // nothing to splice.
		{
			return;
		}

		node** const l_source_link = other_p.__get_next_link(first_p);
		node* const l_last = __get_node(last_p);
		if (*l_source_link == l_last) // the range is empty, nothing to splice.
		{
			return;
		}

		node** const l_destination_link = __get_next_link(pos_p);
		_FE_MAYBE_UNUSED_ node* const l_position = pos_p.unwrap().is_before_begin() ? nullptr : __get_node(pos_p); // for assertions only

		node* const l_range_front = *l_source_link;
		node* l_range_back = l_range_front;
		size_type l_count = 1;
		for (; l_range_back->m_next != l_last; l_range_back = l_range_back->m_next) // find the back of the range.
		{
			FE_ASSERT(l_range_back != l_position, "Assertion failed: pos_p must not be in (first_p, last_p); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");
			FE_ASSERT(l_range_back->m_next != nullptr, "Assertion failed: last_p is not reachable from first_p.");
			++l_count;
		}
		FE_ASSERT(l_range_back != l_position, "Assertion failed: pos_p must not be in (first_p, last_p); please refer to: https://en.cppreference.com/w/cpp/container/forward_list/splice_after.html");

		*l_source_link = l_last; // unlink the range from the other

		l_range_back->m_next = *l_destination_link;
		*l_destination_link = l_range_front; // link the range after pos_p

		if (this != &other_p)
		{
			other_p.m_size -= l_count;
			m_size += l_count;
		}
	}

	_FE_FORCE_INLINE_ constexpr void splice_after(const_iterator pos_p, forward_list&& other_p, const_iterator first_p, const_iterator last_p) noexcept
	{
		splice_after(pos_p, other_p, first_p, last_p);
	}


	// returns the number of the removed elements.
	constexpr size_type remove(const T& value_p) noexcept
	{
		node* l_deferred = nullptr; // value_p may refer to an element of this list, so the node that holds value_p is destroyed last.
		size_type l_removed = 0;

		node** l_link = &m_front;
		while (*l_link != nullptr)
		{
			node* const l_node = *l_link;
			if (l_node->_value == value_p)
			{
				*l_link = l_node->m_next; // unlink the node
				if (std::addressof(l_node->_value) == std::addressof(value_p)) // T may overload the unary operator &.
				{
					l_deferred = l_node;
				}
				else
				{
					__deallocate_and_destruct_node(l_node);
				}
				++l_removed;
				continue;
			}
			l_link = &(l_node->m_next);
		}

		if (l_deferred != nullptr)
		{
			__deallocate_and_destruct_node(l_deferred);
		}

		m_size -= l_removed; // decrease the size
		return l_removed;
	}

	// returns the number of the removed elements.
	template <class UnaryPredicate>
	constexpr size_type remove_if(UnaryPredicate predicate_p) noexcept
	{
		size_type l_removed = 0;

		node** l_link = &m_front;
		while (*l_link != nullptr)
		{
			node* const l_node = *l_link;
			if (static_cast<bool>(predicate_p(l_node->_value))) // the result only has to be contextually convertible to bool, so it is not compared with true.
			{
				*l_link = l_node->m_next; // unlink the node
				__deallocate_and_destruct_node(l_node);
				++l_removed;
				continue;
			}
			l_link = &(l_node->m_next);
		}

		m_size -= l_removed; // decrease the size
		return l_removed;
	}


	constexpr void reverse() noexcept
	{
		node* l_previous = nullptr;
		node* l_current = m_front;
		while (l_current != nullptr)
		{
			node* const l_next = l_current->m_next;
			l_current->m_next = l_previous;
			l_previous = l_current;
			l_current = l_next;
		}
		m_front = l_previous; // update the front pointer
	}


	// removes all but the first element from every consecutive group of equivalent elements. returns the number of the removed elements.
	template <class BinaryPredicate>
	constexpr size_type unique(BinaryPredicate predicate_p) noexcept
	{
		if (m_size < 2)
		{
			return 0; // nothing to remove
		}

		size_type l_removed = 0;
		node* l_current = m_front;
		while (l_current->m_next != nullptr)
		{
			node* const l_next = l_current->m_next;
			if (static_cast<bool>(predicate_p(l_current->_value, l_next->_value))) // the result only has to be contextually convertible to bool.
			{
				l_current->m_next = l_next->m_next; // unlink the duplicate
				__deallocate_and_destruct_node(l_next);
				++l_removed;
				continue;
			}
			l_current = l_next;
		}

		m_size -= l_removed; // decrease the size
		return l_removed;
	}

	_FE_FORCE_INLINE_ constexpr size_type unique() noexcept
	{
		return unique(std::equal_to<>());
	}


	// stable bottom-up merge sort; O(n log n) comparisons, no allocation, no recursion.
	template <class Compare>
	constexpr void sort(Compare compare_p) noexcept
	{
		if (m_size < 2)
		{
			return; // nothing to sort
		}

		for (size_type l_run_length = 1; l_run_length < m_size; l_run_length *= 2) // merge the runs of l_run_length into the runs of l_run_length * 2.
		{
			node** l_tail_link = &m_front;
			node* l_remainder = m_front;
			while (l_remainder != nullptr)
			{
				node* const l_left = l_remainder;
				node* const l_right = __split_after(l_left, l_run_length);
				l_remainder = __split_after(l_right, l_run_length);

				FE::pair<node*, node*> l_merged = __merge_chains(l_left, l_right, compare_p);
				*l_tail_link = l_merged._first;
				l_tail_link = &(l_merged._second->m_next);
			}
		}
	}

	_FE_FORCE_INLINE_ constexpr void sort() noexcept
	{
		sort(std::less<>());
	}


private:
	// a detached chain of nodes; _back->m_next is always nullptr.
	struct node_chain
	{
		node* _front;
		node* _back;
		size_type _size;
	};

private: // internal modifiers shared by the iterator-pair and the range overloads; unlike the helpers below, these modify the member variables.
	template <class Iterator, class Sentinel>
	constexpr void __assign_from(Iterator first_p, Sentinel last_p) noexcept
	{
		node** l_link = &m_front;
		size_type l_assigned = 0;
		for (; (*l_link != nullptr) && (first_p != last_p); ++first_p) // reuse the current nodes.
		{
			(*l_link)->_value = *first_p;
			l_link = &((*l_link)->m_next);
			++l_assigned;
		}

		if (*l_link != nullptr) // this list is longer than the range, trim the remainder.
		{
			__deallocate_and_destruct_chain(*l_link);
			*l_link = nullptr;
		}
		else if (first_p != last_p) // this list is shorter than the range, append new nodes.
		{
			node_chain l_chain = __allocate_and_construct_nodes_from(std::move(first_p), std::move(last_p));
			*l_link = l_chain._front;
			l_assigned += l_chain._size;
		}

		m_size = l_assigned;
	}

	// returns the iterator to the last inserted element, or pos_p if the range is empty.
	template <class Iterator, class Sentinel>
	constexpr iterator __insert_after_from(const_iterator pos_p, Iterator first_p, Sentinel last_p) noexcept
	{
		node** const l_link = __get_next_link(pos_p);
		node_chain l_chain = __allocate_and_construct_nodes_from(std::move(first_p), std::move(last_p));
		if (l_chain._size == 0) // nothing to insert, return the position as is.
		{
			return FE::iterator_cast<iterator>(pos_p);
		}

		l_chain._back->m_next = *l_link;
		*l_link = l_chain._front; // link the new nodes after pos_p
		m_size += l_chain._size; // increase the size
		return FE::iterator_cast<iterator>(l_chain._back); // return the last inserted element.
	}

private: // these are private internal functions meant to operate on the given arguments; these never directly modify the member variables, except for the m_allocator's state.
	// the element is constructed by std::allocator_traits::construct() as the standard containers do.
	template <typename... Arguments>
	_FE_FORCE_INLINE_ node* __allocate_and_construct_node(Arguments&&... arguments_p) const noexcept
	{
		// the allocator's pointer may be a fancy pointer (a class type); the list itself links the nodes with the raw pointers.
		node* const l_new = std::to_address(alloc_traits::allocate(m_allocator, 1));
		FE_ASSERT(l_new != nullptr, "Assertion failed: allocation returned nullptr.");
		::new(l_new) node();
		alloc_traits::construct(m_allocator, std::addressof(l_new->_value), std::forward<Arguments>(arguments_p)...);
		return l_new;
	}

	// the element is destroyed by std::allocator_traits::destroy() as the standard containers do.
	_FE_FORCE_INLINE_ void __deallocate_and_destruct_node(node* const node_p) const noexcept
	{
		FE_ASSERT(node_p != nullptr, "Assertion failed: node_p must not be nullptr.");
		const typename alloc_traits::pointer l_allocated = std::pointer_traits<typename alloc_traits::pointer>::pointer_to(*node_p); // converts the raw pointer back to the allocator's pointer.
		alloc_traits::destroy(m_allocator, std::addressof(node_p->_value));
		node_p->~node();
		alloc_traits::deallocate(m_allocator, l_allocated, 1);
	}

	// constructs count_p nodes with the same arguments; no arguments value-initialize the elements.
	template <typename... Arguments>
	node_chain __allocate_and_construct_nodes(const size_type count_p, const Arguments&... arguments_p) const noexcept
	{
		node_chain l_chain{ nullptr, nullptr, 0 };
		node** l_link = &(l_chain._front);
		for (size_type i = 0; i < count_p; ++i)
		{
			node* const l_new = __allocate_and_construct_node(arguments_p...);
			*l_link = l_new;
			l_link = &(l_new->m_next);
			l_chain._back = l_new;
		}
		l_chain._size = count_p;
		return l_chain;
	}

	// constructs the nodes from [first_p, last_p) in a single pass; last_p may be a sentinel of a different type.
	template <class Iterator, class Sentinel>
	node_chain __allocate_and_construct_nodes_from(Iterator first_p, Sentinel last_p) const noexcept
	{
		node_chain l_chain{ nullptr, nullptr, 0 };
		node** l_link = &(l_chain._front);
		for (; first_p != last_p; ++first_p)
		{
			node* const l_new = __allocate_and_construct_node(*first_p);
			*l_link = l_new;
			l_link = &(l_new->m_next);
			l_chain._back = l_new;
			++(l_chain._size);
		}
		return l_chain;
	}

	// this does not modify the m_size
	void __deallocate_and_destruct_chain(node* const front_p) const noexcept
	{
		FE_ASSERT(front_p != nullptr, "Assertion failed: front_p must not be nullptr.");

		for (node* it = front_p; it != nullptr;)
		{
			node* const l_to_delete = it;
			it = it->m_next;

			__deallocate_and_destruct_node(l_to_delete);
		}
	}

	// returns the address of the link that follows pos_p: this list's m_front if pos_p is before_begin(), otherwise pos_p's m_next.
	node** __get_next_link(const const_iterator& pos_p) const noexcept
	{
		const forward_list_iterator l_position = pos_p.unwrap();
		if (l_position.m_owner_front != nullptr) // pos_p is before_begin()
		{
			FE_ASSERT(l_position.m_owner_front == &m_front, "Assertion failed: cannot operate on an alien before_begin() iterator.");
			return const_cast<node**>(&m_front);
		}

		FE_ASSERT(l_position.m_iterator != nullptr, "Assertion failed: the position must not be end().");
		return &(l_position.m_iterator->m_next);
	}

	// returns the node that pos_p refers to; nullptr if pos_p is end().
	_FE_FORCE_INLINE_ static node* __get_node(const const_iterator& pos_p) noexcept
	{
		const forward_list_iterator l_position = pos_p.unwrap();
		FE_ASSERT(l_position.m_owner_front == nullptr, "Assertion failed: before_begin() does not refer to any node.");
		return l_position.m_iterator;
	}

	// returns the address of the link that points to the index_p-th node: m_front if index_p is 0, the last node's m_next if index_p is m_size.
	node** __get_link_at(const size_type index_p) const noexcept
	{
		FE_ASSERT(index_p <= m_size, "Assertion failed: index_p must not be greater than the size.");

		node* const* l_link = &m_front;
		for (size_type i = 0; i < index_p; ++i)
		{
			l_link = &((*l_link)->m_next);
		}
		return const_cast<node**>(l_link);
	}

	// detaches the chain after its count_p-th node and returns the front of the detached remainder; returns nullptr if the chain is not longer than count_p.
	static node* __split_after(node* const front_p, const size_type count_p) noexcept
	{
		FE_ASSERT(count_p > 0, "Assertion failed: count must be greater than 0");

		node* l_back = front_p;
		for (size_type i = 1; (i < count_p) && (l_back != nullptr); ++i)
		{
			l_back = l_back->m_next;
		}

		if (l_back == nullptr)
		{
			return nullptr;
		}

		node* const l_remainder = l_back->m_next;
		l_back->m_next = nullptr; // detach
		return l_remainder;
	}

	// merges two sorted chains into one sorted chain and returns its front and back. on ties, the elements of left_p precede the elements of right_p.
	template <class Compare>
	static FE::pair<node*, node*> __merge_chains(node* left_p, node* right_p, Compare& compare_p) noexcept
	{
		node* l_front = nullptr;
		node* l_back = nullptr;
		node** l_link = &l_front;

		while ((left_p != nullptr) && (right_p != nullptr))
		{
			if (static_cast<bool>(compare_p(right_p->_value, left_p->_value))) // the result only has to be contextually convertible to bool.
			{
				l_back = right_p;
				right_p = right_p->m_next;
			}
			else
			{
				l_back = left_p;
				left_p = left_p->m_next;
			}
			*l_link = l_back;
			l_link = &(l_back->m_next);
		}

		node* l_rest = (left_p != nullptr) ? left_p : right_p;
		*l_link = l_rest; // link the rest
		for (; l_rest != nullptr; l_rest = l_rest->m_next) // find the back
		{
			l_back = l_rest;
		}
		return FE::pair<node*, node*>{ l_front, l_back };
	}

#ifdef _DEBUG_
	// walks the entire list to check whether pos_p belongs to *this; for debugging only.
	boolean __has_position(const const_iterator& pos_p) const noexcept
	{
		const forward_list_iterator l_position = pos_p.unwrap();
		if (l_position.m_owner_front != nullptr) // pos_p is before_begin()
		{
			return l_position.m_owner_front == &m_front;
		}

		for (node* it = m_front; it != nullptr; it = it->m_next)
		{
			if (it == l_position.m_iterator)
			{
				return true;
			}
		}
		return l_position.m_iterator == nullptr; // end() belongs to every list.
	}
#endif
};

END_NAMESPACE

// eXceptionless Template Library
namespace xtl
{
	// eXceptionless assertive singly linked list
	template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
	using forward_list = FE::forward_list<T, Allocator>;
}

#endif
