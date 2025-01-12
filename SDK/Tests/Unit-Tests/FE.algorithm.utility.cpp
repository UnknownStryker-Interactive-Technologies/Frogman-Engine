#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <unordered_map>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/algorithm/utility.hxx>

// std::make_heap, std::push_heap, and std::pop_heap
#include <algorithm>
#include <array>




TEST(FE_algorithm_utility, binary_search)
{
	std::array<int, 10> l_array = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	for (int i = 1; i <= l_array.size(); ++i)
	{
		std::array<int, 10>::iterator l_result = FE::algorithm::utility::binary_search(l_array.begin(), l_array.end(), i);
		EXPECT_EQ(*l_result, i);
	}
}
void frogman_binary_search_benchmark(benchmark::State& state_p)
{
	std::array<int, 20> l_array = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (auto _ : state_p)
	{
		for (int i = 1; i <= l_array.size(); ++i)
		{
			EXPECT_EQ( *(FE::algorithm::utility::binary_search(l_array.begin(), l_array.end(), i)), i );
		}
	}
}
BENCHMARK(frogman_binary_search_benchmark);

void std_binary_search_benchmark(benchmark::State& state_p)
{
	std::array<int, 20> l_array = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (auto _ : state_p)
	{
		for (int i = 1; i <= l_array.size(); ++i)
		{
			EXPECT_EQ( *(std::lower_bound(l_array.begin(), l_array.end(), i)), i );
		}
	}
}
BENCHMARK(std_binary_search_benchmark);




TEST(FE_algorithm_utility, binary_insertion_sort)
{
	std::array<int, 32> l_array = 
	{ 
		42, 17, 93, 8, 55, 23, 76, 34, 
		89, 12, 3, 66, 28, 51, 78, 95, 
		1, 37, 81, 40, 61, 11, 25, 99, 
		2, 70, 47, 84, 19, 5, 91, 63
	};
	FE::algorithm::utility::binary_insertion_sort(l_array.begin(), l_array.end());

	for (int i = 0; i < l_array.size() - 1; ++i)
	{
		EXPECT_LE(l_array[i], l_array[i + 1]);
	}
}
void small_array_binary_insertion_sort_benchmark(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::array<std::size_t, 16> l_array =
		{
			42, 17, 93, 8, 55, 23, 76, 34,
			89, 12, 3, 66, 28, 51, 78, 95
		};

		FE::algorithm::utility::binary_insertion_sort(l_array.begin(), l_array.end());
	}
}
BENCHMARK(small_array_binary_insertion_sort_benchmark);

void small_array_std_sort_benchmark(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::array<std::size_t, 16> l_array =
		{
			42, 17, 93, 8, 55, 23, 76, 34,
			89, 12, 3, 66, 28, 51, 78, 95
		};
		std::sort(l_array.begin(), l_array.end());
	}
}
BENCHMARK(small_array_std_sort_benchmark);




TEST(FE_algorithm_utility, exclude)
{
	FE::int32 l_value_to_exclude = 0;
	std::array<int, 10> l_array = { 1, 2, 0, 0, 5, 0, 7, 8, 0, 10 };

	FE::pair<std::array<int, 10>::iterator, std::array<int, 10>::iterator> l_result = FE::algorithm::utility::exclude<FE::algorithm::utility::IsolationVector::_Right>(l_array.begin(), l_array.end(), l_value_to_exclude);

	while (l_result._first != l_result._second)
	{
		EXPECT_NE(*(l_result._first), l_value_to_exclude);
		++(l_result._first);
	}
}

TEST(FE_algorithm_utility, cherry_pick)
{
	FE::int32 l_value_to_exclude = 0;
	std::array<int, 10> l_array = { 1, 2, 0, 0, 5, 0, 7, 8, 0, 10 };

	FE::pair<std::array<int, 10>::iterator, std::array<int, 10>::iterator> l_result = FE::algorithm::utility::cherry_pick<FE::algorithm::utility::IsolationVector::_Left>(l_array.begin(), l_array.end(), l_value_to_exclude);

	while (l_result._first != l_result._second)
	{
		EXPECT_NE(*(l_result._first), l_value_to_exclude);
		++(l_result._first);
	}
}

// count_int_digit_length
TEST(FE_algorithm_utility, count_int_digit_length)
{
	int l_int = 10000;
	EXPECT_EQ(FE::algorithm::utility::count_int_digit_length(l_int), 5);

	EXPECT_EQ(FE::algorithm::utility::count_int_digit_length(0), 1);
}

// count_uint_digit_length
TEST(FE_algorithm_utility, count_uint_digit_length)
{
	var::uint64 l_int = 10000;
	EXPECT_EQ(FE::algorithm::utility::count_uint_digit_length(l_int), 5);

	EXPECT_EQ(FE::algorithm::utility::count_uint_digit_length(0), 1);
}

// string_to_uint
TEST(FE_algorithm_utility, string_to_uint)
{
	FE::algorithm::utility::uint_info l_result = FE::algorithm::utility::string_to_uint("1024MB");
	EXPECT_EQ(1024, l_result._value);
	EXPECT_EQ(4, l_result._digit_length);

	char l_string[10] = "8102023";
	l_result = FE::algorithm::utility::string_to_uint(l_string);
	EXPECT_EQ(8102023, l_result._value);
	EXPECT_EQ(7, l_result._digit_length);
}

// string_to_int
TEST(FE_algorithm_utility, string_to_int)
{
	FE::algorithm::utility::int_info l_result = FE::algorithm::utility::string_to_int("-999HP");
	EXPECT_EQ(-999, l_result._value);
	EXPECT_EQ(3, l_result._digit_length);

	char l_string[10] = "-5959";
	l_result = FE::algorithm::utility::string_to_int(l_string);
	EXPECT_EQ(-5959, l_result._value);
	EXPECT_EQ(4, l_result._digit_length);
}

// int_to_string
TEST(FE_algorithm_utility, int_to_string)
{
	int l_int = -10000;
	char l_buffer[10] = "\0";
	FE::algorithm::utility::int_to_string<char>(l_buffer, 10, l_int);
	EXPECT_STREQ(l_buffer, "-10000");

	memset(l_buffer, 0, 10);

	l_int = 10000;
	FE::algorithm::utility::int_to_string<char>(l_buffer, 10, l_int);
	EXPECT_STREQ(l_buffer, "10000");
}

// uint_to_string
TEST(FE_algorithm_utility, uint_to_string)
{
	var::uint64 l_int = 65165684;
	char l_buffer[10] = "\0";
	FE::algorithm::utility::uint_to_string<char>(l_buffer, 10, l_int);
	EXPECT_STREQ(l_buffer, "65165684");

	memset(l_buffer, 0, 10);

	l_int = 45465;
	FE::algorithm::utility::uint_to_string<char>(l_buffer, 10, l_int);
	EXPECT_STREQ(l_buffer, "45465");
}

// float_to_string
TEST(FE_algorithm_utility, float_to_string)
{
	char l_buffer[30] = "\0";
	var::float32 l_pi = 3.14159f;
	FE::algorithm::utility::float_to_string(l_buffer, sizeof(l_buffer), l_pi);
	std::memset(l_buffer + 7, 0, 4 * 1);

	EXPECT_STREQ(l_buffer, "3.14159");
}

// string_to_float
TEST(FE_algorithm_utility, string_to_float)
{
	FE::algorithm::utility::real_info l_result = FE::algorithm::utility::string_to_float("3.14159265f");
	EXPECT_EQ(1, l_result._int_digit_length);
	EXPECT_EQ(8, l_result._floating_point_length);
	EXPECT_EQ(10, l_result._total_length);

	EXPECT_TRUE
	(
		[&]() -> bool
		{
			if (l_result._value >= 3.140 && l_result._value <= 3.145)
			{
				return true;
			}

			return false;
		}()
	);

	l_result = FE::algorithm::utility::string_to_float("-3.14159265f");
	EXPECT_EQ(1, l_result._int_digit_length);
	EXPECT_EQ(8, l_result._floating_point_length);
	EXPECT_EQ(10, l_result._total_length);

	EXPECT_TRUE
	(
		[&]() -> bool
		{
			if (l_result._value <= -3.140 && l_result._value >= -3.145)
			{
				return true;
			}

			return false;
		}()
	);
}

// string_to_boolean
TEST(FE_algorithm_utility, string_to_boolean)
{
	EXPECT_TRUE(FE::algorithm::utility::string_to_boolean("true"));
	EXPECT_FALSE(FE::algorithm::utility::string_to_boolean("false"));
}

// boolean_to_string
TEST(FE_algorithm_utility, boolean_to_string)
{
	const char* l_buffer = nullptr;
	l_buffer = FE::algorithm::utility::boolean_to_string<char>(true);
	EXPECT_STREQ(l_buffer, "true");

	l_buffer = FE::algorithm::utility::boolean_to_string<char>(false);
	EXPECT_STREQ(l_buffer, "false");
}
