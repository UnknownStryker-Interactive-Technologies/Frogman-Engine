#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/containers/queue.hxx>
#include <FE/core/fstring.hxx>
#include <string>
#include <queue>
#pragma warning (disable: 26800)




TEST(queue, trivially_constructible_and_destructible)
{
	FE::fqueue<int, 10> l_another_queue = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	
	FE::fqueue<int, 10> l_queue(l_another_queue.cbegin(), l_another_queue.cend());
	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_begin = l_queue.cbegin();
	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_end = l_queue.cend();

	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rbegin = l_queue.crbegin();
	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rend = l_queue.crend();

	{
		FE::fqueue<int, 10> l_copied_queue = l_queue;
		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_end = l_copied_queue.cend();

		int l_expected_value = 0;
		while(l_const_queue_iterator_begin != l_const_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_queue_iterator_begin, l_expected_value++);
			EXPECT_EQ(*l_const_queue_iterator_begin, *l_const_copied_queue_iterator_begin);
			++l_const_queue_iterator_begin;
			++l_const_copied_queue_iterator_begin;
		}


		l_const_queue_iterator_begin = l_queue.cbegin();
		l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();


		FE::fqueue<int, 10> l_moved_queue = std::move(l_copied_queue);
		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_begin = l_moved_queue.cbegin();
		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_end = l_moved_queue.cend();

		while (l_const_moved_queue_iterator_begin != l_const_moved_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_queue_iterator_begin, *l_const_queue_iterator_begin);
			++l_const_moved_queue_iterator_begin;
			++l_const_queue_iterator_begin;

			EXPECT_EQ(*l_const_copied_queue_iterator_begin, 0);
		}

		l_copied_queue = { 5, 4, 3, 2, 1 };
		l_copied_queue.swap(l_queue);
	}
}

TEST(queue, constructible_and_destructible)
{
	FE::fqueue<std::string, 5> l_queue = { "hi", ",", " ", "world", "."};
	FE::fqueue<std::string, 5>::const_iterator l_const_queue_iterator_begin = l_queue.cbegin();
	FE::fqueue<std::string, 5>::const_iterator l_const_queue_iterator_end = l_queue.cend();

	FE::fqueue<std::string, 5>::const_reverse_iterator l_const_queue_iterator_rbegin = l_queue.crbegin();
	FE::fqueue<std::string, 5>::const_reverse_iterator l_const_queue_iterator_rend = l_queue.crend();

	{
		FE::fqueue<std::string, 5> l_copied_queue = l_queue;
		FE::fqueue<std::string, 5>::const_iterator l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
		FE::fqueue<std::string, 5>::const_iterator l_const_copied_queue_iterator_end = l_copied_queue.cend();

		while (l_const_queue_iterator_begin != l_const_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_queue_iterator_begin, *l_const_copied_queue_iterator_begin);
			++l_const_queue_iterator_begin;
			++l_const_copied_queue_iterator_begin;
		}


		l_const_queue_iterator_begin = l_queue.cbegin();
		l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();


		FE::fqueue<std::string, 5> l_moved_queue = std::move(l_copied_queue);
		FE::fqueue<std::string, 5>::const_iterator l_const_moved_queue_iterator_begin = l_moved_queue.cbegin();
		FE::fqueue<std::string, 5>::const_iterator l_const_moved_queue_iterator_end = l_moved_queue.cend();

		while (l_const_moved_queue_iterator_begin != l_const_moved_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_queue_iterator_begin, *l_const_queue_iterator_begin);
			++l_const_moved_queue_iterator_begin;
			++l_const_queue_iterator_begin;

			EXPECT_EQ(*l_const_copied_queue_iterator_begin, "\0");
		}

		l_copied_queue = { "hello", ",", " ", "C++", "."};
		l_copied_queue.swap(l_queue);
	}
}


TEST(queue, push_pop)
{
	{
		FE::fqueue<int, 5> l_integral_queue;
		for (int i = 0; i < l_integral_queue.capacity(); ++i)
		{
			l_integral_queue.push(i);
		}

		std::size_t l_expected_value = 0;
		for (int i = 0; i < l_integral_queue.capacity() >> 1; ++i)
		{
			EXPECT_EQ(l_expected_value, l_integral_queue.pop());
			++l_expected_value;
		}

		for (int i = 0; i < l_integral_queue.capacity() >> 1; ++i)
		{
			l_integral_queue.push(i);
		}

		l_integral_queue.pop_all();

		FE::fqueue<int, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_queue.crbegin();
	}


	std::initializer_list<std::string> l_initializer_list = { "hello", ",", " ", "C++", "." };
	FE::fqueue<std::string, 5> l_cstring_queue = { "hello", ",", " ", "C++", "." };
	FE::fqueue<FE::fstring<10>, 5> l_fstring_queue;

	FE::fqueue<std::string, 5>::const_iterator L_cstring_const_iterator_begin = l_cstring_queue.cbegin();
	FE::fqueue<FE::fstring<10>, 5>::const_iterator L_fstring_const_iterator_begin = l_fstring_queue.cbegin();

	EXPECT_EQ(l_cstring_queue.capacity(), l_fstring_queue.capacity());

	for (int i = 0; i < l_cstring_queue.capacity(); ++i)
	{
		l_fstring_queue.push(l_cstring_queue.pop().c_str());

		EXPECT_TRUE(FE::algorithm::string::compare(L_fstring_const_iterator_begin[i].c_str(), l_initializer_list.begin()[i].c_str()));
	}
}

TEST(queue, multiple_assignments_)
{
	FE::fqueue<std::string, 5> l_queue = { "hi", ",", " ", "world", "." };
	l_queue = { "hello", ",", " ", "C++", "." };
	EXPECT_TRUE(FE::algorithm::string::compare((l_queue.cbegin())->c_str(), "hello"));
	EXPECT_TRUE(FE::algorithm::string::compare((l_queue.cend() - 1)->c_str(), "."));
	{
		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		EXPECT_TRUE(FE::algorithm::string::compare((l_queue.cend() - 1)->c_str(), "."));
		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
		{
			EXPECT_TRUE(FE::algorithm::string::compare<FE::var::character>(iterator->c_str(), *l_initializer_list_cbegin));
			++l_initializer_list_cbegin;
		}
	}


	FE::fqueue<std::string, 5> l_another_queue = { "hi", ",", " ", "world", "." };
	l_queue = std::move(l_another_queue);
	{
		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
		{
			EXPECT_TRUE(FE::algorithm::string::compare<FE::var::character>(iterator->c_str(), *l_initializer_list_cbegin));
			++l_initializer_list_cbegin;
		}
	}
	l_queue = std::move(l_another_queue);
}

TEST(queue, container_comparison_)
{
	FE::fqueue<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.front().c_str(), "C++"));
	FE::fqueue<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages2.back().c_str(), "C"));
	EXPECT_TRUE(l_languages1 == l_languages2);

    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "C++"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "C#"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "Python"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "Java"));
    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C

	l_languages1.push("Python");
	l_languages2.pop();
	EXPECT_TRUE(l_languages1 != l_languages2);
}


TEST(queue, pop_all)
{
	FE::fqueue<std::string, 3> l_languages;
	l_languages.push("Java");
	l_languages.push("Python");
	l_languages.push("C#");

	EXPECT_TRUE(l_languages.pop() == "Java");
	EXPECT_TRUE(l_languages.pop() == "Python");
	l_languages.push("C++");

	EXPECT_TRUE(l_languages.front() == "C#");
	EXPECT_TRUE(l_languages.back() == "C++");

	l_languages.pop_all();
	EXPECT_TRUE(l_languages.is_empty());
}




void FE_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fqueue<const char*, 64> l_queue;
	benchmark::DoNotOptimize(l_queue);

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(FE_queue_push_and_pop);


void std_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	std::queue<const char*> l_queue;
	benchmark::DoNotOptimize(l_queue);

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(std_queue_push_and_pop);