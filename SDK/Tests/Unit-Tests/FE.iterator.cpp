//#include <gtest/gtest.h>
//#include <benchmark/benchmark.h>
//
//
//// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
//#include <FE/iterator.hxx>
//
//
//
//
//
//TEST(iterator_casting, iterator_to_pointer)
//{
//	std::vector<int> l_vector = { 0, 1, 2, 3, 4, 5 };
//
//	int* l_pointer = iterator_cast<int*>(l_vector.begin());
//	EXPECT_EQ(l_pointer, l_vector.begin().operator->());
//}
//
//TEST(iterator_casting, pointer_to_iterator)
//{
//	std::vector<int> l_vector = { 0, 1, 2, 3, 4, 5 };
//
//	int* l_pointer = iterator_cast<int*>(l_vector.begin());
//	EXPECT_EQ(l_pointer, l_vector.begin().operator->());
//
//	FE::iterator<FE::contiguous_iterator<int>> l_iterator = iterator_cast<FE::iterator<FE::contiguous_iterator<int>>>(l_pointer);
//	EXPECT_EQ(l_iterator.operator->(), l_pointer);
//}
//
//TEST(iterator_casting, iterator_to_iterator)
//{
//	std::vector<int> l_vector = { 0, 1, 2, 3, 4, 5 };
//
//	FE::iterator<FE::contiguous_iterator<int>> l_iterator = iterator_cast<FE::iterator<FE::contiguous_iterator<int>>>(l_vector.begin());
//	EXPECT_EQ(l_iterator.operator->(), l_vector.begin().operator->());
//}
//
//TEST(iterator, std_vector_iteration)
//{
//	std::vector<int> l_vector = { 0, 1, 2, 3, 4, 5 };
//	FE::iterator<FE::contiguous_iterator<int>> l_begin_iterator(l_vector.begin().operator->());
//	FE::iterator<FE::contiguous_iterator<int>> l_end_iterator((l_vector.begin().operator->()) + l_vector.size());
//
//	auto l_std_vector_iterator = l_vector.begin();
//	for (auto it = l_begin_iterator; it != l_end_iterator; ++it)
//	{
//		EXPECT_EQ(*l_std_vector_iterator, *it);
//		++l_std_vector_iterator;
//	}
//
//	for (auto it = l_end_iterator - 1; it != l_begin_iterator; --it)
//	{
//		--l_std_vector_iterator;
//		EXPECT_EQ(*l_std_vector_iterator, *it);
//	}
//
//	FE::iterator<FE::contiguous_iterator<int>> l_iterator = l_begin_iterator + 1;
//	EXPECT_EQ(*l_iterator, 1);
//
//	l_iterator += 2;
//	EXPECT_EQ(*l_iterator, 3);
//
//	l_iterator -= 2;
//	EXPECT_EQ(*l_iterator, 1);
//}
//
//
//
//
//TEST(reverse_iterator, std_vector_reverse_iteration)
//{
//	std::vector<int> l_vector = { 0, 1, 2, 3, 4, 5 };
//	FE::reverse_iterator<FE::contiguous_iterator<int>> l_rbegin_iterator(l_vector.rbegin().operator->());
//	FE::reverse_iterator<FE::contiguous_iterator<int>> l_rend_iterator(l_vector.begin().operator->());
//
//	auto l_std_vector_reverse_iterator = l_vector.rbegin();
//	for (auto it = l_rbegin_iterator; it != l_rend_iterator; ++it)
//	{
//		EXPECT_EQ(*l_std_vector_reverse_iterator, *it);
//		++l_std_vector_reverse_iterator;
//	}
//
//	for (auto it = l_rend_iterator - 1; it != l_rbegin_iterator; --it)
//	{
//		--l_std_vector_reverse_iterator;
//		EXPECT_EQ(*l_std_vector_reverse_iterator, *it);
//	}
//
//	FE::reverse_iterator<FE::contiguous_iterator<int>> l_reverse_iterator = l_rbegin_iterator + 1;
//	EXPECT_EQ(*l_reverse_iterator, 4);
//
//	l_reverse_iterator += 2;
//	EXPECT_EQ(*l_reverse_iterator, 2);
//
//	l_reverse_iterator -= 2;
//	EXPECT_EQ(*l_reverse_iterator, 4);
//}
//
//
//void FE_iterator_vector_iteration_test(benchmark::State& state_p) noexcept
//{
//	std::vector<int> l_vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//	for (auto _ : state_p)
//	{
//		FE::iterator<FE::contiguous_iterator<int>> l_iterator = l_vec.begin().operator->();
//		benchmark::DoNotOptimize(l_iterator);
//		FE::const_iterator<FE::contiguous_iterator<int>> l_end = l_vec.data() + l_vec.size();
//		benchmark::DoNotOptimize(l_end);
//		while (l_iterator.operator!=(l_end))
//		{
//			++l_iterator;
//		}
//	}
//}
//
//BENCHMARK(FE_iterator_vector_iteration_test);
//
//
//void std_iterator_vector_iteration_test(benchmark::State& state_p) noexcept
//{
//	std::vector<int> l_vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//	for (auto _ : state_p)
//	{
//		auto l_iterator = l_vec.begin();
//		benchmark::DoNotOptimize(l_iterator);
//		auto l_end = l_vec.cend();
//		benchmark::DoNotOptimize(l_end);
//		while (l_iterator != l_end)
//		{
//			++l_iterator;	
//		}
//	}
//}
//
//BENCHMARK(std_iterator_vector_iteration_test);