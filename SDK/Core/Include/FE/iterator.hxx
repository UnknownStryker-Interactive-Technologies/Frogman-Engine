#ifndef _FE_CORE_ITERATOR_HXX_
#define _FE_CORE_ITERATOR_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/type_traits.hxx>




BEGIN_NAMESPACE(FE)


/*
The FE::iterator<Implementation> class template provides a type-safe iterator that extends a specified implementation
enforcing non-null assertions and supporting various iterator operations such as dereferencing, incrementing, and arithmetic.
*/
template <class Implementation>
class iterator final : public Implementation
{
	static_assert(std::is_class<Implementation>::value, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::iterator_category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	constexpr iterator() noexcept : base_type() {}
	constexpr iterator(const pointer value_p) noexcept : base_type(value_p) {}
	constexpr iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	constexpr iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	constexpr ~iterator() noexcept = default;
	

	_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
	{
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
	{
		return Implementation::operator->();
	}

	_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
	{
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
	{
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ constexpr iterator& operator++() noexcept
	{
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr iterator operator++(int) noexcept
	{
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr iterator& operator--() noexcept
	{
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr iterator& operator--(int) noexcept
	{
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ constexpr iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator-(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ constexpr iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr difference_type operator-(const base_type& other_p) const noexcept
	{
		return Implementation::operator-(other_p);
	}


	_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
	{
		return Implementation::operator[](index_p);
	}
	_FE_FORCE_INLINE_ constexpr reference operator[](const difference_type index_p) noexcept
	{
		return Implementation::operator[](index_p);
	}


	_FE_FORCE_INLINE_ constexpr iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class reverse_iterator final : public Implementation
{
	static_assert(std::is_class<Implementation>::value, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::iterator_category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	constexpr reverse_iterator() noexcept : base_type() {}
	constexpr reverse_iterator(const pointer value_p) noexcept : base_type(value_p) {}
	constexpr reverse_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	constexpr reverse_iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	constexpr ~reverse_iterator() noexcept = default;


	_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
	{
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
	{
		return Implementation::operator->();
	}

	_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
	{
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
	{
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator++() noexcept
	{
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator operator++(int) noexcept
	{
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator--() noexcept
	{
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator--(int) noexcept
	{
		reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator-(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr difference_type operator-(const base_type& other_p) const noexcept
	{
		return other_p.operator-(*this);
	}


	_FE_FORCE_INLINE_ constexpr reference operator[](const difference_type index_p) noexcept
	{
		return *(operator+(index_p));
	}
	_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
	{
		return *(operator+(index_p));
	}


	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class const_iterator final : public Implementation
{
	static_assert(std::is_class<Implementation>::value, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::iterator_category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	const_iterator() noexcept : base_type() {}
	const_iterator(const_pointer const value_p) noexcept : base_type(value_p) {}
	const_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	const_iterator(base_type&& other_p) noexcept : base_type(other_p) {}
	~const_iterator() noexcept {}


	_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
	{
		return Implementation::operator*();
	}

	_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
	{
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ constexpr const_iterator& operator++() noexcept
	{
		Implementation::operator++();
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr const_iterator operator++(int) noexcept
	{
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr const_iterator& operator--() noexcept
	{
		Implementation::operator--();
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr const_iterator& operator--(int) noexcept
	{
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr const_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator+(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ constexpr const_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr const_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator-(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ constexpr const_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr difference_type operator-(const base_type& other_p) const noexcept
	{
		return Implementation::operator-(other_p);
	}


	_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
	{
		return Implementation::operator[](index_p);
	}


	_FE_FORCE_INLINE_ constexpr const_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr const_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FE_FORCE_INLINE_ constexpr const_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class const_reverse_iterator final : public Implementation
{
	static_assert(std::is_class<Implementation>::value, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::iterator_category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	const_reverse_iterator() noexcept : base_type() {}
	const_reverse_iterator(const_pointer const value_p) noexcept : base_type(value_p) {}
	const_reverse_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	const_reverse_iterator(base_type&& other_p) noexcept : base_type(other_p) {}
	~const_reverse_iterator() noexcept {}


	_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
	{
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
	{
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator++() noexcept
	{
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator operator++(int) noexcept
	{
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator--() noexcept
	{
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator--(int) noexcept
	{
		const_reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ constexpr const_reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator-(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr const_reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr difference_type operator-(const base_type& other_p) const noexcept
	{
		return other_p.operator-(*this);
	}


	_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
	{
		return *(operator+(index_p));
	}


	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr const_reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ constexpr boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ constexpr boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};



/*
The contiguous_iterator class template in the FE namespace provides an iterator for traversing contiguous memory
supporting various operations such as dereferencing, pointer arithmetic, and comparison, while ensuring safety against null pointers.
*/
template <typename T>
class contiguous_iterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = var::ptrdiff;
	using pointer = T*;
	using reference = T&;
	using const_pointer = const T*;
	using const_reference = const T&;

protected:
	pointer m_iterator;

public:
	_FE_CONSTEXPR17_ contiguous_iterator() noexcept : m_iterator() {}
	_FE_CONSTEXPR17_ contiguous_iterator(const_pointer const value_p) noexcept : m_iterator(const_cast<pointer>(value_p)) {}
	_FE_CONSTEXPR17_ contiguous_iterator(const contiguous_iterator& other_p) noexcept : m_iterator(other_p.m_iterator) {}
	_FE_CONSTEXPR17_ contiguous_iterator(contiguous_iterator&& other_p) noexcept : m_iterator(other_p.m_iterator) { other_p.m_iterator = nullptr; }
	constexpr ~contiguous_iterator() noexcept {}

	_FE_FORCE_INLINE_ constexpr boolean is_null() const noexcept { return m_iterator == nullptr; }

	_FE_FORCE_INLINE_ constexpr reference operator*() noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(m_iterator);
	}
	_FE_FORCE_INLINE_ constexpr pointer operator->() noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator;
	}

	_FE_FORCE_INLINE_ constexpr const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(m_iterator);
	}
	_FE_FORCE_INLINE_ constexpr const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator;
	}


	_FE_FORCE_INLINE_ constexpr void operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		++(m_iterator);
	}
	_FE_FORCE_INLINE_ constexpr void operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		--(m_iterator);
	}


	_FE_FORCE_INLINE_ constexpr pointer operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator + pointer_offset_p;
	}
	_FE_FORCE_INLINE_ constexpr void operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		m_iterator += pointer_offset_p;
	}


	_FE_FORCE_INLINE_ constexpr pointer operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator - pointer_offset_p;
	}
	_FE_FORCE_INLINE_ constexpr void operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		m_iterator -= pointer_offset_p;
	}
	_FE_FORCE_INLINE_ constexpr difference_type operator-(const contiguous_iterator& value_p) const noexcept
	{
		return m_iterator - value_p.m_iterator;
	}


	_FE_FORCE_INLINE_ constexpr const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator[index_p];
	}
	_FE_FORCE_INLINE_ constexpr reference operator[](const difference_type index_p) noexcept
	{
		FE_NEGATIVE_ASSERT(m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return m_iterator[index_p];
	}


	_FE_FORCE_INLINE_ constexpr contiguous_iterator& operator=(const contiguous_iterator& other_p) noexcept
	{
		m_iterator = other_p.m_iterator;
		return *this;
	}
	_FE_FORCE_INLINE_ constexpr contiguous_iterator& operator=(contiguous_iterator&& rvalue_p) noexcept
	{
		m_iterator = rvalue_p.m_iterator;
		return *this;
	}


	_FE_FORCE_INLINE_ constexpr boolean operator<(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator < other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ constexpr boolean operator<=(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator <= other_p.m_iterator;
	}

	_FE_FORCE_INLINE_ constexpr boolean operator>(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator > other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ constexpr boolean operator>=(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator >= other_p.m_iterator;
	}

	_FE_FORCE_INLINE_ constexpr boolean operator==(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator == other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ constexpr boolean operator!=(const contiguous_iterator& other_p) const noexcept
	{
		return m_iterator != other_p.m_iterator;
	}
};




template <typename T, typename = void>
struct has_iterator_category : std::false_type {};

template <typename T>
struct has_iterator_category<T, std::void_t<typename T::iterator_category>> : std::true_type {};


template <typename Impl>
struct is_iterator : std::false_type {};

template <typename Impl>
struct is_iterator<FE::iterator<Impl>> : std::true_type {};


template <typename Impl>
struct is_const_iterator : std::false_type {};

template <typename Impl>
struct is_const_iterator<FE::const_iterator<Impl>> : std::true_type {};


template <typename Impl>
struct is_reverse_iterator : std::false_type {};

template <typename Impl>
struct is_reverse_iterator<FE::reverse_iterator<Impl>> : std::true_type {};


template <typename Impl>
struct is_const_reverse_iterator : std::false_type {};

template <typename Impl>
struct is_const_reverse_iterator<FE::const_reverse_iterator<Impl>> : std::true_type {};


/*
The FE::is_frogman_iterator<T> struct is a type trait that determines if a given type T is considered an "in-house" iterator by checking if it is an iterator, const iterator, reverse iterator, or const reverse iterator.
*/
template <typename T>
struct is_frogman_iterator 
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (
		(FE::is_iterator<T>::value == true) ||
		(FE::is_const_iterator<T>::value == true) ||
		(FE::is_reverse_iterator<T>::value == true) ||
		(FE::is_const_reverse_iterator<T>::value == true)
		);

};

template <typename T, typename = void>
struct is_frogman_iterator_wrapper : std::false_type {};

template <typename T>
struct is_frogman_iterator_wrapper<T, std::void_t<typename T::wrapped_iterator_type>> : std::true_type {};

/* Possible conversions
* 1. Pointer type -> Frogman Engine Iterator type
* 2. Frogman Engine Iterator type -> Pointer type
* 3. Pointer type -> Pointer type
* 4. Frogman Engine Iterator type -> Frogman Engine Iterator type
* 5. STL Iterator type -> Pointer type (The opposite direction is not allowed).
* 6. STL Iterator type -> Frogman Engine Iterator type (The opposite direction is not allowed).

*/
template<class To, class From>
_FE_FORCE_INLINE_ constexpr To iterator_cast(From ptr_p) noexcept
{
	static_assert(FE::has_iterator_category<From>::value || std::is_pointer<From>::value, "Static assertion failure: template arguments must be a pointer type or an iterator type.");
	static_assert(FE::has_iterator_category<To>::value || std::is_pointer<To>::value, "Static assertion failure: template arguments must be a pointer type or an iterator type.");

	if constexpr (FE::is_frogman_iterator<From>::value == true)
	{
		if constexpr (std::is_pointer_v<To> == true)
		{
			return const_cast<To>(ptr_p.operator->());
		}
		else if constexpr (FE::is_frogman_iterator_wrapper<To>::value == true)
		{
			return To{ ptr_p };
		}
		else if constexpr (std::is_class_v<To> == true)
		{
			return To{ const_cast<typename To::pointer>(ptr_p.operator->()) };
		}
	}
	else if constexpr (FE::is_frogman_iterator_wrapper<From>::value == true)
	{
		if constexpr (std::is_pointer_v<To> == true)
		{
			return const_cast<To>( ptr_p.unwrap().operator->() );
		}
		else if constexpr (FE::is_frogman_iterator_wrapper<To>::value == true)
		{
			return To{ ptr_p.unwrap() };
		}
		else if constexpr (std::is_class_v<To> == true)
		{
			return To{ const_cast<typename To::pointer>( ptr_p.operator->() ) };
		}
	}
	else if constexpr (std::is_class_v<From> == true)
	{
		if constexpr (std::is_pointer_v<To> == true)
		{
			return std::pointer_traits<To>::pointer_to(const_cast<typename From::reference>(*ptr_p));
		}
		else if constexpr (std::is_class_v<To> == true)
		{
			return To{ std::pointer_traits<typename From::pointer>::pointer_to(const_cast<typename From::reference>(*ptr_p)) };
		}
	}
	else if constexpr (std::is_pointer_v<From> == true)
	{
		if constexpr (std::is_pointer_v<To> == true)
		{
			return (To)(ptr_p);
		}
		else if constexpr (FE::is_frogman_iterator_wrapper<To>::value == true)
		{
			return To{ typename To::wrapped_iterator_type{ const_cast<typename std::remove_const_t<From>>(ptr_p) } };
		}
		else if constexpr (std::is_class_v<To> == true)
		{
			return To{ const_cast<typename std::remove_const_t<From>>(ptr_p) };
		}
	}
}

END_NAMESPACE

using FE::iterator_cast;

#endif
