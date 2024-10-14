#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <unordered_map>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/algorithm/utility.hxx>
using namespace FE;
using namespace algorithm;
using namespace utility;




/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/
int main(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p)
{
    
    testing::InitGoogleTest(&argc_p, argv_p);

	if (argv_p == nullptr)
	{
		char l_arg0_default[] = "benchmark";
		char* l_args_default = l_arg0_default;
		argc_p = 1;
		argv_p = &l_args_default;
	}
	
    benchmark::Initialize(&argc_p, argv_p);
	FE_ASSERT(benchmark::ReportUnrecognizedArguments(argc_p, argv_p) == true, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
    int32 l_exit_code = RUN_ALL_TESTS();
	std::cerr << "\n\n";
	benchmark::RunSpecifiedBenchmarks();
	std::cerr << "\n\n";
    benchmark::Shutdown();
    return l_exit_code;
}


TEST(count_integral_digit_length, _)
{
	int l_int = 10000;
	EXPECT_EQ(count_int_digit_length(l_int), 5);

	EXPECT_EQ(count_int_digit_length(0), 1);
}


TEST(int_to_string, _)
{
	int l_int = -10000;
	char l_buffer[10] = "\0";
	int_to_string<char>(l_buffer, 10, l_int);
	EXPECT_EQ(0, strcmp(l_buffer, "-10000"));

	memset(l_buffer, 0, 10);

	l_int = 10000;
	int_to_string<char>(l_buffer, 10, l_int);
	EXPECT_EQ(0, strcmp(l_buffer, "10000"));
}

TEST(convert_uint_to_string, _)
{
	var::uint64 l_int = 65165684;
	char l_buffer[10] = "\0";
	uint_to_string<char>(l_buffer, 10, l_int);
	EXPECT_EQ(0, strcmp(l_buffer, "65165684"));

	memset(l_buffer, 0, 10);

	l_int = 45465;
	uint_to_string<char>(l_buffer, 10, l_int);
	EXPECT_EQ(0, strcmp(l_buffer, "45465"));
}

TEST(float_to_string, _)
{
	char l_buffer[30] = "\0";
	var::float32 l_pi = 3.14159f;
	float_to_string(l_buffer, sizeof(l_buffer), l_pi);
	std::memset(l_buffer + 7, 0, 4 * 1);

	EXPECT_EQ(0, strcmp(l_buffer, "3.14159"));
}

TEST(boolean_to_string, _)
{
	var::boolean l_is_true = true;
	EXPECT_EQ(0, strcmp(boolean_to_string<char>(l_is_true), "true"));
}


TEST(string_to_uint, all)
{
	uint_info l_result = string_to_uint("1024MB");
	EXPECT_EQ(1024, l_result._value);
	EXPECT_EQ(4, l_result._digit_length);

	char l_string[10] = "8102023";
	l_result = string_to_uint(l_string);
	EXPECT_EQ(8102023, l_result._value);
	EXPECT_EQ(7, l_result._digit_length);
}


TEST(string_to_int, all)
{
	int_info l_result = string_to_int("-999HP");
	EXPECT_EQ(-999, l_result._value);
	EXPECT_EQ(3, l_result._digit_length);

	char l_string[10] = "-5959";
	l_result = string_to_int(l_string);
	EXPECT_EQ(-5959, l_result._value);
	EXPECT_EQ(4, l_result._digit_length);
}




TEST(string_to_boolean, all)
{
	var::boolean l_result = string_to_boolean("true");
	EXPECT_EQ(true, l_result);

	char l_string[10] = "false";
	l_result = string_to_boolean(l_string);
	EXPECT_EQ(false, l_result);
}




TEST(string_to_float, all)
{
	real_info l_result = string_to_float("3.14159265f");
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




	l_result = string_to_float("-3.14159265f");
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