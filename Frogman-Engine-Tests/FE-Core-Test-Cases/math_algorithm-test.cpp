#include <gtest/gtest.h>
#include <FE/core/algorithm/math.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/algorithm/private/math.cpp> /* I have no idea why Visual Studio 2022 cannot recognize function symbols of math.cpp even the .cpp file is a part of the same project. */
/*
Build started...
1>------ Build started: Project: FE_TEST, Configuration: Debug x64 ------
1>math_algorithm-test.cpp
1>main.cpp
1>gtest.lib(gtest-all.obj) : warning LNK4099: PDB '' was not found with 'gtest.lib(gtest-all.obj)' or at ''; linking object as if no debug info
1>math_algorithm-test.obj : error LNK2019: unresolved external symbol "bool const __cdecl FE::algorithm::math::is_prime(unsigned __int64)" (?is_prime@math@algorithm@FE@@YA?B_N_K@Z) referenced in function "private: virtual void __cdecl math_prime_Test::TestBody(void)" (?TestBody@math_prime_Test@@EEAAXXZ)
1>math_algorithm-test.obj : error LNK2019: unresolved external symbol "unsigned __int64 const __cdecl FE::algorithm::math::to_prime(unsigned __int64)" (?to_prime@math@algorithm@FE@@YA?B_K_K@Z) referenced in function "private: virtual void __cdecl math_prime_Test::TestBody(void)" (?TestBody@math_prime_Test@@EEAAXXZ)
1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine-Lab\Solution\FE_TEST\Debug\FE_TEST.exe : fatal error LNK1120: 2 unresolved externals
1>Done building project "FE_TEST.vcxproj" -- FAILED.
========== Build: 0 succeeded, 1 failed, 1 up-to-date, 0 skipped ==========
========== Build started at 3:26 PM and took 02.666 seconds ==========
*/
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

	constexpr index_t l_matrix_idx = math::calculate_index(3, 1, 1);
	EXPECT_EQ(l_matrix[l_matrix_idx], 1.0f);
}


TEST(math, abs)
{
	EXPECT_EQ(math::abs(-5i64), 5);
}


TEST(math, is_nearly_equal)
{
	float64 l_first = 3.1415'9265'3590;
	float64 l_second = 3.1415'9265'3580;
	EXPECT_TRUE(math::is_nearly_equal(l_first, l_second, 0.0000'0000'0020));

}