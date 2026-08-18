#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <string>

// boost
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/pool.hpp>

// Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
#include <FE/algorithm/utility.hxx>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>
#include <FE/pool/memory_resource.hxx>
#include <FE/framework.hxx>




void boost_object_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[10000];
	benchmark::DoNotOptimize(l_strings);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings[i] = l_allocator.construct();
		}

		for (int i = 0; i < 500; ++i)
		{
			l_allocator.destroy(l_strings[i]);
		}
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(1000);

void boost_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::pool_allocator<std::string, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(boost_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(1000);

void boost_fast_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::fast_pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(1000);



void aligned_malloc_aligned_free_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(aligned_malloc_aligned_free_extreme_fixed_sized_accumulation_test)->Iterations(1000);

void std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::unsynchronized_pool_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_accumulation_test)->Iterations(1000);


void FE_TLGPMP_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::list<std::string> l_strings(FE::framework::framework_base::get_framework().get_memory_resource());
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(FE_TLGPMP_extreme_fixed_sized_accumulation_test)->Iterations(1000);

void FE_LTLGPMP_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::list<std::string> l_strings(FE::framework::framework_base::get_framework().get_large_memory_resource());
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < 500; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 500; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(FE_LTLGPMP_extreme_fixed_sized_accumulation_test)->Iterations(1000);




void boost_object_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::string* l_string;
	benchmark::DoNotOptimize(l_string);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_string = l_allocator.construct();
		l_allocator.destroy(l_string);
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_fixed_sized_test)->Iterations(30000);

void boost_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::pool_allocator<std::string, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(boost_pool_allocator_extreme_fixed_sized_test)->Iterations(30000);

void boost_fast_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::fast_pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(boost_fast_pool_allocator_extreme_fixed_sized_test)->Iterations(30000);



void aligned_malloc_aligned_free_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string> l_strings;
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(aligned_malloc_aligned_free_extreme_fixed_sized_test)->Iterations(30000);

void std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::pmr::unsynchronized_pool_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_test)->Iterations(30000);


void FE_TLGPMP_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::pmr::list<std::string> l_strings(FE::framework::framework_base::get_framework().get_memory_resource());
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(FE_TLGPMP_extreme_fixed_sized_test)->Iterations(30000);

void FE_LTLGPMP_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::pmr::list<std::string> l_strings(FE::framework::framework_base::get_framework().get_large_memory_resource());
	benchmark::DoNotOptimize(l_strings);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(FE_LTLGPMP_extreme_fixed_sized_test)->Iterations(30000);




// Random size allocation and deallocation benchmark for default C++ operators
void aligned_malloc_aligned_free_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte> l_vector;
	benchmark::DoNotOptimize(l_vector);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_test)->Iterations(10000);


// Random size allocation and deallocation benchmark for FE::scalable_allocator
void FE_TLGPMP_random_size_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector<std::byte> l_vector(FE::framework::framework_base::get_framework().get_memory_resource());
	benchmark::DoNotOptimize(l_vector);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(FE_TLGPMP_random_size_test)->Iterations(10000);

void FE_LTLGPMP_random_size_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector<std::byte> l_vector(FE::framework::framework_base::get_framework().get_large_memory_resource());
	benchmark::DoNotOptimize(l_vector);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(FE_LTLGPMP_random_size_test)->Iterations(10000);


// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte, 
		boost::pool_allocator<std::byte, 
		boost::default_user_allocator_new_delete, 
		boost::details::pool::null_mutex>> l_vector;
	benchmark::DoNotOptimize(l_vector);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(boost_pool_allocator_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for boost::fast_pool_allocator
void boost_fast_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_vector;
	benchmark::DoNotOptimize(l_vector);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(boost_fast_pool_allocator_random_size_test)->Iterations(10000);




// Random size allocation and deallocation benchmark for default C++ operators
void aligned_malloc_aligned_free_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
			std::vector<std::byte> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_accumulation_test)->Iterations(1000);



// Random size allocation and deallocation benchmark for FE::scalable_allocator
void FE_TLGPMP_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector< std::pmr::vector<std::byte> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
			std::pmr::vector<std::byte> l_temp_vector(FE::framework::framework_base::get_framework().get_memory_resource());
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_TLGPMP_size_accumulation_test)->Iterations(1000);

void FE_LTLGPMP_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector< std::pmr::vector<std::byte> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
			std::pmr::vector<std::byte> l_temp_vector(FE::framework::framework_base::get_framework().get_large_memory_resource());
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_LTLGPMP_size_accumulation_test)->Iterations(1000);


// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte, 
		boost::pool_allocator<std::byte, 
		boost::default_user_allocator_new_delete, 
		boost::details::pool::null_mutex>> > l_vector;

	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
			std::vector<std::byte, 
				boost::pool_allocator<std::byte, 
				boost::default_user_allocator_new_delete, 
				boost::details::pool::null_mutex>> l_temp_vector;

			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(boost_pool_allocator_random_size_accumulation_test)->Iterations(1000);

// Random size allocation and deallocation benchmark for boost::fast_pool_allocator
void boost_fast_pool_allocator_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte, boost::fast_pool_allocator<std::byte>> > l_vector; // the code does not compile with the boost pool's null_mutex
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	for (auto _ : state_p)
	{
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::system_page_size / 2);
			std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.push_back(l_temp_vector);
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_random_size_accumulation_test)->Iterations(100);

#undef _MAX_ITERATION_