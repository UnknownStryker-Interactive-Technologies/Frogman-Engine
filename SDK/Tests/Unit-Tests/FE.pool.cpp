#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <string>

// boost
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/pool.hpp>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/algorithm/utility.hxx>
#include <FE/pool/block_pool_allocator.hxx>
#include <FE/pool/memory_resource.hpp>




TEST(FE_Pool, compile)
{
	FE::scalable_pool_resource<FE::PoolPageCapacity::_4KB> l_resource;
	std::pmr::vector<std::string> l_strings(&l_resource);
}




#define _MAX_ITERATION_ 10000


void boost_object_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = l_allocator.malloc();
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.free(l_strings[i]);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.free(l_strings[i]);
			}
		}
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_test);

void boost_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	boost::pool_allocator<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = boost::pool_allocator<std::string>::allocate(1);
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				boost::pool_allocator<std::string>::deallocate(l_strings[i], 1);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				boost::pool_allocator<std::string>::deallocate(l_strings[i], 1);
			}
		}
	}
}
BENCHMARK(boost_pool_allocator_extreme_test);

void boost_fast_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	boost::fast_pool_allocator<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = boost::fast_pool_allocator<std::string>::allocate(1);
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				boost::fast_pool_allocator<std::string>::deallocate(l_strings[i], 1);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				boost::fast_pool_allocator<std::string>::deallocate(l_strings[i], 1);
			}
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_extreme_test);


void FE_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<FE::PoolPageCapacity::_64MB, FE::SIMD_auto_alignment> l_allocator;
	l_allocator.create_pages(1);
	benchmark::DoNotOptimize(l_allocator);

	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = l_allocator.template allocate<std::string>(1);
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.template deallocate<std::string>(l_strings[i], 1);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.template deallocate<std::string>(l_strings[i], 1);
			}
		}
	}
}
BENCHMARK(FE_pool_allocator_extreme_test);

void FE_block_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	FE::block_pool<FE::PoolPageCapacity::_64MB, sizeof(std::string), FE::SIMD_auto_alignment> l_allocator;
	l_allocator.create_pages(1);
	benchmark::DoNotOptimize(l_allocator);

	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = l_allocator.template allocate<std::string>();
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.template deallocate<std::string>(l_strings[i]);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.template deallocate<std::string>(l_strings[i]);
			}
		}
	}
}
BENCHMARK(FE_block_pool_allocator_extreme_test);


void _mm_malloc_mm_free_extreme_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings[i] = new std::string;
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				delete l_strings[i];
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				delete l_strings[i];
			}
		}
	}
}
BENCHMARK(_mm_malloc_mm_free_extreme_test);


void std_pmr_unsynchronized_pool_resource_extreme_test(benchmark::State& state_p) noexcept
{
	std::pmr::unsynchronized_pool_resource l_resource;
	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings.push_back(std::string());
		}
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_strings.pop_back();
			}
		}
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_strings.pop_back();
			}
		}
	}
}
BENCHMARK(std_pmr_unsynchronized_pool_resource_extreme_test);


void scalable_pool_resource_extreme_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool_resource<FE::PoolPageCapacity::_64MB> l_resource;
	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_strings.push_back(std::string());
		}
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_strings.pop_back();
			}
		}
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_strings.pop_back();
			}
		}
	}
}
BENCHMARK(scalable_pool_resource_extreme_test);
#undef _MAX_ITERATION_