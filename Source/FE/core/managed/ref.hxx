#ifndef _FE_CORE_REF_HXX_
#define _FE_CORE_REF_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/managed/private/ref_table.hpp>




BEGIN_NAMESPACE(FE)


template<typename T>
class ref final
{
	using ref_block_type = internal::managed::ref_block<T>;
	using smart_ref_type = ref_block_type*;
	
public:
	using element_type = typename std::remove_all_extents<T>::type;
	using reference = element_type&;
	using const_reference = const element_type&;

private:
	smart_ref_type m_ref_block;

public:
	_FORCE_INLINE_ _CONSTEXPR20_ ref() noexcept : m_ref_block() {}

	_FORCE_INLINE_ _CONSTEXPR23_ ~ref() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ref(const ref& other_p) noexcept : m_ref_block(other_p.m_ref_block)
	{
		if (other_p.m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ref(ref&& rvalue_p) noexcept : m_ref_block(rvalue_p.m_ref_block) { rvalue_p.m_ref_block = nullptr; }

	_FORCE_INLINE_ _CONSTEXPR20_ ref(const FE::trackable<T>& trackable_p) noexcept : m_ref_block(trackable_p.m_ref_block)
	{
		if (this->m_ref_block != nullptr)
		{
			++(this->m_ref_block->_ref_count);
		}
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ref& operator=(const ref& other_p) noexcept
	{
		if (other_p.m_ref_block != nullptr)
		{
			this->m_ref_block = other_p.m_ref_block;
			++(this->m_ref_block->_ref_count);
		}
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ref& operator=(ref&& rvalue_p) noexcept
	{
		if (rvalue_p.m_ref_block != nullptr)
		{
			this->m_ref_block = rvalue_p.m_ref_block;
			rvalue_p.m_ref_block = nullptr;
		}
		return *this;
	}

	_FORCE_INLINE_ _CONSTEXPR20_ ref& operator=(const FE::trackable<T>& trackable_p) noexcept
	{
		if (trackable_p.m_ref_block == nullptr)
		{
			return *this;
		}
		
		this->m_ref_block = trackable_p.m_ref_block;
		++(this->m_ref_block->_ref_count);
		return *this;
	}

	_FORCE_INLINE_ void reset() noexcept
	{
		if (this->m_ref_block != nullptr)
		{
			this->__destruct();
		}
	}

 	_FORCE_INLINE_ void swap(ref& in_out_other_p) noexcept { this->m_ref_block = algorithm::utility::exchange<smart_ref_type>(in_out_other_p.m_ref_block, nullptr); }

	_FORCE_INLINE_ boolean is_expired() const noexcept { return (this->m_ref_block == nullptr) || (this->m_ref_block->_address == nullptr); }
	_FORCE_INLINE_ explicit operator bool() const noexcept { return !(this->is_expired()); }
	_FORCE_INLINE_ boolean operator!() const noexcept { return this->is_expired(); }

	_FORCE_INLINE_ const_reference get() const noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failed: unable to get() a null reference");
		return *(static_cast<element_type*>(this->m_ref_block->_address));
	}
	_FORCE_INLINE_ reference get() noexcept
	{
		FE_ASSERT(this->is_expired() == true, "Assertion failed: unable to get() a null reference");
		return *(static_cast<element_type*>(this->m_ref_block->_address));
	}

	_FORCE_INLINE_ boolean operator==(_MAYBE_UNUSED_ FE::null_t nullref_p) const noexcept { return this->is_expired(); }
	_FORCE_INLINE_ boolean operator!=(_MAYBE_UNUSED_ FE::null_t nullref_p) const noexcept { return !(this->is_expired()); }

private:
	_FORCE_INLINE_ _CONSTEXPR20_ void __destruct() noexcept
	{	
		FE_ASSERT(this->m_ref_block == nullptr, "${%s@0}: The smart pointer was nullptr.", TO_STRING(FE::ERROR_CODE::_FATAL_ERROR_NULLref));
		--(this->m_ref_block->_ref_count);

		if((this->m_ref_block->_address == nullptr) && (this->m_ref_block->_ref_count == 0))
		{
			delete this->m_ref_block;
			this->m_ref_block = nullptr;
		}
	}
};
END_NAMESPACE
#endif 