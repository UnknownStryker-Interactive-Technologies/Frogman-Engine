#ifndef _FE_CORE_TRACKABLE_HXX_
#define _FE_CORE_TRACKABLE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/managed/private/ref_table.hpp>




BEGIN_NAMESPACE(FE)


/* NOTICE: 
   Any objects that contain one of the FE.core.managed pointer and reference cannot have static lifetime.
   It will result in the segmentation fault(address not mapped to the object) at the end of the program execution.
   
   FE.core.managed pointer and reference: FE::trackable, FE::ptr, FE::trackable, and FE::ref.
   FE.core classes associated with those: FE::string, FE::string_view, FE::array, and other FE.core template data containers. 
*/
template <typename T>
class trackable final
{
	friend class ref<T>;

	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ref_type = ref_block_type*;

public:
	using element_type = T;
	using reference = element_type&;
	using const_reference = const element_type&;

private:
	element_type m_data;
	smart_ref_type m_ref_block;

public:
	_FORCE_INLINE_ _CONSTEXPR17_ trackable() noexcept : m_data(), m_ref_block() {}
	_FORCE_INLINE_ _CONSTEXPR23_ ~trackable() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ trackable(const trackable& other_p) noexcept : m_data(other_p.m_data), m_ref_block(new ref_block_type) { this->m_ref_block->_address = &(this->m_data); }
	_FORCE_INLINE_ _CONSTEXPR20_ trackable(trackable&& rvalue_p) noexcept : m_data(std::move(rvalue_p.m_data)), m_ref_block(rvalue_p.m_ref_block) { rvalue_p.m_ref_block = nullptr; }
	_FORCE_INLINE_ _CONSTEXPR20_ trackable(const element_type& value_p) noexcept : m_data(value_p), m_ref_block(new ref_block_type) { this->m_ref_block->_address = &(this->m_data); }

	template<typename... Arguments>
	_FORCE_INLINE_ _CONSTEXPR20_ trackable(Arguments&&... values_p) noexcept : m_data(std::forward<Arguments&&>(values_p)...), m_ref_block(new ref_block_type) { this->m_ref_block->_address = &(this->m_data); }

	_FORCE_INLINE_ _CONSTEXPR20_ trackable& operator=(const trackable& other_p) noexcept
	{
		if (this->m_ref_block == nullptr)
		{
			this->m_ref_block = new ref_block_type;
		}

		this->m_data = other_p.m_data;
		this->m_ref_block->_address = &(this->m_data);
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ trackable& operator=(trackable&& rvalue_p) noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}

		this->m_ref_block = rvalue_p.m_ref_block;
		rvalue_p.m_ref_block = nullptr;
		
		this->m_data = std::move(rvalue_p.m_data);
		this->m_ref_block->_address = &(this->m_data);
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ trackable& operator=(const element_type& value_p) noexcept
	{
		if (this->m_ref_block == nullptr)
		{
			this->m_ref_block = new ref_block_type;
		}

		this->m_data = value_p;
		this->m_ref_block->_address = &(this->m_data);
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ void reset(const element_type& value_p) noexcept { *this->operator=(value_p); }

	_FORCE_INLINE_ _CONSTEXPR20_ void swap(trackable& in_out_other_p) noexcept
	{
		trackable l_tmp = std::move(in_out_other_p);
		in_out_other_p = std::move(*this);
		*this = std::move(l_tmp);
	}

	_FORCE_INLINE_ _CONSTEXPR20_ reference get() noexcept { return this->m_data; }
	_FORCE_INLINE_ _CONSTEXPR20_ const_reference get() const noexcept { return this->m_data; }

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "Assertion failure: The smart reference was null.");
		if (this->m_ref_block->_address != nullptr)
		{
			this->m_ref_block->_address = nullptr;
		}

		if(this->m_ref_block->_ref_count == 0)
		{
			delete this->m_ref_block;
			this->m_ref_block = nullptr;
		}
	}
};

END_NAMESPACE
#endif 