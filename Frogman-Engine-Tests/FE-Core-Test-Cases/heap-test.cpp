#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <FE/core/algorithm/heap.hxx>
#include <FE/core/containers/queue.hxx>




TEST(array_based_max_heap, binary_search)
{
	using namespace FE;

	FE::fpriority_queue<var::int64, 64, std::less<var::int64>> l_heap_based_fpriority_queue;
	l_heap_based_fpriority_queue.push(100);
	l_heap_based_fpriority_queue.push(19);
	l_heap_based_fpriority_queue.push(36);
	l_heap_based_fpriority_queue.push(17);
	l_heap_based_fpriority_queue.push(3);
	l_heap_based_fpriority_queue.push(25);
	l_heap_based_fpriority_queue.push(1);
	l_heap_based_fpriority_queue.push(2);
	l_heap_based_fpriority_queue.push(7);

	int64 l_target_value = 3;
	index_t l_target_location = binary_search_contiguous_binary_heap<var::int64, HEAP_TYPE::_MAX_HEAP>(l_heap_based_fpriority_queue.cbegin().operator->(), l_heap_based_fpriority_queue.size(), l_target_value);

	EXPECT_EQ(l_heap_based_fpriority_queue.get_container()[l_target_location], l_target_value);
}