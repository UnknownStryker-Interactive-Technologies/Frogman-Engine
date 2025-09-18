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
#include <FE/pool/block_pool.hxx>
#include <FE/pool/memory_resource.hpp>




TEST(FE_Core_scalable_pool, __defragment)
{
	FE::scalable_pool<FE::SIMD_auto_alignment> l_pool;
	std::pmr::list<std::size_t> l_strings(&l_pool);
	for (int i = 0; i < 126; ++i)
	{
		l_strings.push_back(10);
	}

	for (int i = 0; i < 126; ++i)
	{
		l_strings.pop_back();
	}

	l_strings.push_back(10);
	l_strings.pop_back();

}


void boost_object_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::string* l_strings[10000];
	benchmark::DoNotOptimize(l_strings);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);
	

	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings[i] = l_allocator.construct();
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_allocator.destroy(l_strings[i]);
		}
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void boost_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(boost_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void boost_fast_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::fast_pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void FE_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<FE::SIMD_auto_alignment> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::list<std::string> l_strings(&l_allocator);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(FE_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void FE_block_pool_allocator_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	FE::block_pool<sizeof(std::string), FE::align_64bytes> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::list<std::string> l_strings(&l_allocator);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(FE_block_pool_allocator_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void aligned_malloc_aligned_free_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::list<std::string> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(aligned_malloc_aligned_free_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::unsynchronized_pool_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_accumulation_test)->Iterations(10000);

void FE_pmr_memory_resource_extreme_fixed_sized_accumulation_test(benchmark::State& state_p) noexcept
{
	FE::memory_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			l_strings.push_back(std::string());
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_strings.pop_back();
		}
	}
}
BENCHMARK(FE_pmr_memory_resource_extreme_fixed_sized_accumulation_test)->Iterations(10000);




void boost_object_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::string* l_string;
	benchmark::DoNotOptimize(l_string);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);


	for (auto _ : state_p)
	{
		l_string = l_allocator.construct();
		l_allocator.destroy(l_string);
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_fixed_sized_test)->Iterations(10000);

void boost_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(boost_pool_allocator_extreme_fixed_sized_test)->Iterations(10000);

void boost_fast_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string, boost::fast_pool_allocator<std::string>> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(boost_fast_pool_allocator_extreme_fixed_sized_test)->Iterations(10000);

void FE_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<FE::SIMD_auto_alignment> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::list<std::string> l_strings(&l_allocator);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(FE_pool_allocator_extreme_fixed_sized_test)->Iterations(10000);

void FE_block_pool_allocator_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	FE::block_pool<sizeof(std::string), FE::align_64bytes> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::list<std::string> l_strings(&l_allocator);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(FE_block_pool_allocator_extreme_fixed_sized_test)->Iterations(10000);

void aligned_malloc_aligned_free_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::list<std::string> l_strings;
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(aligned_malloc_aligned_free_extreme_fixed_sized_test)->Iterations(10000);

void std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	std::pmr::unsynchronized_pool_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(std_pmr_unsynchronized_pool_resource_extreme_fixed_sized_test)->Iterations(10000);

void FE_pmr_memory_resource_extreme_fixed_sized_test(benchmark::State& state_p) noexcept
{
	FE::memory_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::list<std::string> l_strings(&l_resource);
	benchmark::DoNotOptimize(l_strings);


	for (auto _ : state_p)
	{
		l_strings.push_back(std::string());
		l_strings.pop_back();
	}
}
BENCHMARK(FE_pmr_memory_resource_extreme_fixed_sized_test)->Iterations(10000);




// Random size allocation and deallocation benchmark for default C++ operators
void aligned_malloc_aligned_free_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte> l_vector;
	benchmark::DoNotOptimize(l_vector);
	

	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for FE::scalable_pool
void FE_scalable_pool_random_size_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<FE::SIMD_auto_alignment> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::vector<std::byte> l_vector(&l_allocator);
	benchmark::DoNotOptimize(l_vector);


	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(FE_scalable_pool_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for FE::scalable_pool
void FE_pmr_memory_resource_random_size_test(benchmark::State& state_p) noexcept
{
	FE::memory_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::vector<std::byte> l_vector(&l_resource);
	benchmark::DoNotOptimize(l_vector);


	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(FE_pmr_memory_resource_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte, boost::pool_allocator<std::byte, boost::default_user_allocator_new_delete, boost
		::details::pool::null_mutex>> l_vector;
	benchmark::DoNotOptimize(l_vector);


	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(boost_pool_allocator_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for boost::fast_pool_allocator
void boost_fast_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_vector;
	benchmark::DoNotOptimize(l_vector);


	for (auto _ : state_p)
	{
		FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
		l_vector.resize(l_random_size);
	}
}
BENCHMARK(boost_fast_pool_allocator_random_size_test)->Iterations(100);




// Random size allocation and deallocation benchmark for default C++ operators
void aligned_malloc_aligned_free_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
			std::vector<std::byte> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_accumulation_test)->Iterations(1000);

// Random size allocation and deallocation benchmark for FE::scalable_pool
void FE_scalable_pool_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<FE::SIMD_auto_alignment> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	std::pmr::vector< std::pmr::vector<std::byte> > l_vector(&l_allocator);
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
			std::pmr::vector<std::byte> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_scalable_pool_random_size_accumulation_test)->Iterations(1000);

// Random size allocation and deallocation benchmark for FE::scalable_pool
void FE_pmr_memory_resource_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	FE::memory_resource l_resource;
	benchmark::DoNotOptimize(l_resource);

	std::pmr::vector< std::pmr::vector<std::byte> > l_vector(&l_resource);
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
			std::pmr::vector<std::byte> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_pmr_memory_resource_random_size_accumulation_test)->Iterations(1000);

// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte, boost::pool_allocator<std::byte, boost::default_user_allocator_new_delete, boost
		::details::pool::null_mutex>> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
			std::vector<std::byte, boost::pool_allocator<std::byte, boost::default_user_allocator_new_delete, boost
				::details::pool::null_mutex>> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.emplace_back(l_temp_vector);
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(boost_pool_allocator_random_size_accumulation_test)->Iterations(1000);

// Random size allocation and deallocation benchmark for boost::fast_pool_allocator
void boost_fast_pool_allocator_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector< std::vector<std::byte, boost::fast_pool_allocator<std::byte>> > l_vector;
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);


	for (auto _ : state_p)
	{
		for (int i = 0; i < 1000; ++i)
		{
			FE::int32 l_random_size = (rand() % FE::one_KiB) + 1;
			std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_temp_vector;
			l_temp_vector.resize(l_random_size);
			l_vector.push_back(l_temp_vector);
		}

		for (int i = 0; i < 1000; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_random_size_accumulation_test)->Iterations(100);

#undef _MAX_ITERATION_