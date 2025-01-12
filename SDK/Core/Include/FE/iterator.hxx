#ifndef _FE_CORE_ITERATOR_HXX_
#define _FE_CORE_ITERATOR_HXX_
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
#include <FE/type_traits.hxx>




BEGIN_NAMESPACE(FE)


/*
The FE::iterator<Implementation> class template provides a type-safe iterator that extends a specified implementation
enforcing non-null assertions and supporting various iterator operations such as dereferencing, incrementing, and arithmetic.
*/
template <class Implementation>
class iterator final : public Implementation
{
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	iterator() noexcept : base_type() {}
	iterator(const pointer value_p) noexcept : base_type(value_p) {}
	iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	~iterator() noexcept {}
	

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator*() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pointer operator->() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator operator++(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator--(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(other_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator[](index_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator[](const difference_type index_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator[](index_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class reverse_iterator final : public Implementation
{
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;

	reverse_iterator() noexcept : base_type() {}
	reverse_iterator(const pointer value_p) noexcept : base_type(value_p) {}
	reverse_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	reverse_iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	~reverse_iterator() noexcept {}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator*() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pointer operator->() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator operator++(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator--(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		FE_NEGATIVE_ASSERT(other_p.is_null() == true, "${%s@0}: The right-hand input reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

		return other_p.operator-(*this);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator[](const difference_type index_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(this->operator+(index_p));
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(this->operator+(index_p));
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class const_iterator final : public Implementation
{
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
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


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null const_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null const_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator++();
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator operator++(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator--();
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator--(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator+(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(pointer_offset_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(other_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator[](index_p);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
class const_reverse_iterator final : public Implementation
{
	FE_NEGATIVE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");
public:
	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
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


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null const_reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator*();
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null const_reverse_iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator->();
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator--();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator operator++(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator++();
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator--(int) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		const_reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator-(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return Implementation::operator+(pointer_offset_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		FE_NEGATIVE_ASSERT(other_p.is_null() == true, "${%s@0}: The right-hand input const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));

		return other_p.operator-(*this);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(this->operator+(index_p));
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const base_type& other_p) const noexcept
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
	using category = contiguous_iterator<typename FE::remove_const_reference<T>::type>;
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
	_FE_CONSTEXPR20_ ~contiguous_iterator() noexcept {}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean is_null() const noexcept { return this->m_iterator == nullptr; }

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator*() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(this->m_iterator);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pointer operator->() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator*() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return *(this->m_iterator);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_pointer operator->() const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void operator++() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		++(this->m_iterator);
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void operator--() noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		--(this->m_iterator);
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pointer operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator + pointer_offset_p;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		this->m_iterator += pointer_offset_p;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ pointer operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator - pointer_offset_p;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ void operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		this->m_iterator -= pointer_offset_p;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ difference_type operator-(const contiguous_iterator& value_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator - value_p.m_iterator;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ const_reference operator[](const difference_type index_p) const noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator[index_p];
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ reference operator[](const difference_type index_p) noexcept
	{
		FE_NEGATIVE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr));
		return this->m_iterator[index_p];
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ contiguous_iterator& operator=(const contiguous_iterator& other_p) noexcept
	{
		this->m_iterator = other_p.m_iterator;
		return *this;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ contiguous_iterator& operator=(contiguous_iterator&& rvalue_p) noexcept
	{
		this->m_iterator = rvalue_p.m_iterator;
		return *this;
	}


	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator < other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator<=(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator <= other_p.m_iterator;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator > other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator>=(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator >= other_p.m_iterator;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator == other_p.m_iterator;
	}
	_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const contiguous_iterator& other_p) const noexcept
	{
		return this->m_iterator != other_p.m_iterator;
	}
};




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
The FE::is_in_house_iterator<T> struct is a type trait that determines if a given type T is considered an "in-house" iterator by checking if it is an iterator, const iterator, reverse iterator, or const reverse iterator.
*/
template <typename T>
struct is_in_house_iterator
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (
		(FE::is_iterator<T>::value == true) ||
		(FE::is_const_iterator<T>::value == true) ||
		(FE::is_reverse_iterator<T>::value == true) ||
		(FE::is_const_reverse_iterator<T>::value == true)
		);

};

/* Possible conversions
* 1. Pointer type -> Frogman Engine Iterator type
* 2. Frogman Engine Iterator type -> Pointer type
* 3. Pointer type -> Pointer type
* 4. Frogman Engine Iterator type -> Frogman Engine Iterator type
* 5. STL Iterator type -> Pointer type (The opposite direction is not allowed).
* 6. STL Iterator type -> Frogman Engine Iterator type (The opposite direction is not allowed).

*/
template<class To, class From>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ To iterator_cast(From ptr_p) noexcept
{
	FE_NEGATIVE_STATIC_ASSERT(((std::is_class<From>::value == false) && (std::is_pointer<From>::value == false)), "Static assertion failure: template arguments must be a pointer type or an iterator type.");
	FE_NEGATIVE_STATIC_ASSERT(((std::is_class<To>::value == false) && (std::is_pointer<To>::value == false)), "Static assertion failure: template arguments must be a pointer type or an iterator type.");

	if constexpr (FE::is_in_house_iterator<From>::value == true)
	{
		if constexpr (std::is_pointer<To>::value == true)
		{
			return const_cast<To>(ptr_p.operator->());
		}
		else if constexpr (std::is_class<To>::value == true)
		{
			return To{ const_cast<typename To::pointer>(ptr_p.operator->()) };
		}
	}
	else if constexpr (std::is_class<From>::value == true)
	{
		if constexpr (std::is_pointer<To>::value == true)
		{
			return std::pointer_traits<To>::pointer_to(const_cast<typename From::reference>(*ptr_p));
		}
		else if constexpr (std::is_class<To>::value == true)
		{
			return To{ std::pointer_traits<typename From::pointer>::pointer_to(const_cast<typename From::reference>(*ptr_p)) };
		}
	}
	else if constexpr (std::is_pointer<From>::value == true)
	{
		if constexpr (std::is_pointer<To>::value == true)
		{
			return (To)(ptr_p);
		}
		else if constexpr (std::is_class<To>::value == true)
		{
			return To{ const_cast<typename std::remove_const<From>::type>(ptr_p) };
		}
	}
}

END_NAMESPACE

using FE::iterator_cast;

#endif
