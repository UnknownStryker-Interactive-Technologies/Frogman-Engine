#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <string>

// boost
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <boost/intrusive_ptr.hpp>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/pool/block_pool_allocator.hxx>
#include <FE/pool/pool_allocator.hxx>
/*
Profile Guided Optimization: https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170

memory layout reflection for serialization  																		      |--- etc...
                                                                                      *
	                                                                                  |
	Shallower <----- Memory Hierarchy Depth ----> Deeper                      Lower Memory Address
                                                      |----------------|              |
	                                                  | - FE::string - |              |
	|----------------------------------|    ----------|FE::smart_ptr   |              |
	|  Target Entry Non-Trivial Object |    |         |length, capacity|              |
	|- member variables -              |    |         |----------------|              |
	|  FE::string m_raw_name ----------|----|                                         |
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
	                                           |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
													                                  |
																					  *
																					  
Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
																									|
												    |-----------------------------------------------|
												    |
												    |--> FE::string.length -> FE::string.capacity
																					   |
																					   |
									           |---------------------------------------|
											   |
											   |--> FE::vector.x -> FE::vector.y -> FE::vector.z
*/ 
int main(int argc_p, char** argv_p)
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
	FE_ASSERT(benchmark::ReportUnrecognizedArguments(argc_p, argv_p) == true, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
    int32 l_exit_code = RUN_ALL_TESTS();
	std::cerr << "\n\n";
	benchmark::RunSpecifiedBenchmarks();
	std::cerr << "\n\n";
    benchmark::Shutdown();
    return l_exit_code;
}




TEST(pool_allocator, all)
{
	FE::scalable_pool<sizeof(std::string) * 64> l_scalable_pool;

	FE::pool_allocator<std::string, FE::size_in_bytes<sizeof(std::string) * 64>> l_pool_allocator(&l_scalable_pool);
	l_pool_allocator.create_pages(1);

	{

		std::string* l_ptr = l_pool_allocator.allocate(1);
		l_pool_allocator.deallocate(l_ptr, 1);
	}
	
	{
		std::vector<std::string, FE::pool_allocator<std::string, FE::size_in_bytes<sizeof(std::string) * 64>>> l_vector(l_pool_allocator);

		l_vector.reserve(32);
		l_vector.shrink_to_fit();
	}
}




void memory_pooled_std_list_iteration(benchmark::State& state_p) noexcept
{
	// std::list nodes are allocated under the memory pool namespace named "list node pool". This provides higher cache hit ratio. 
	FE::scalable_pool< FE::size_in_bytes<1 * FE::one_mb>::size > l_scalable_pool;
	FE::pool_allocator<int, FE::size_in_bytes<1 * FE::one_mb>> l_pool_allocator( &l_scalable_pool );
	std::list<int, FE::pool_allocator<int, FE::size_in_bytes<1 * FE::one_mb>>> l_list(l_pool_allocator);
	benchmark::DoNotOptimize(l_list);

	l_list.push_back(0);
	l_list.pop_front();

	for (int i = 1; i < 1000; ++i)
	{
		l_list.push_back(1);
	}

	for (auto _ : state_p)
	{
		for (auto& element : l_list)
		{
			benchmark::DoNotOptimize(element);
		}
	}
}
BENCHMARK(memory_pooled_std_list_iteration);

void std_list_iteration(benchmark::State& state_p) noexcept
{
	std::list<int> l_list;
	benchmark::DoNotOptimize(l_list);

	for (int i = 0; i < 1000; ++i)
	{
		l_list.push_back(1);
	}

	for (auto _ : state_p)
	{
		for (auto& element : l_list)
		{
			benchmark::DoNotOptimize(element);
		}
	}
}
BENCHMARK(std_list_iteration);

#define _MAX_ITERATION_ 384

void boost_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	boost::pool_allocator<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = boost::pool_allocator<std::string>::allocate(1);
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				boost::pool_allocator<std::string>::deallocate(l_s_strings[i], 1);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				boost::pool_allocator<std::string>::deallocate(l_s_strings[i], 1);
			}
		}
	}
}
BENCHMARK(boost_pool_allocator_extreme_test);

void boost_object_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	boost::object_pool<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);
	
	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = l_allocator.malloc();
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.free(l_s_strings[i]);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.free(l_s_strings[i]);
			}
		}
	}
}
BENCHMARK(boost_object_pool_allocator_extreme_test);

void FE_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	FE::scalable_pool<1000 * FE::one_mb> l_allocator;
	l_allocator.create_pages(1);
	benchmark::DoNotOptimize(l_allocator);

	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	for (auto _ : state_p)
	{
		for(var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = l_allocator.template allocate<std::string>(1);
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.template deallocate<std::string>(l_s_strings[i], 1);
			}
		}
	
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.template deallocate<std::string>(l_s_strings[i], 1);
			}
		}
	}
}
BENCHMARK(FE_pool_allocator_extreme_test);

void FE_block_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	FE::block_pool<sizeof(std::string), _MAX_ITERATION_> l_allocator;
	l_allocator.create_pages(1);
	benchmark::DoNotOptimize(l_allocator);

	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = l_allocator.template allocate<std::string>();
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.template deallocate<std::string>(l_s_strings[i]);
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.template deallocate<std::string>(l_s_strings[i]);
			}
		}
	}
}
BENCHMARK(FE_block_pool_allocator_extreme_test);

void cpp_new_delete_extreme_test(benchmark::State& state_p) noexcept
{
	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = new std::string;
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				delete l_s_strings[i];
			}
		}

		for (var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				delete l_s_strings[i];
			}
		}
	}
}
BENCHMARK(cpp_new_delete_extreme_test);
#undef _MAX_ITERATION_