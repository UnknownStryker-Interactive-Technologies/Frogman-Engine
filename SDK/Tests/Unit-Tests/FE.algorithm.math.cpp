#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/algorithm/math.hpp>

// for performance comparison
#include <algorithm>
#include <cmath>




TEST(math, abs)
{
	EXPECT_EQ(FE::algorithm::math::abs(-5), 5);
}

void frogman_abs_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::math::abs(-5);
		(void)r;
	}
}
BENCHMARK(frogman_abs_benchmark);

void std_abs_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = std::abs(-5);
		(void)r;
	}
}
BENCHMARK(std_abs_benchmark);




TEST(math, min_max)
{
    EXPECT_EQ(FE::algorithm::math::min(-5, 7), -5);
    EXPECT_EQ(FE::algorithm::math::max(-5, 7), 7);
}

void frogman_min_max_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::math::min(-5, 7);
		r = FE::algorithm::math::max(-5, 7);
		(void)r;
	}
}
BENCHMARK(frogman_min_max_benchmark);

void std_min_max_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = std::min(-5, 7);
		r = std::max(-5, 7);
		(void)r;
	}
}
BENCHMARK(std_min_max_benchmark);




TEST(math, clamp)
{
	EXPECT_EQ(FE::algorithm::math::clamp(-5, 0, 10), 0);
	EXPECT_EQ(FE::algorithm::math::clamp(5, 0, 10), 5);
	EXPECT_EQ(FE::algorithm::math::clamp(15, 0, 10), 10);
}

void frogman_clamp_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::math::clamp(-5, 0, 10);
		(void)r;
	}
}
BENCHMARK(frogman_clamp_benchmark);

void std_clamp_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		auto r = std::clamp(-5, 0, 10);
		(void)r;
	}
}
BENCHMARK(std_clamp_benchmark);




TEST(math, is_nearly_equal)
{
	FE::float64 l_first = 3.1415'9265'3590;
	FE::float64 l_second = 3.1415'9265'3580;

	EXPECT_TRUE(FE::algorithm::math::is_nearly_equal(l_first, l_second, 0.0000'0000'0020));
}

TEST(math, calculate_index_of_a_matrix)
{
	FE::float64 l_matrix[9] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	/* Three by three matrix. The row and the column of the matrix above is three. */

	FE::index_t l_matrix_idx = FE::algorithm::math::calculate_index_of_a_matrix(3, 1, 1);
	EXPECT_EQ(l_matrix[l_matrix_idx], 1.0f);
}

TEST(math, radian_to_degree)
{
	EXPECT_EQ(FE::algorithm::math::radian_to_degree(FE::algorithm::math::pi), 180.0);
}

TEST(math, degree_to_radian)
{
	EXPECT_EQ(FE::algorithm::math::degree_to_radian(180.0), FE::algorithm::math::pi);
}

TEST(math, calculate_2D_direction)
{
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.5, 0.0), 0.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(1.0, 0.0), 0.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.5, 0.5), 45.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(1.0, 1.0), 45.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.0, 0.5), 90.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.0, 1.0), 90.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-0.5, 0.5), 135.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-1.0, 1.0), 135.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-0.5, 0.0), 180.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-1.0, 0.0), 180.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-0.5, -0.5), -135.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(-0.1, -0.1), -135.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.0, -0.5), -90.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.0, -1.0), -90.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.5, -0.5), -45.0);
	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(1.0, -1.0), -45.0);

	EXPECT_EQ(FE::algorithm::math::calculate_2D_direction(0.0, 0.0), 0.0);
}

TEST(math, is_prime)
{
	EXPECT_TRUE(FE::algorithm::math::is_prime(2));
	EXPECT_TRUE(FE::algorithm::math::is_prime(3));
	EXPECT_TRUE(FE::algorithm::math::is_prime(5));
	EXPECT_TRUE(FE::algorithm::math::is_prime(7));
	EXPECT_TRUE(FE::algorithm::math::is_prime(11));
	EXPECT_TRUE(FE::algorithm::math::is_prime(13));
	EXPECT_TRUE(FE::algorithm::math::is_prime(17));
	EXPECT_TRUE(FE::algorithm::math::is_prime(19));
	EXPECT_TRUE(FE::algorithm::math::is_prime(23));
	EXPECT_TRUE(FE::algorithm::math::is_prime(29));
	EXPECT_TRUE(FE::algorithm::math::is_prime(31));
	EXPECT_TRUE(FE::algorithm::math::is_prime(37));
	EXPECT_TRUE(FE::algorithm::math::is_prime(41));
	EXPECT_TRUE(FE::algorithm::math::is_prime(43));
	EXPECT_TRUE(FE::algorithm::math::is_prime(47));
	EXPECT_TRUE(FE::algorithm::math::is_prime(53));
	EXPECT_TRUE(FE::algorithm::math::is_prime(59));
	EXPECT_TRUE(FE::algorithm::math::is_prime(61));
	EXPECT_TRUE(FE::algorithm::math::is_prime(67));
	EXPECT_TRUE(FE::algorithm::math::is_prime(71));
	EXPECT_TRUE(FE::algorithm::math::is_prime(73));
	EXPECT_TRUE(FE::algorithm::math::is_prime(79));
	EXPECT_TRUE(FE::algorithm::math::is_prime(83));
	EXPECT_TRUE(FE::algorithm::math::is_prime(89));
	EXPECT_TRUE(FE::algorithm::math::is_prime(97));
	EXPECT_TRUE(FE::algorithm::math::is_prime(101));
	EXPECT_TRUE(FE::algorithm::math::is_prime(103));
	EXPECT_TRUE(FE::algorithm::math::is_prime(107));
	EXPECT_TRUE(FE::algorithm::math::is_prime(109));
	EXPECT_TRUE(FE::algorithm::math::is_prime(113));
	EXPECT_TRUE(FE::algorithm::math::is_prime(127));
	EXPECT_TRUE(FE::algorithm::math::is_prime(131));
	EXPECT_TRUE(FE::algorithm::math::is_prime(137));
	EXPECT_TRUE(FE::algorithm::math::is_prime(139));
	EXPECT_TRUE(FE::algorithm::math::is_prime(149));
}

TEST(math, to_highter_prime)
{
	var::uint64 l_num = 4;
	EXPECT_FALSE(FE::algorithm::math::is_prime(l_num));
	l_num = FE::algorithm::math::to_higher_prime(l_num);
	EXPECT_TRUE(FE::algorithm::math::is_prime(l_num));
}

TEST(math, to_lower_prime)
{
	var::uint64 l_num = 4;
	EXPECT_FALSE(FE::algorithm::math::is_prime(l_num));
	l_num = FE::algorithm::math::to_lower_prime(l_num);
	EXPECT_TRUE(FE::algorithm::math::is_prime(l_num));
}

TEST(math, select_minimum)
{
	EXPECT_EQ(FE::algorithm::math::select_minimum({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }), 1);
}

TEST(math, select_maximum)
{
	EXPECT_EQ(FE::algorithm::math::select_maximum({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }), 10);
}
