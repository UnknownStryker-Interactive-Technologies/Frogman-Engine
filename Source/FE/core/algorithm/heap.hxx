#ifndef _FE_CORE_HEAP_HXX_
#define _FE_CORE_HEAP_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/allocator.hxx>
#include <FE/core/pair.hxx>
#include <algorithm>
#include <memory_resource>
#include <memory>
#include <vector>




BEGIN_NAMESPACE(FE)


struct max_heap
{
	template<typename T>
	_FORCE_INLINE_ boolean operator()(const T& first_p, const T& second_p) const noexcept
	{
		return first_p._second < second_p._second;
	}
};

struct min_heap
{
	template<typename T>
	_FORCE_INLINE_ boolean operator()(const T& first_p, const T& second_p) const noexcept
	{
		return first_p._second > second_p._second;
	}
};


enum struct HEAP_TYPE : boolean
{
	_MIN_HEAP = false,
	_MAX_HEAP = true
};


template<typename T, HEAP_TYPE HeapType>
index_t binary_search_contiguous_binary_heap(const T* const contiguous_binary_heap_p, size_t capacity_p, const T& target_p) noexcept
{
	var::index_t l_index = 0;
	while (l_index < capacity_p)
	{
		if (contiguous_binary_heap_p[l_index] == target_p)
		{
			return l_index;
		}

		FE::pair<var::index_t, T> l_left_child;
		l_left_child._first = (2 * l_index) + 1;
		l_left_child._second = contiguous_binary_heap_p[l_left_child._first];

		FE::pair<var::index_t, T> l_right_child;
		l_right_child._first = (2 * l_index) + 2;
		l_right_child._second = contiguous_binary_heap_p[l_right_child._first];

		FE::pair<var::index_t, T> l_closest;
		if constexpr (HeapType == HEAP_TYPE::_MAX_HEAP)
		{
			l_closest = algorithm::utility::select<max_heap>(l_left_child, l_right_child);
		}
		else if constexpr (HeapType == HEAP_TYPE::_MIN_HEAP)
		{
			l_closest = select<min_heap>(l_left_child, l_right_child);
		}
		l_index = l_closest._first;
	}

	return FE::max_value<index_t>;
}

END_NAMESPACE
#endif