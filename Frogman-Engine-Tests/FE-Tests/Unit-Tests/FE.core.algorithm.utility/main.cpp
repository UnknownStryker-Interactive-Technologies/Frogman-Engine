#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <unordered_map>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/private/debug.h>
using namespace FE;
using namespace algorithm;
using namespace utility;




/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/
int main(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p)
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
	FE_ABORT_IF(benchmark::ReportUnrecognizedArguments(argc_p, argv_p) == true, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
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
	EXPECT_EQ(count_integral_digit_length<int>(l_int), 5);

	EXPECT_EQ(count_integral_digit_length<int>(0), 1);
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
<<<<<<< HEAD
	var::uint64 l_int = 65165684;
=======
	int l_int = 65165684;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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




TEST(any_primitive_to_string, thread_local_buffer)
{
	{
		const char* l_result_ptr = buffered_any_primitive_to_string<char>(true);
		EXPECT_EQ(0, strcmp(l_result_ptr, "true"));

		l_result_ptr = buffered_any_primitive_to_string<char>(1024);
		EXPECT_EQ(0, strcmp(l_result_ptr, "1024"));

		l_result_ptr = buffered_any_primitive_to_string<char>('t');
		EXPECT_EQ(0, strcmp(l_result_ptr, "t"));

		l_result_ptr = buffered_any_primitive_to_string<char>("hi, world");
		EXPECT_EQ(0, strcmp(l_result_ptr, "hi, world"));


		std::unique_ptr<int> l_unique_pointer = std::make_unique<int>();
		l_result_ptr = buffered_any_primitive_to_string<char>(l_unique_pointer.get());

		char l_buffer[100] = "\0";
		std::snprintf(l_buffer, 100, "%p", l_unique_pointer.get());
		EXPECT_EQ(0, strcmp(l_result_ptr, l_buffer));


		l_result_ptr = buffered_any_primitive_to_string<char>(nullptr);
		EXPECT_EQ(0, strcmp(l_result_ptr, "nullptr"));
	}

	{
		var::boolean l_bool = false;
		const char* l_result_ptr = buffered_any_primitive_to_string<char>(l_bool);
		EXPECT_EQ(0, strcmp(l_result_ptr, "false"));

		var::int32 l_int32 = 1024;
		l_result_ptr = buffered_any_primitive_to_string<char>(l_int32);
		EXPECT_EQ(0, strcmp(l_result_ptr, "1024"));

<<<<<<< HEAD
		var::ASCII l_character = 't';
=======
		var::character l_character = 't';
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_result_ptr = buffered_any_primitive_to_string<char>(l_character);
		EXPECT_EQ(0, strcmp(l_result_ptr, "t"));

		std::string hi_world = "hi, world";
		l_result_ptr = buffered_any_primitive_to_string<char>(hi_world.c_str());
		EXPECT_EQ(0, strcmp(l_result_ptr, "hi, world"));


		std::unique_ptr<int> l_unique_pointer = std::make_unique<int>();
		l_result_ptr = buffered_any_primitive_to_string<char>(l_unique_pointer.get());

		char l_buffer[100] = "\0";
		std::snprintf(l_buffer, 100, "%p", l_unique_pointer.get());
		EXPECT_EQ(0, strcmp(l_result_ptr, l_buffer));


		l_result_ptr = buffered_any_primitive_to_string<char>(nullptr);
		EXPECT_EQ(0, strcmp(l_result_ptr, "nullptr"));
	}
}




TEST(any_to_string, thread_local_buffer)
{
	std::vector<std::unordered_map<std::string, void*>> l_complex;
	l_complex.emplace_back();
	const char* l_result_ptr = buffered_any_to_string<char>(l_complex);

<<<<<<< HEAD
	std::unique_ptr<var::ASCII[]> l_unqiue_buffer(new var::ASCII[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
=======
	std::unique_ptr<var::character[]> l_unqiue_buffer(new var::character[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	any_object_binary_representation(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_complex);

	EXPECT_EQ(0, strcmp(l_result_ptr, l_unqiue_buffer.get()));
}




TEST(any_primitive_to_string, local_buffer)
{
<<<<<<< HEAD
	std::unique_ptr<var::ASCII[]> l_unqiue_buffer(new var::ASCII[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
=======
	std::unique_ptr<var::character[]> l_unqiue_buffer(new var::character[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948


	{
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, true);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "true"));

		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, 1024);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "1024"));

		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, 't');
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "t"));

		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, "hi, world");
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "hi, world"));


		std::unique_ptr<int> l_unique_pointer = std::make_unique<int>();
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_unique_pointer.get());

		char l_buffer[100] = "\0";
		std::snprintf(l_buffer, 100, "%p", l_unique_pointer.get());
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), l_buffer));


		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, nullptr);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "nullptr"));
	}


	{
		var::boolean l_bool = false;
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_bool);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "false"));

		var::int32 l_int32 = 1024;
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_int32);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "1024"));

<<<<<<< HEAD
		var::ASCII l_character = 't';
=======
		var::character l_character = 't';
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_character);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "t"));

		std::string hi_world = "hi, world";
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, hi_world.c_str());
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "hi, world"));


		std::unique_ptr<int> l_integral_address = std::make_unique<int>();
		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_integral_address.get());

		char l_answer[100] = "\0";
		std::snprintf(l_answer, 100, "%p", l_integral_address.get());
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), l_answer));


		any_primitive_to_string<char>(l_unqiue_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, nullptr);
		EXPECT_EQ(0, strcmp(l_unqiue_buffer.get(), "nullptr"));
	}
}




TEST(any_to_string, local_buffer)
{
<<<<<<< HEAD
	std::unique_ptr<var::ASCII[]> l_result_buffer(new var::ASCII[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
=======
	std::unique_ptr<var::character[]> l_result_buffer(new var::character[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	std::vector<std::unordered_map<std::string, void*>> l_complex;
	l_complex.emplace_back();
	any_to_string<char>(l_result_buffer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_complex);

<<<<<<< HEAD
	std::unique_ptr<var::ASCII[]> l_answer(new var::ASCII[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
=======
	std::unique_ptr<var::character[]> l_answer(new var::character[_UTILITY_ALGORITHM_BUFER_SIZE_]{});
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	any_object_binary_representation(l_answer.get(), _UTILITY_ALGORITHM_BUFER_SIZE_, l_complex);

	EXPECT_EQ(0, strcmp(l_result_buffer.get(), l_answer.get()));
}




TEST(string_to_uint, all)
{
	integral_info l_result = string_to_uint<var::uint32>("1024MB");
	EXPECT_EQ(1024, l_result._value);
	EXPECT_EQ(4, l_result._digit_length);

	char l_string[10] = "8102023";
	l_result = string_to_uint<var::uint32>(l_string);
	EXPECT_EQ(8102023, l_result._value);
	EXPECT_EQ(7, l_result._digit_length);
}


TEST(string_to_int, all)
{
	integral_info l_result = string_to_int<var::int32>("-999HP");
	EXPECT_EQ(-999, l_result._value);
	EXPECT_EQ(3, l_result._digit_length);

	char l_string[10] = "-5959";
	l_result = string_to_int<var::int32>(l_string);
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
	real_info l_result = string_to_float<var::float64>("3.14159265f");
	EXPECT_EQ(1, l_result._integral_part_length);
	EXPECT_EQ(8, l_result._real_part_length);
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




	l_result = string_to_float<var::float64>("-3.14159265f");
	EXPECT_EQ(1, l_result._integral_part_length);
	EXPECT_EQ(8, l_result._real_part_length);
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