//#include <gtest/gtest.h>
//#include <benchmark/benchmark.h>
//
//// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
//#include <FE/farray.hxx>
//#include <FE/fqueue.hxx>
//#include <FE/fstack.hxx>
//
//// std
//#include <stack>
//#include <string>
//#include <queue>
//
//
//
//
//TEST(queue, trivially_constructible_and_destructible)
//{
//	FE::fqueue<int, 10> l_another_queue = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//	
//	FE::fqueue<int, 10> l_queue(l_another_queue.cbegin(), l_another_queue.cend());
//	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_begin = l_queue.cbegin();
//	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_end = l_queue.cend();
//
//	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rbegin = l_queue.crbegin();
//	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rend = l_queue.crend();
//
//	{
//		FE::fqueue<int, 10> l_copied_queue = l_queue;
//		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
//		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_end = l_copied_queue.cend();
//
//		int l_expected_value = 0;
//		while(l_const_queue_iterator_begin != l_const_queue_iterator_end)
//		{
//			EXPECT_EQ(*l_const_queue_iterator_begin, l_expected_value++);
//			EXPECT_EQ(*l_const_queue_iterator_begin, *l_const_copied_queue_iterator_begin);
//			++l_const_queue_iterator_begin;
//			++l_const_copied_queue_iterator_begin;
//		}
//
//
//		l_const_queue_iterator_begin = l_queue.cbegin();
//		l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
//
//
//		FE::fqueue<int, 10> l_moved_queue = std::move(l_copied_queue);
//		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_begin = l_moved_queue.cbegin();
//		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_end = l_moved_queue.cend();
//
//		while (l_const_moved_queue_iterator_begin != l_const_moved_queue_iterator_end)
//		{
//			EXPECT_EQ(*l_const_moved_queue_iterator_begin, *l_const_queue_iterator_begin);
//			++l_const_moved_queue_iterator_begin;
//			++l_const_queue_iterator_begin;
//
//			EXPECT_EQ(*l_const_copied_queue_iterator_begin, 0);
//		}
//
//		l_copied_queue = { 5, 4, 3, 2, 1 };
//		l_copied_queue.swap(l_queue);
//	}
//}
//
//
//TEST(queue, multiple_assignments_)
//{
//	FE::fqueue<std::string, 5> l_queue = { "hi", ",", " ", "world", "." };
//	l_queue = { "hello", ",", " ", "C++", "." };
//	EXPECT_EQ(strcmp((l_queue.cbegin())->c_str(), "hello"), 0);
//	EXPECT_EQ(strcmp((l_queue.cend() - 1)->c_str(), "."), 0);
//	{
//		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
//		auto l_initializer_list_cbegin = l_initializer_list.begin();
//		EXPECT_EQ(strcmp((l_queue.cend() - 1)->c_str(), "."), 0);
//		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
//		{
//			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
//			++l_initializer_list_cbegin;
//		}
//	}
//
//
//	FE::fqueue<std::string, 5> l_another_queue = { "hi", ",", " ", "world", "." };
//	l_queue = std::move(l_another_queue);
//	{
//		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
//		auto l_initializer_list_cbegin = l_initializer_list.begin();
//		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
//		{
//			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
//			++l_initializer_list_cbegin;
//		}
//	}
//	l_queue = std::move(l_another_queue);
//}
//
//
//TEST(queue, container_comparison_)
//{
//	FE::fqueue<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
//    EXPECT_EQ(strcmp(l_languages1.front().c_str(), "C++"), 0);
//	FE::fqueue<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
//    EXPECT_EQ(strcmp(l_languages2.back().c_str(), "C"), 0);
//	EXPECT_TRUE(l_languages1 == l_languages2);
//
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C++"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C#"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Python"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Java"), 0);
//    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C
//
//	l_languages1.push("Python");
//	l_languages2.pop();
//	EXPECT_TRUE(l_languages1 != l_languages2);
//}
//
//
//TEST(queue, pop_all)
//{
//	FE::fqueue<std::string, 3> l_languages;
//	l_languages.push("Java");
//	l_languages.push("Python");
//	l_languages.push("C#");
//
//	EXPECT_TRUE(l_languages.pop() == "Java");
//	EXPECT_TRUE(l_languages.pop() == "Python");
//	l_languages.push("C++");
//
//	EXPECT_TRUE(l_languages.front() == "C#");
//	EXPECT_TRUE(l_languages.back() == "C++");
//
//	l_languages.pop_all();
//	EXPECT_TRUE(l_languages.is_empty());
//}
//
//
//
//
//void FE_queue_push_and_pop(benchmark::State& state_p) noexcept
//{
//	FE::fqueue<const char*, 64> l_queue;
//
//	for (auto _ : state_p)
//	{
//		l_queue.push("Pizza");
//		l_queue.pop();
//	}
//}
//BENCHMARK(FE_queue_push_and_pop);
//
//
//void std_queue_push_and_pop(benchmark::State& state_p) noexcept
//{
//	std::queue<const char*> l_queue;
//
//	for (auto _ : state_p)
//	{
//		l_queue.push("Pizza");
//		l_queue.pop();
//	}
//}
//BENCHMARK(std_queue_push_and_pop);
//
//
//
//
//
//
//
//
//
//
//
//
//
//TEST(fstack, trivially_constructible_and_destructible)
//{
//	FE::fstack<int, 10> l_another_stack = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//	FE::fstack<int, 10> l_stack(l_another_stack.cbegin(), l_another_stack.cend());
//	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
//	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_end = l_stack.cend();
//
//	{
//		FE::fstack<int, 10> l_copied_stack = l_stack;
//		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
//		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();
//		
//		while(l_const_stack_iterator_begin != l_const_stack_iterator_end)
//		{
//			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
//			++l_const_stack_iterator_begin;
//			++l_const_copied_stack_iterator_begin;
//		}
//
//
//		l_const_stack_iterator_begin = l_stack.cbegin();
//		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
//
//
//		FE::fstack<int, 10> l_moved_stack = std::move(l_copied_stack);
//		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
//		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();
//
//		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
//		{
//			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
//			++l_const_moved_stack_iterator_begin;
//			++l_const_stack_iterator_begin;
//
//			EXPECT_EQ(*l_const_copied_stack_iterator_begin, 0);
//		}
//
//		l_copied_stack = { 5, 4, 3, 2, 1 };
//		l_copied_stack.swap(l_stack);
//	}
//}
//
//TEST(fstack, constructible_and_destructible)
//{
//	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "."};
//	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
//	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_end = l_stack.cend();
//
//	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rbegin = l_stack.crbegin();
//	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rend = l_stack.crend();
//
//	{
//		FE::fstack<std::string, 5> l_copied_stack = l_stack;
//		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
//		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();
//
//		while (l_const_stack_iterator_begin != l_const_stack_iterator_end)
//		{
//			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
//			++l_const_stack_iterator_begin;
//			++l_const_copied_stack_iterator_begin;
//		}
//
//
//		l_const_stack_iterator_begin = l_stack.cbegin();
//		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
//
//
//		FE::fstack<std::string, 5> l_moved_stack = std::move(l_copied_stack);
//		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
//		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();
//
//		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
//		{
//			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
//			++l_const_moved_stack_iterator_begin;
//			++l_const_stack_iterator_begin;
//
//			EXPECT_EQ(*l_const_copied_stack_iterator_begin, "\0");
//		}
//
//		l_copied_stack = { "hello", ",", " ", "C++", "."};
//		l_copied_stack.swap(l_stack);
//	}
//}
//
//TEST(fstack, multiple_assignments_)
//{
//	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "." };
//	l_stack = { "hello", ",", " ", "C++", "." };
//	EXPECT_EQ(strcmp(l_stack.cbegin()->c_str(), "hello"), 0);
//	EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
//	{
//		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
//		auto l_initializer_list_cbegin = l_initializer_list.begin();
//		EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
//		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
//		{
//			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
//			++l_initializer_list_cbegin;
//		}
//	}
//
//
//	FE::fstack<std::string, 5> l_another_stack = { "hi", ",", " ", "world", "." };
//	l_stack = std::move(l_another_stack);
//	{
//		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
//		auto l_initializer_list_cbegin = l_initializer_list.begin();
//		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
//		{
//			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
//			++l_initializer_list_cbegin;
//		}
//	}
//	l_stack = std::move(l_another_stack);
//}
//
//TEST(fstack, container_comparison_)
//{
//	FE::fstack<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
//    EXPECT_EQ(strcmp(l_languages1.top().c_str(), "C"), 0);
//	FE::fstack<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
//    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
//	EXPECT_TRUE(l_languages1 == l_languages2);
//
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Java"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Python"), 0);
//    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C#"), 0);
//    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
//    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C
//
//	l_languages1.push("Python");
//	l_languages2.pop();
//	EXPECT_TRUE(l_languages1 != l_languages2); // Python is not equivalent to Java
//}
//
//
//
//
//void FE_stack_push_and_pop(benchmark::State& state_p) noexcept
//{
//	FE::fstack<const char*, 64> l_stack;
//
//	for (auto _ : state_p)
//	{
//		l_stack.push("Pizza");
//		l_stack.pop();
//	}
//}
//BENCHMARK(FE_stack_push_and_pop);
//
//
//void std_stack_push_and_pop(benchmark::State& state_p) noexcept
//{
//	std::stack<const char*, std::vector<const char*>> l_stack;
//
//	for (auto _ : state_p)
//	{
//		l_stack.push("Pizza");
//		l_stack.pop();
//	}
//}
//BENCHMARK(std_stack_push_and_pop);
//
//
//
//
//TEST(farray, all)
//{
//	{
//		FE::farray<const char*, 64> l_array;
//		l_array.push_back("Pizza");
//		
//		EXPECT_EQ(strcmp(l_array.back(), "Pizza"), 0);
//		l_array.pop_back();
//	}
//}