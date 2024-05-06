#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/containers/stack.hxx>
#include <FE/core/fstring.hxx>
#include <string>
#include <stack>
#pragma warning (disable: 26800)




TEST(fstack, trivially_constructible_and_destructible)
{
	FE::fstack<int, 10> l_another_stack = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	FE::fstack<int, 10> l_stack(l_another_stack.cbegin(), l_another_stack.cend());
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	{
		FE::fstack<int, 10> l_copied_stack = l_stack;
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();
		
		while(l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<int, 10> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, 0);
		}

		l_copied_stack = { 5, 4, 3, 2, 1 };
		l_copied_stack.swap(l_stack);
	}
}

TEST(fstack, constructible_and_destructible)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "."};
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rbegin = l_stack.crbegin();
	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rend = l_stack.crend();

	{
		FE::fstack<std::string, 5> l_copied_stack = l_stack;
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();

		while (l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<std::string, 5> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, "\0");
		}

		l_copied_stack = { "hello", ",", " ", "C++", "."};
		l_copied_stack.swap(l_stack);
	}
}


TEST(fstack, push_pop)
{
	{
		FE::fstack<int, 5> l_integral_stack;
		for (int i = 0; i < l_integral_stack.capacity(); ++i)
		{
			l_integral_stack.push(i);
		}

		std::size_t l_expected_value = l_integral_stack.capacity();
		for (int i = 0; i < l_integral_stack.capacity(); ++i)
		{
			--l_expected_value;
			EXPECT_EQ(l_expected_value, l_integral_stack.pop());
		}

		for (int i = 0; i < l_integral_stack.capacity(); ++i)
		{
			l_integral_stack.push(i);
		}

		l_integral_stack.pop_all();

		FE::fstack<int, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_stack.crbegin();

		while (l_const_iterator_begin != l_integral_stack.crend())
		{
			EXPECT_EQ(*l_const_iterator_begin, 0);
			++l_const_iterator_begin;
		}
	}


	std::initializer_list<std::string> l_initializer_list = { "hello", ",", " ", "C++", "." };
	FE::fstack<std::string, 5> l_cstring_stack = { ".", "C++", " ", ",", "hello" };
	FE::fstack<FE::fstring<10>, 5> l_fstring_stack;

	FE::fstack<std::string, 5>::const_iterator L_cstring_const_iterator_begin = l_cstring_stack.cbegin();
	FE::fstack<FE::fstring<10>, 5>::const_iterator L_fstring_const_iterator_begin = l_fstring_stack.cbegin();

	EXPECT_EQ(l_cstring_stack.capacity(), l_fstring_stack.capacity());

	for (int i = 0; i < l_cstring_stack.capacity(); ++i)
	{
		l_fstring_stack.push(l_cstring_stack.pop().c_str());
		
		EXPECT_TRUE(FE::algorithm::string::compare(L_fstring_const_iterator_begin[i].c_str(), l_initializer_list.begin()[i].c_str()));
	}
}

TEST(fstack, multiple_assignments_)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "." };
	l_stack = { "hello", ",", " ", "C++", "." };
	EXPECT_TRUE(FE::algorithm::string::compare((l_stack.cbegin())->c_str(), "hello"));
	EXPECT_TRUE(FE::algorithm::string::compare((l_stack.cend() - 1)->c_str(), "."));
	{
		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		EXPECT_TRUE(FE::algorithm::string::compare((l_stack.cend() - 1)->c_str(), "."));
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_TRUE(FE::algorithm::string::compare<FE::var::character>(iterator->c_str(), *l_initializer_list_cbegin));
			++l_initializer_list_cbegin;
		}
	}


	FE::fstack<std::string, 5> l_another_stack = { "hi", ",", " ", "world", "." };
	l_stack = std::move(l_another_stack);
	{
		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_TRUE(FE::algorithm::string::compare<FE::var::character>(iterator->c_str(), *l_initializer_list_cbegin));
			++l_initializer_list_cbegin;
		}
	}
	l_stack = std::move(l_another_stack);
}

TEST(fstack, container_comparison_)
{
	FE::fstack<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.top().c_str(), "C"));
	FE::fstack<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages2.top().c_str(), "C"));
	EXPECT_TRUE(l_languages1 == l_languages2);

    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "C"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "Java"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "Python"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages1.pop().c_str(), "C#"));
    EXPECT_TRUE(FE::algorithm::string::compare(l_languages2.top().c_str(), "C"));
    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C

	l_languages1.push("Python");
	l_languages2.pop();
	EXPECT_TRUE(l_languages1 != l_languages2); // Python is not equivalent to Java
}




void FE_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fstack<const char*, 64> l_stack;
	benchmark::DoNotOptimize(l_stack);

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(FE_stack_push_and_pop);


void std_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	std::stack<const char*, std::vector<const char*>> l_stack;
	benchmark::DoNotOptimize(l_stack);

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(std_stack_push_and_pop);