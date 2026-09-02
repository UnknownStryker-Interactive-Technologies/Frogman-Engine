#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// std
#include <iostream>
#include <string>
#include <random>

// boost
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/pool.hpp>

// Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
#include <FE/algorithm/utility.hxx>
#include <FE/memory_resource.hxx>
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




// ---------------------------------------------------------------------------------------------------------------------
// Random-size benchmarks.
// The size stream is deterministic and shared: every allocator variant draws the same sizes in the same order.
// rand() is not used because MSVC's RAND_MAX is 32767, so "rand() % (4 MiB)" never produced a size above 32767.
// reserve() is used instead of resize() so that the measurement is the allocation itself, not the zero-fill of the block.
// ---------------------------------------------------------------------------------------------------------------------
class random_size_sequence
{
	std::mt19937 m_engine;
	std::uniform_int_distribution<var::int32> m_distribution;
	static constexpr std::uint32_t seed = 20260903u;

public:
	random_size_sequence(FE::int32 min_p, FE::int32 max_p) noexcept : m_engine(seed), m_distribution(min_p, max_p) {}
	void reset() noexcept { m_engine.seed(seed); }
	FE::int32 next() noexcept { return m_distribution(m_engine); }
};
constexpr FE::int32 random_size_min = 1;
constexpr FE::int32 random_size_max = 64 * FE::one_KiB; // <= 128 B goes to the block pools, 129 B ~ 64 KiB to the scalable pool.

// Random size allocation and deallocation benchmark for default C++ operators
void aligned_malloc_aligned_free_random_size_test(benchmark::State& state_p) noexcept
{
	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		std::vector<std::byte> l_vector;
		benchmark::DoNotOptimize(l_vector);
		benchmark::ClobberMemory();

		l_vector.reserve(l_sizes.next());
		benchmark::DoNotOptimize(l_vector.data());
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_test)->Iterations(10000);


void FE_TLGPMP_random_size_test(benchmark::State& state_p) noexcept
{
	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		std::pmr::vector<std::byte> l_vector(FE::framework::framework_base::get_framework().get_memory_resource());
		benchmark::DoNotOptimize(l_vector);
		benchmark::ClobberMemory();

		l_vector.reserve(l_sizes.next());
		benchmark::DoNotOptimize(l_vector.data());
	}
}
BENCHMARK(FE_TLGPMP_random_size_test)->Iterations(10000);

void FE_LTLGPMP_random_size_test(benchmark::State& state_p) noexcept
{
	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		std::pmr::vector<std::byte> l_vector(FE::framework::framework_base::get_framework().get_large_memory_resource());
		benchmark::DoNotOptimize(l_vector);
		benchmark::ClobberMemory();

		l_vector.reserve(l_sizes.next());
		benchmark::DoNotOptimize(l_vector.data());
	}
}
BENCHMARK(FE_LTLGPMP_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		std::vector<std::byte,
			boost::pool_allocator<std::byte,
			boost::default_user_allocator_new_delete,
			boost::details::pool::null_mutex>> l_vector;
		benchmark::DoNotOptimize(l_vector);
		benchmark::ClobberMemory();

		l_vector.reserve(l_sizes.next());
		benchmark::DoNotOptimize(l_vector.data());
	}
}
BENCHMARK(boost_pool_allocator_random_size_test)->Iterations(10000);

// Random size allocation and deallocation benchmark for boost::fast_pool_allocator
void boost_fast_pool_allocator_random_size_test(benchmark::State& state_p) noexcept
{
	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_vector;
		benchmark::DoNotOptimize(l_vector);
		benchmark::ClobberMemory();

		l_vector.reserve(l_sizes.next());
		benchmark::DoNotOptimize(l_vector.data());
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

	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		l_sizes.reset(); // every iteration replays the same 1000 sizes
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			std::vector<std::byte> l_temp_vector;
			l_temp_vector.reserve(l_sizes.next());
			benchmark::DoNotOptimize(l_temp_vector.data());
			l_vector.emplace_back(std::move(l_temp_vector));
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(aligned_malloc_aligned_free_random_size_accumulation_test)->Iterations(1000);

void FE_TLGPMP_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector< std::pmr::vector<std::byte> > l_vector(FE::framework::framework_base::get_framework().get_memory_resource());
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		l_sizes.reset();
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			std::pmr::vector<std::byte> l_temp_vector(FE::framework::framework_base::get_framework().get_memory_resource());
			l_temp_vector.reserve(l_sizes.next());
			benchmark::DoNotOptimize(l_temp_vector.data());
			l_vector.emplace_back(std::move(l_temp_vector));
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_TLGPMP_random_size_accumulation_test)->Iterations(1000);

void FE_LTLGPMP_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::pmr::vector< std::pmr::vector<std::byte> > l_vector(FE::framework::framework_base::get_framework().get_large_memory_resource());
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		l_sizes.reset();
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			std::pmr::vector<std::byte> l_temp_vector(FE::framework::framework_base::get_framework().get_large_memory_resource());
			l_temp_vector.reserve(l_sizes.next());
			benchmark::DoNotOptimize(l_temp_vector.data());
			l_vector.emplace_back(std::move(l_temp_vector));
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(FE_LTLGPMP_random_size_accumulation_test)->Iterations(1000);



// Random size allocation and deallocation benchmark for boost::pool_allocator
void boost_pool_allocator_random_size_accumulation_test(benchmark::State& state_p) noexcept
{
	std::vector<
		std::vector<std::byte,
		boost::pool_allocator<	std::byte,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex>
		>,

		boost::pool_allocator<	std::vector<std::byte,
		boost::pool_allocator<	std::byte,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex>
		>,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex>
	> l_vector;

	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		l_sizes.reset();
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			std::vector<std::byte,
				boost::pool_allocator<std::byte,
				boost::default_user_allocator_new_delete,
				boost::details::pool::null_mutex>> l_temp_vector;

			l_temp_vector.reserve(l_sizes.next());
			benchmark::DoNotOptimize(l_temp_vector.data());
			l_vector.emplace_back(std::move(l_temp_vector));
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
	std::vector<
		std::vector<std::byte, boost::fast_pool_allocator<std::byte>>,
		boost::fast_pool_allocator< std::vector< std::byte, boost::fast_pool_allocator<std::byte> > >
	> l_vector; // the code does not compile with the boost pool's null_mutex
	benchmark::DoNotOptimize(l_vector);
	l_vector.reserve(state_p.max_iterations);
	benchmark::ClobberMemory();

	random_size_sequence l_sizes(random_size_min, random_size_max);
	for (auto _ : state_p)
	{
		l_sizes.reset();
		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			std::vector<std::byte, boost::fast_pool_allocator<std::byte>> l_temp_vector;
			l_temp_vector.reserve(l_sizes.next());
			benchmark::DoNotOptimize(l_temp_vector.data());
			l_vector.emplace_back(std::move(l_temp_vector));
		}

		for (int i = 0; i < state_p.max_iterations; ++i)
		{
			l_vector.pop_back();
		}
	}
}
BENCHMARK(boost_fast_pool_allocator_random_size_accumulation_test)->Iterations(10);

#undef _MAX_ITERATION_