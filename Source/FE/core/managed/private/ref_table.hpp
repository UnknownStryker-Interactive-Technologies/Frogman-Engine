#ifndef _FE_CORE_MANAGED_REF_TABLE_HPP_
#define _FE_CORE_MANAGED_REF_TABLE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool.hxx>




// Forward declare all the memory management automation classes 
namespace FE
{
	template<typename T, class Allocator>
	class exclusive_ptr;

	template<typename T>
	class ptr;

	template<typename T>
	class trackable;

	template<typename T>
	class ref;

}




BEGIN_NAMESPACE(FE::internal::managed)


template<typename T>
struct ref_block
{
	T* _address = nullptr;
	var::count_t _ref_count = 0;
};


class ref_table 
{
	template<typename T, class Allocator>
	friend class ::FE::exclusive_ptr;

	template<typename T>
	friend class ::FE::ptr;

	template<typename T>
	friend class ::FE::trackable;

	template<typename T>
	friend class ::FE::ref;
	
public:
	static constexpr auto ref_table_page_capacity = 10000;

private:
	thread_local static FE::block_pool<sizeof(ref_block<void>), ref_table_page_capacity> tl_s_ref_block_pool;
};


END_NAMESPACE
#endif