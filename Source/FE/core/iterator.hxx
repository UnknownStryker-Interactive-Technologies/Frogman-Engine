#ifndef _FE_CORE_ITERATOR_HXX_
#define _FE_CORE_ITERATOR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>




BEGIN_NAMESPACE(FE)


template<class To, class From>
_FORCE_INLINE_ To iterator_cast(const From& ptr_p) noexcept
{
	FE_STATIC_ASSERT(((std::is_class<From>::value == false) && (std::is_pointer<From>::value == false)), "Static assertion failure: template arguments must be a pointer type or an iterator type.");
	FE_STATIC_ASSERT(((std::is_class<To>::value == false) && (std::is_pointer<To>::value == false)), "Static assertion failure: template arguments must be a pointer type or an iterator type.");

	if constexpr (std::is_class<To>::value == true)
	{
		return To{ const_cast<To>(ptr_p) };
	}
	else if constexpr (std::is_class<To>::value == false && std::is_pointer<To>::value == true)
	{
		if constexpr (std::is_class<From>::value == true)
		{
			return const_cast<To>(ptr_p.operator->());
		}
		else if constexpr (std::is_class<From>::value == false && std::is_pointer<From>::value == true)
		{
			return const_cast<To>(ptr_p);
		}
	}
}


template <class Implementation>
struct iterator final : public Implementation
{
	FE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");

	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;
	using index_type = typename Implementation::index_type;

public:
	_FORCE_INLINE_ iterator() noexcept : base_type() {}
	_FORCE_INLINE_ iterator(const pointer value_p) noexcept : base_type(value_p) {}
	_FORCE_INLINE_ iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	_FORCE_INLINE_ ~iterator() noexcept {}
	

	_FORCE_INLINE_ reference operator*() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator*();
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator->();
	}


	_FORCE_INLINE_ iterator& operator++() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator++();
		return *this;
	}

	_FORCE_INLINE_ iterator operator++(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FORCE_INLINE_ iterator& operator--() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator--();
		return *this;
	}

	_FORCE_INLINE_ iterator& operator--(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FORCE_INLINE_ iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator+(pointer_offset_p);
	}

	_FORCE_INLINE_ iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FORCE_INLINE_ iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(pointer_offset_p);
	}

	_FORCE_INLINE_ iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FORCE_INLINE_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(other_p);
	}


	_FORCE_INLINE_ reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return Implementation::operator[](index_p);
	}


	_FORCE_INLINE_ iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FORCE_INLINE_ iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FORCE_INLINE_ iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FORCE_INLINE_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FORCE_INLINE_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FORCE_INLINE_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FORCE_INLINE_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FORCE_INLINE_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FORCE_INLINE_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
struct reverse_iterator final : public Implementation
{
	FE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");

	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;
	using index_type = typename Implementation::index_type;

public:
	_FORCE_INLINE_ reverse_iterator() noexcept : base_type() {}
	_FORCE_INLINE_ reverse_iterator(const pointer value_p) noexcept : base_type(value_p) {}
	_FORCE_INLINE_ reverse_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ reverse_iterator(base_type&& rvalue_p) noexcept : base_type(rvalue_p) {}
	_FORCE_INLINE_ ~reverse_iterator() noexcept {}


	_FORCE_INLINE_ reference operator*() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null reverse_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator*();
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null reverse_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator->();
	}


	_FORCE_INLINE_ reverse_iterator& operator++() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator--();
		return *this;
	}

	_FORCE_INLINE_ reverse_iterator operator++(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FORCE_INLINE_ reverse_iterator& operator--() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator++();
		return *this;
	}

	_FORCE_INLINE_ reverse_iterator& operator--(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FORCE_INLINE_ reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(pointer_offset_p);
	}

	_FORCE_INLINE_ reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FORCE_INLINE_ reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator+(pointer_offset_p);
	}

	_FORCE_INLINE_ reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}

	_FORCE_INLINE_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(other_p.is_null() == true, "${%s@0}: The right-hand input reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		const base_type* const l_other_parent = &other_p;
		return l_other_parent->operator-(*this);
	}


	_FORCE_INLINE_ reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return Implementation::operator[](index_p);
	}


	_FORCE_INLINE_ reverse_iterator& operator=(const pointer value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FORCE_INLINE_ reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FORCE_INLINE_ reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FORCE_INLINE_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FORCE_INLINE_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FORCE_INLINE_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FORCE_INLINE_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FORCE_INLINE_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FORCE_INLINE_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
struct const_iterator final : public Implementation
{
	FE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");

	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;
	using index_type = typename Implementation::index_type;

public:
	_FORCE_INLINE_ const_iterator() noexcept : base_type() {}
	_FORCE_INLINE_ const_iterator(const_pointer const value_p) noexcept : base_type(value_p) {}
	_FORCE_INLINE_ const_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ const_iterator(base_type&& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ ~const_iterator() noexcept {}


	_FORCE_INLINE_ const_reference operator*() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null const_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator*();
	}

	_FORCE_INLINE_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null const_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator->();
	}


	_FORCE_INLINE_ const_iterator& operator++() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator++();
		return *this;
	}

	_FORCE_INLINE_ const_iterator operator++(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FORCE_INLINE_ const_iterator& operator--() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator--();
		return *this;
	}

	_FORCE_INLINE_ const_iterator& operator--(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FORCE_INLINE_ const_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator+(pointer_offset_p);
	}

	_FORCE_INLINE_ const_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}


	_FORCE_INLINE_ const_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(pointer_offset_p);
	}

	_FORCE_INLINE_ const_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}

	_FORCE_INLINE_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(other_p);
	}


	_FORCE_INLINE_ reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return Implementation::operator[](index_p);
	}


	_FORCE_INLINE_ const_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FORCE_INLINE_ const_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FORCE_INLINE_ const_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FORCE_INLINE_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FORCE_INLINE_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FORCE_INLINE_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FORCE_INLINE_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FORCE_INLINE_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FORCE_INLINE_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};


template <class Implementation>
struct const_reverse_iterator final : public Implementation
{
	FE_STATIC_ASSERT(std::is_class<Implementation>::value == false, "Static Assertion Failed: Illegal Implementation Detected.");

	using base_type = Implementation;
	using iterator_category = typename Implementation::category;
	using value_type = typename Implementation::value_type;
	using difference_type = typename Implementation::difference_type;
	using pointer = typename Implementation::pointer;
	using reference = typename Implementation::reference;
	using const_pointer = typename Implementation::const_pointer;
	using const_reference = typename Implementation::const_reference;
	using index_type = typename Implementation::index_type;

public:
	_FORCE_INLINE_ const_reverse_iterator() noexcept : base_type() {}
	_FORCE_INLINE_ const_reverse_iterator(const_pointer const value_p) noexcept : base_type(value_p) {}
	_FORCE_INLINE_ const_reverse_iterator(const base_type& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ const_reverse_iterator(base_type&& other_p) noexcept : base_type(other_p) {}
	_FORCE_INLINE_ ~const_reverse_iterator() noexcept {}


	_FORCE_INLINE_ const_reference operator*() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to dereference a null const_reverse_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator*();
	}

	_FORCE_INLINE_ const_pointer operator->() const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: Unable to access a null const_reverse_iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator->();
	}


	_FORCE_INLINE_ const_reverse_iterator& operator++() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator--();
		return *this;
	}

	_FORCE_INLINE_ const_reverse_iterator operator++(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		reverse_iterator l_temporary = *this;
		Implementation::operator--();
		return l_temporary;
	}


	_FORCE_INLINE_ const_reverse_iterator& operator--() noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator++();
		return *this;
	}

	_FORCE_INLINE_ const_reverse_iterator& operator--(int) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		const_reverse_iterator l_temporary = *this;
		Implementation::operator++();
		return l_temporary;
	}


	_FORCE_INLINE_ const_reverse_iterator operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator-(pointer_offset_p);
	}

	_FORCE_INLINE_ const_reverse_iterator& operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator-=(pointer_offset_p);
		return *this;
	}


	_FORCE_INLINE_ const_reverse_iterator operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return Implementation::operator+(pointer_offset_p);
	}

	_FORCE_INLINE_ const_reverse_iterator& operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		Implementation::operator+=(pointer_offset_p);
		return *this;
	}

	_FORCE_INLINE_ difference_type operator-(const base_type& other_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(other_p.is_null() == true, "${%s@0}: The right-hand input const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));

		const base_type* const l_other_parent = &other_p;
		return l_other_parent->operator-(*this);
	}


	_FORCE_INLINE_ const_reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->is_null() == true, "${%s@0}: The const_reverse_iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return Implementation::operator[](index_p);
	}


	_FORCE_INLINE_ const_reverse_iterator& operator=(const_pointer const value_p) noexcept
	{
		Implementation::operator=(value_p);
		return *this;
	}

	_FORCE_INLINE_ const_reverse_iterator& operator=(const base_type& other_p) noexcept
	{
		Implementation::operator=(other_p);
		return *this;
	}

	_FORCE_INLINE_ const_reverse_iterator& operator=(base_type&& rvalue_p) noexcept
	{
		Implementation::operator=(rvalue_p);
		rvalue_p = nullptr;
		return *this;
	}


	_FORCE_INLINE_ boolean operator<(const base_type& other_p) const noexcept
	{
		return Implementation::operator<(other_p);
	}

	_FORCE_INLINE_ boolean operator<=(const base_type& other_p) const noexcept
	{
		return Implementation::operator<=(other_p);
	}

	_FORCE_INLINE_ boolean operator>(const base_type& other_p) const noexcept
	{
		return Implementation::operator>(other_p);
	}

	_FORCE_INLINE_ boolean operator>=(const base_type& other_p) const noexcept
	{
		return Implementation::operator>=(other_p);
	}

	_FORCE_INLINE_ boolean operator==(const base_type& other_p) const noexcept
	{
		return Implementation::operator==(other_p);
	}

	_FORCE_INLINE_ boolean operator!=(const base_type& other_p) const noexcept
	{
		return Implementation::operator!=(other_p);
	}
};




template <typename T>
struct contiguous_iterator
{
	using category = contiguous_iterator<typename FE::remove_const_reference<T>::type>;
	using value_type = T;
	using difference_type = var::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using const_pointer = const T*;
	using const_reference = const T&;
	using index_type = var::index_t;

protected:
	pointer m_iterator;

public:
	_FORCE_INLINE_ contiguous_iterator() noexcept : m_iterator() {}
	_FORCE_INLINE_ contiguous_iterator(const_pointer const value_p) noexcept : m_iterator(const_cast<pointer>(value_p)) {}
	_FORCE_INLINE_ contiguous_iterator(const contiguous_iterator& other_p) noexcept : m_iterator(other_p.m_iterator) {}
	_FORCE_INLINE_ contiguous_iterator(contiguous_iterator&& other_p) noexcept : m_iterator(other_p.m_iterator) { other_p.m_iterator = nullptr; }
	_FORCE_INLINE_ ~contiguous_iterator() noexcept {}

	_FORCE_INLINE_ boolean is_null() const noexcept { return this->m_iterator == nullptr; }

	_FORCE_INLINE_ reference operator*() const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return *(this->m_iterator);
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator;
	}


	_FORCE_INLINE_ void operator++() noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		++(this->m_iterator);
	}

	_FORCE_INLINE_ void operator--() noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		--(this->m_iterator);
	}


	_FORCE_INLINE_ pointer operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator + pointer_offset_p;
	}

	_FORCE_INLINE_ void operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator += pointer_offset_p;
	}


	_FORCE_INLINE_ pointer operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator - pointer_offset_p;
	}

	_FORCE_INLINE_ void operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator -= pointer_offset_p;
	}


	_FORCE_INLINE_ difference_type operator-(const contiguous_iterator& value_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator - value_p.m_iterator;
	}


	_FORCE_INLINE_ reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return this->m_iterator[index_p];
	}


	_FORCE_INLINE_ contiguous_iterator& operator=(const contiguous_iterator& other_p) noexcept
	{
		this->m_iterator = other_p.m_iterator;
		return *this;
	}

	_FORCE_INLINE_ contiguous_iterator& operator=(contiguous_iterator&& rvalue_p) noexcept
	{
		this->m_iterator = rvalue_p.m_iterator;
		return *this;
	}

	_FORCE_INLINE_ boolean operator<(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator < other_p.m_iterator;
	}

	_FORCE_INLINE_ boolean operator<=(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator <= other_p.m_iterator;
	}

	_FORCE_INLINE_ boolean operator>(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator > other_p.m_iterator;
	}

	_FORCE_INLINE_ boolean operator>=(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator >= other_p.m_iterator;
	}

	_FORCE_INLINE_ boolean operator==(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator == other_p.m_iterator;
	}

	_FORCE_INLINE_ boolean operator!=(const contiguous_iterator& other_p) const noexcept
	{
		FE_ASSERT(this->m_iterator == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator != other_p.m_iterator;
	}
};




template<typename T>
class ptr;


template <typename T>
struct obsessive_iterator
{
	struct ptr;

	using category = contiguous_iterator<typename FE::remove_const_reference<T>::type>;
	using value_type = T;
	using difference_type = var::ptrdiff_t;
	using pointer = ptr;
	using reference = value_type&;
	using const_pointer = const pointer;
	using const_reference = const reference;
	using index_type = var::index_t;

protected:
	pointer m_iterator;

public:
	_FORCE_INLINE_ obsessive_iterator() noexcept : m_iterator() {}
	_FORCE_INLINE_ obsessive_iterator(const_pointer& value_p) noexcept : m_iterator(value_p._smart_ptr, value_p._index) {}
	_FORCE_INLINE_ obsessive_iterator(const obsessive_iterator& other_p) noexcept : m_iterator(other_p.m_iterator._smart_ptr, other_p.m_iterator._index) {}
	_FORCE_INLINE_ obsessive_iterator(obsessive_iterator&& other_p) noexcept : m_iterator(other_p.m_iterator._smart_ptr, other_p.m_iterator._index)
	{
		other_p._smart_ptr = nullptr; 
		other_p._index = 0;
	}
	_FORCE_INLINE_ ~obsessive_iterator() noexcept {}

	_FORCE_INLINE_ boolean is_null() const noexcept { return this->m_iterator == nullptr; }

	_FORCE_INLINE_ reference operator*() const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator.operator*();
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator.operator->();
	}


	_FORCE_INLINE_ void operator++() noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator.operator++();
	}

	_FORCE_INLINE_ void operator--() noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator.operator--();
	}


	_FORCE_INLINE_ pointer operator+(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator.operator+(pointer_offset_p);
	}

	_FORCE_INLINE_ void operator+=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator.operator+=(pointer_offset_p);
	}


	_FORCE_INLINE_ pointer operator-(const difference_type pointer_offset_p) const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator.operator-(pointer_offset_p);
	}

	_FORCE_INLINE_ void operator-=(const difference_type pointer_offset_p) noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		this->m_iterator.operator-=(pointer_offset_p);
	}


	_FORCE_INLINE_ difference_type operator-(const obsessive_iterator& value_p) const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		return this->m_iterator.operator-(value_p.m_iterator);
	}


	_FORCE_INLINE_ reference operator[](const index_type index_p) const noexcept
	{
		FE_ASSERT(this->m_iterator._smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		return this->m_iterator.operator[](index_p);
	}


	_FORCE_INLINE_ obsessive_iterator& operator=(const obsessive_iterator&& other_p) noexcept
	{
		this->m_iterator.operator=(other_p.m_iterator);
		return *this;
	}

	_FORCE_INLINE_ obsessive_iterator& operator=(obsessive_iterator&& rvalue_p) noexcept
	{
		this->m_iterator.operator=(rvalue_p.m_iterator);
		return *this;
	}


	_FORCE_INLINE_ boolean operator<(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator<(other_p.m_iterator);
	}

	_FORCE_INLINE_ boolean operator<=(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator<=(other_p.m_iterator);
	}

	_FORCE_INLINE_ boolean operator>(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator>(other_p.m_iterator);
	}

	_FORCE_INLINE_ boolean operator>=(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator>=(other_p.m_iterator);
	}

	_FORCE_INLINE_ boolean operator==(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator==(other_p.m_iterator);
	}

	_FORCE_INLINE_ boolean operator!=(const obsessive_iterator& other_p) const noexcept
	{
		return this->m_iterator.operator!=(other_p.m_iterator);
	}




	struct ptr
	{
		FE::ptr<T[]> _smart_ptr;
		var::index_t _index = 0;


		_FORCE_INLINE_ T& operator*() const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: Unable to dereference a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			return *(this->operator->());
		}

		_FORCE_INLINE_ T* operator->() const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: Unable to access a null iterator.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			return (this->_smart_ptr.operator->() + this->_index);
		}

		_FORCE_INLINE_ void operator++() noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			++(this->_index);
		}

		_FORCE_INLINE_ void operator--() noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			--(this->_index);
		}


		_FORCE_INLINE_ T* operator+(const ptrdiff_t pointer_offset_p) const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			return this->operator->() + pointer_offset_p;
		}

		_FORCE_INLINE_ void operator+=(const ptrdiff_t pointer_offset_p) noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			this->_index += pointer_offset_p;
		}


		_FORCE_INLINE_ T* operator-(const ptrdiff_t pointer_offset_p) const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			return this->operator->() - pointer_offset_p;
		}

		_FORCE_INLINE_ void operator-=(const ptrdiff_t pointer_offset_p) noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			this->_index -= pointer_offset_p;
		}


		_FORCE_INLINE_ ptrdiff_t operator-(const ptr& value_p) const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			return this->operator->() - value_p.operator->();
		}


		_FORCE_INLINE_ T& operator[](const index_type index_p) const noexcept
		{
			FE_ASSERT(this->_smart_ptr == nullptr, "${%s@0}: The iterator was null.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
			FE_ASSERT(index_p == FE::max_value<index_t>, "${%s@0}: negative integers cannot be assigned to an index_t variable.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
			return this->operator->()[index_p];
		}


		_FORCE_INLINE_ void operator=(const ptr& value_p) noexcept
		{
			this->_smart_ptr = value_p._smart_ptr;
			this->_index = value_p._index;
		}


		_FORCE_INLINE_ boolean operator<(const ptr& other_p) const noexcept
		{
			return this->operator->() < other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator<=(const ptr& other_p) const noexcept
		{
			return this->operator->() <= other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator>(const ptr& other_p) const noexcept
		{
			return this->operator->() > other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator>=(const ptr& other_p) const noexcept
		{
			return this->operator->() >= other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator==(const ptr& other_p) const noexcept
		{
			return this->operator->() == other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator!=(const ptr& other_p) const noexcept
		{
			return this->operator->() != other_p.operator->();
		}

		_FORCE_INLINE_ boolean operator==(const T* const value_p) const noexcept
		{
			return this->operator->() == value_p;
		}

		_FORCE_INLINE_ boolean operator!=(const T* const value_p) const noexcept
		{
			return this->operator->() != value_p;
		}
	};
};




END_NAMESPACE


using FE::iterator_cast;


#endif
