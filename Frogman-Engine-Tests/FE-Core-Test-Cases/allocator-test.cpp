#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/miscellaneous/google_test_extension.h>
#include <FE/core/allocator.hxx>




struct test {};

TEST(allocator, aligned_allocator)
{
	//EXPECT_EQ(FE::heap_memory_tracker<test>::query_type_data()._thread_local_total_bytes_by_type, 0);

	{
		FE::aligned_allocator<test> l_allocator;
		test* l_ptc = l_allocator.allocate(100);
		//EXPECT_EQ(FE::heap_memory_tracker<test>::query_type_data()._thread_local_total_bytes_by_type, sizeof(test) * 100);
		l_ptc = l_allocator.reallocate(l_ptc, 100, 10);
		l_allocator.deallocate(l_ptc, 10);
	}

	//EXPECT_EQ(FE::heap_memory_tracker<test>::query_type_data()._thread_local_total_bytes_by_type, 0);
}



