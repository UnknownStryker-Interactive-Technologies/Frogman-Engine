#ifndef _FE_CORE_SMART_PTRS_PRIVATE_SMART_PTR_VARIANTS_BASE_HPP_
#define _FE_CORE_SMART_PTRS_PRIVATE_SMART_PTR_VARIANTS_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/allocator.hxx>
#include <deque>




BEGIN_NAMESPACE(FE::internal::smart_ptr)


class smart_ptr_variants_base 
{
public:
	using ref_table_type = std::deque<void*, FE::aligned_allocator<void*>>;
	using ref_table_value_type = typename ref_table_type::value_type;

	using ref_table_key_type = typename ref_table_type::pointer;
	using ref_table_recycler_type = std::deque<ref_table_key_type, FE::aligned_allocator<ref_table_key_type>>;

	static constexpr ref_table_key_type invalid_key_value = nullptr;
	
protected:
	thread_local static ref_table_type tl_s_ref_table;
	thread_local static ref_table_recycler_type tl_s_ref_table_recycler;

public:
	_FORCE_INLINE_ static boolean reset_and_return_result() noexcept
	{
		//auto l_table_current_size = tl_s_ref_table.size();

		//if ((l_table_current_size == tl_s_ref_table_recycler.size()) && (l_table_current_size != 0))
		//{
		//	tl_s_ref_table.clear();
		//	tl_s_ref_table_recycler.clear();

		//	tl_s_ref_table.shrink_to_fit();
		//	tl_s_ref_table_recycler.shrink_to_fit();
		//	return _FE_SUCCESS_;
		//}

		return _FE_FAILED_;
	}
};


template<typename T>
class exclusive_ptr_base : public smart_ptr_variants_base
{
public:
	using base_type = smart_ptr_variants_base;

	using ref_table_type = typename base_type::ref_table_type;
	using ref_table_value_type = typename base_type::ref_table_value_type;

	using ref_table_recycler_type = typename base_type::ref_table_recycler_type;
	using ref_table_key_type = typename ref_table_type::pointer;

	using element_type = typename std::remove_all_extents<T>::type;
	using pointer = element_type*;

protected:
	pointer m_smart_ptr;
	ref_table_key_type m_key;

	_CONSTEXPR20_ exclusive_ptr_base() noexcept : m_smart_ptr(), m_key(base_type::invalid_key_value) {}
	_CONSTEXPR20_ exclusive_ptr_base(pointer value_p) noexcept : m_smart_ptr(value_p), m_key(base_type::invalid_key_value) {}
	_CONSTEXPR20_ ~exclusive_ptr_base() noexcept = default;

public:
	_FORCE_INLINE_ pointer get() const noexcept
	{
		return this->m_smart_ptr;
	}

	_FORCE_INLINE_ ref_table_key_type get_key() const noexcept
	{
		return this->m_key;
	}

protected:
	_FORCE_INLINE_ static ref_table_key_type __register_property_address(ref_table_value_type value_p) noexcept
	{
		if (base_type::tl_s_ref_table_recycler.empty() == false)
		{
			ref_table_key_type l_reused_key = base_type::tl_s_ref_table_recycler.back();
			base_type::tl_s_ref_table_recycler.pop_back();
			*l_reused_key = value_p;
			return l_reused_key;
		}

		return &(base_type::tl_s_ref_table.emplace_back(value_p));
	}

	_FORCE_INLINE_ static void __unregister_property_address(ref_table_key_type key_p) noexcept
	{
		FE_ASSERT(key_p == nullptr, "${%s@0}: ${%s@1} is an invalid key.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(key_p));
		*key_p = nullptr;
		
		//FE_LOG("Unregistered a key value: ${%lu@0}.\n\n\n", &key_p);

		base_type::tl_s_ref_table_recycler.emplace_back(key_p);

//#ifdef _DEBUG_
//
//		auto l_begin = base_type::tl_s_ref_table_recycler.begin();
//		auto l_end = base_type::tl_s_ref_table_recycler.end();
//
//		while(l_begin != l_end)
//		{
//			FE_ASSERT(base_type::tl_s_ref_table[*l_begin] != nullptr, "Assertion Failed: unregistered keys' value must be nullptr.");
//			
//			for (auto it = l_begin + 1; it != l_end; ++it)
//			{
//				FE_ASSERT(*l_begin == *it, "Assertion Failed: Keys are always unique.");
//			}
//			++l_begin;
//		}
//#endif
	}
};


class ptr_base : public smart_ptr_variants_base
{
public:
	using base_type = smart_ptr_variants_base;

	using ref_table_type = typename base_type::ref_table_type;
	using ref_table_value_type = typename base_type::ref_table_value_type;

	using ref_table_key_type = typename ref_table_type::pointer;

protected:
	_FORCE_INLINE_ static void* __get_property_address(typename ref_table_key_type key_p) noexcept
	{
		if (key_p == base_type::invalid_key_value)
		{
			return nullptr;
		}

		return *key_p;
	}
};


END_NAMESPACE
#endif