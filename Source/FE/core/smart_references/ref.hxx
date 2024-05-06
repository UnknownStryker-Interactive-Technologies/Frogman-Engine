#ifndef _FE_CORE_REF_HXX_
#define _FE_CORE_REF_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/smart_references/private/smart_ref_base.hpp>




BEGIN_NAMESPACE(FE)


template<typename T>
class object;


template<typename T>
class ref final : public internal::smart_ref::ref_base
{
public:
	using base_type = internal::smart_ref::ref_base;
	using ref_table_key_type = typename base_type::ref_table_key_type;

	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;

private:
	mutable element_type* m_smart_ref;
	mutable ref_table_key_type m_key;

public:
	_CONSTEXPR20_ ref() noexcept : m_smart_ref(), m_key(base_type::invalid_key_value) {}

	_CONSTEXPR20_ ref(const ref& other_p) noexcept : m_smart_ref(reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ref != nullptr)
		{
			this->m_key = other_p.m_key;
		}
	}

	_CONSTEXPR20_ ref(ref&& rvalue_p) noexcept : m_smart_ref(reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ref != nullptr)
		{
			rvalue_p.m_smart_ref = nullptr;
			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
	}

	_CONSTEXPR20_ ref(const FE::object<T>& object_p) noexcept : m_smart_ref(reinterpret_cast<pointer>(base_type::__get_property_address(object_p.get_key()))), m_key(base_type::invalid_key_value)
	{
		if (this->m_smart_ref != nullptr)
		{
			this->m_key = object_p.get_key();
		}
	}

	_CONSTEXPR20_ ref& operator=(const ref& other_p) noexcept
	{
		if (other_p.m_smart_ref == nullptr)
		{
			return *this;
		}

		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		if (this->m_smart_ref != nullptr)
		{
			this->m_key = other_p.m_key;
		}
		return *this;
	}

	_CONSTEXPR20_ ref& operator=(ref&& rvalue_p) noexcept
	{
		if (rvalue_p.m_smart_ref == nullptr)
		{
			return *this;
		}

		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(rvalue_p.m_key));
		if (this->m_smart_ref != nullptr)
		{
			rvalue_p.m_smart_ref = nullptr;

			this->m_key = algorithm::utility::exchange<ref_table_key_type>(rvalue_p.m_key, base_type::invalid_key_value);
		}
		return *this;
	}

	_CONSTEXPR20_ ref& operator=(const FE::object<T>& object_p) noexcept
	{
		ref_table_key_type l_retrieved_ref_table_key = object_p.get_key();
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(l_retrieved_ref_table_key));
		if (this->m_smart_ref != nullptr)
		{
			this->m_key = l_retrieved_ref_table_key;
		}
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		this->m_smart_ref = nullptr;
		this->m_key = base_type::invalid_key_value;
	}

	_FORCE_INLINE_ ref_table_key_type get_key() const noexcept
	{
		return this->m_key;
	}

	_CONSTEXPR20_ void swap(ref& in_out_other_p) noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		in_out_other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(in_out_other_p.m_key));

		pointer l_temporary_smart_ptr = algorithm::utility::exchange<pointer>(in_out_other_p.m_smart_ref, this->m_smart_ref);
		this->m_smart_ref = l_temporary_smart_ptr;

		ref_table_key_type l_temporary_key = algorithm::utility::exchange<ref_table_key_type>(in_out_other_p.m_key, this->m_key);
		this->m_key = l_temporary_key;
	}


	_FORCE_INLINE_ boolean is_expired() const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		return this->m_smart_ref == nullptr;
	}

	_FORCE_INLINE_ pointer get_unchecked() const noexcept
	{
		FE_ASSERT(this->m_smart_ref == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ref));
		return this->m_smart_ref;
	}

	_FORCE_INLINE_ explicit operator bool() const noexcept
	{
		return !(this->is_expired());
	}

	_FORCE_INLINE_ boolean operator!() const noexcept
	{
		return this->is_expired();
	}

	_FORCE_INLINE_ element_type& operator*() const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ref == nullptr, "${%s@0}: ${%s@1} is pointing to an invalid object", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ref));
		return *(this->m_smart_ref);
	}

	_FORCE_INLINE_ pointer operator->() const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		FE_ASSERT(this->m_smart_ref == nullptr, "${%s@0}: ${%s@1} is pointing to an invalid object", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_smart_ref));
		return this->m_smart_ref;
	}


	_FORCE_INLINE_ boolean operator==(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref == other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator!=(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref != other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator>(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref > other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator>=(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref >= other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator<(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref < other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator<=(const ref& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));
		other_p.m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(other_p.m_key));
		return this->m_smart_ref <= other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator==(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref == other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator!=(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref != other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator>(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref > other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator>=(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref >= other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator<(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref < other_p.m_smart_ref;
	}

	_FORCE_INLINE_ boolean operator<=(const FE::object<T>& other_p) const noexcept
	{
		this->m_smart_ref = reinterpret_cast<pointer>(base_type::__get_property_address(this->m_key));

		return this->m_smart_ref <= other_p.m_smart_ref;
	}
};


END_NAMESPACE
#endif 