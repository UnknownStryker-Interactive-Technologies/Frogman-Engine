#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/algorithm/math.hpp>
#include <FE/vector.hpp>




/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/
int main(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p)
{
	using namespace FE;

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

using namespace FE;
using namespace algorithm;


TEST(math, to_lower_prime)
{
	var::uint64 l_num = 4;

	var::boolean l_result = math::is_prime(l_num);
	EXPECT_FALSE(l_result);

	if (l_result == false)
	{
		l_num = math::to_lower_prime(l_num);
	}

	l_result = math::is_prime(l_num);
	EXPECT_TRUE(l_result);
}


TEST(math, calculate_index)
{
	float64 l_matrix[9] = 
	{ 
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	/* Three by three matrix. The row and the column of the matrix above is three. */

	constexpr index_t l_matrix_idx = FE_CALCULATE_INDEX(3, 1, 1);
	EXPECT_EQ(l_matrix[l_matrix_idx], 1.0f);
}


TEST(math, abs)
{
	EXPECT_EQ(FE_ABS(-5), 5);
}


TEST(math, is_nearly_equal)
{
	float64 l_first = 3.1415'9265'3590;
	float64 l_second = 3.1415'9265'3580;

	EXPECT_TRUE(FE_IS_NEARLY_EQUAL(l_first, l_second, 0.0000'0000'0020));

}


TEST(math, vector3)
{
	
}


TEST(math, sine)
{
	/*
	    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-450)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-405)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-360)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-315)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-270)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-225)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-180)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-135)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-90)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(-45)) << '\n';
    
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(0)) << '\n';
    
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(450)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(405)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(360)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(315)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(270)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(225)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(180)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(135)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(90)) << '\n';
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(45)) << '\n';
    
    std::cout << FE::sin(FE_DEGREE_TO_RADIAN(4090)) << "\n\n";
	*/
}


TEST(math, cosine)
{
	/*
	std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-450)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-405)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-360)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-315)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-270)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-225)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-180)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-135)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-90)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(-45)) << '\n';

    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(0)) << '\n';

    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(450)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(405)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(360)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(315)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(270)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(225)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(180)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(135)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(90)) << '\n';
    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(45)) << '\n';

    std::cout << FE::cos(FE_DEGREE_TO_RADIAN(4090)) << "\n\n";
	*/
}