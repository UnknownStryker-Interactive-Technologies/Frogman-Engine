#ifndef _FE_CORE_MANAGED_REF_TABLE_HPP_
#define _FE_CORE_MANAGED_REF_TABLE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>




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
	T* _end = nullptr;
	var::count_t _ref_count = 0;
};


END_NAMESPACE
#endif