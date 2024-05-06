#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <FE/core/block_pool.hxx>
#include <FE/core/block_pool_allocator.hxx>
#include <FE/core/pool.hxx>
#include <FE/core/pool_allocator.hxx>
#include <FE/core/algorithm/string.hxx>
#include <FE/miscellaneous/undefine_max_min.h>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <FE/miscellaneous/define_max_min.h>
#include <utility>
#include <string>




TEST(pool, block_allocation)
{
	{
		FE::block_pool<std::string> l_block_pool;

		FE::memory_region_t l_memory_region = "Restaurant";

		FE::block_pool_ptr<std::string> l_smart_ptr1 = l_block_pool.allocate(l_memory_region.data());
		l_smart_ptr1->reserve(20);
		l_smart_ptr1->assign("Aglio e olio!");

		FE::block_pool_ptr<std::string> l_smart_ptr2 = l_block_pool.allocate(l_memory_region.data());
		l_smart_ptr2->reserve(20);
		l_smart_ptr2->assign("Pizza!");

		FE::block_pool_ptr<std::string> l_smart_ptr3 = l_block_pool.allocate(l_memory_region.data());
		l_smart_ptr3->reserve(20);
		l_smart_ptr3->assign("Vongole!");

		FE::block_pool_ptr<std::string> l_smart_ptr4 = l_block_pool.allocate(l_memory_region.data());
		l_smart_ptr4->reserve(20);
		l_smart_ptr4->assign("Carbonara!");

		FE::block_pool_ptr<std::string> l_smart_ptr5 = l_block_pool.allocate(l_memory_region.data());
		l_smart_ptr5->reserve(20);
		l_smart_ptr5->assign("Arrabbiata!");

	}

	{
		FE::block_pool<FE::var::int64, 32> l_pool;
		FE::block_pool_ptr<FE::var::int64, 32> l_smart_ptr[10];

		for (FE::var::uint32 i = 0; i < 10; ++i)
		{
			l_smart_ptr[i] = l_pool.allocate();
		}
	}
}




TEST(pool, block_pool_allocator)
{
	{
		std::map<FE::var::byte*, FE::var::size_t, std::greater<FE::var::byte*>, 
			FE::block_pool_allocator<	std::pair<	FE::var::byte* const, 
													FE::var::size_t>, 
										FE::object_count<32>>
		> l_inst;

		size_t l_size_in_bytes = 128;
		FE::var::byte* l_ptr = new FE::var::byte[l_size_in_bytes];
		l_inst.insert(std::make_pair(l_ptr, l_size_in_bytes));
		l_inst.erase(l_ptr);
		delete[] l_ptr;
	}
}




TEST(pool, generic_block_allocation)
{
	FE::generic_pool<1 KB> l_pool;
	{
		FE::generic_pool_ptr<std::vector<FE::var::int32>, 1 KB> l_smart_ptr = l_pool.allocate<std::vector<FE::var::int32>>();
	}

	{
		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr = l_pool.allocate<std::string>();
	}

	{
		FE::generic_pool_ptr<FE::var::int64, 1 KB> l_smart_ptr[8];

		for (FE::var::uint32 i = 0; i < 8; ++i)
		{
			l_smart_ptr[i] = l_pool.allocate<FE::var::int64>();
		}

		FE::generic_pool_ptr<FE::var::int64, 1 KB> l_another_smart_ptr;
		l_another_smart_ptr = std::move(l_smart_ptr[0]);
	}

	{
		FE::generic_pool_ptr<FE::var::int64, 1 KB> l_smart_ptr[4];

		for (FE::var::uint32 i = 0; i < 4; ++i)
		{
			l_smart_ptr[i] = l_pool.allocate<FE::var::int64>();
		}

		FE::generic_pool_ptr<FE::var::int64, 1 KB> l_another_smart_ptr = std::move(l_smart_ptr[0]);
	}


	FE::memory_region_t l_memory_region = "Restaurant";
	{
		FE::generic_pool<1 KB> l_namespace_pool;
		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr1 = l_namespace_pool.allocate<std::string>(l_memory_region.data(), 1);
		l_smart_ptr1->reserve(20);
		l_smart_ptr1->assign("Aglio e olio!");

		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr2 = l_namespace_pool.allocate<std::string>(l_memory_region.data(), 1);
		l_smart_ptr2->reserve(20);
		l_smart_ptr2->assign("Pizza!");

		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr3 = l_namespace_pool.allocate<std::string>(l_memory_region.data(), 1);
		l_smart_ptr3->reserve(20);
		l_smart_ptr3->assign("Vongole!");

		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr4 = l_namespace_pool.allocate<std::string>(l_memory_region.data(), 1);
		l_smart_ptr4->reserve(20);
		l_smart_ptr4->assign("Carbonara!");

		FE::generic_pool_ptr<std::string, 1 KB> l_smart_ptr5 = l_namespace_pool.allocate<std::string>(l_memory_region.data(), 1);
		l_smart_ptr5->reserve(20);
		l_smart_ptr5->assign("Arrabbiata!");
	}
}




TEST(new_delete_pool_allocator, all)
{
	FE::new_delete_allocator<FE::pool_allocator<std::string>> l_allocator;

	{
		auto l_ptr = l_allocator.allocate(1);

		l_ptr = l_allocator.reallocate(l_ptr, 1, 2);

		l_allocator.deallocate(l_ptr, 2);
	}
}




TEST(pool_allocator, all)
{
	{
		std::vector<std::string, FE::pool_allocator<std::string>> l_vector;

		l_vector.reserve(64);
		l_vector.shrink_to_fit();
	}
}


TEST(namespace_pool_allocator, all)
{
	{
		std::list<int, FE::namespace_pool_allocator<int>> l_list{ FE::namespace_pool_allocator<int>{"list node pool"} };
		l_list.get_allocator().create_pages(1);
		for (int i = 0; i < 10; ++i)
		{
			l_list.push_back(1);
		}
		l_list.get_allocator().shrink_to_fit();
	}
}


void memory_pooled_std_list_iteration(benchmark::State& state_p) noexcept
{
	FE::namespace_pool_allocator<int> l_allocator("list node pool"); // std::list nodes are allocated under the memory pool namespace named "list node pool". This provides higher cache hit ratio. 
	benchmark::DoNotOptimize(l_allocator);
	std::list<int, FE::namespace_pool_allocator<int>> l_list{ l_allocator };
	benchmark::DoNotOptimize(l_list);
	l_allocator.create_pages(1);
	for (int i = 0; i < 10; ++i)
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
	l_allocator.shrink_to_fit();
}
BENCHMARK(memory_pooled_std_list_iteration);


void std_list_iteration(benchmark::State& state_p) noexcept
{
	std::list<int> l_list;
	benchmark::DoNotOptimize(l_list);

	for (int i = 0; i < 10; ++i)
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




#define _MAX_ITERATION_ 5000
void boost_pool_allocator_extreme_test(benchmark::State& state_p) noexcept
{
	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	boost::pool_allocator<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	for (auto _ : state_p)
	{
		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = boost::pool_allocator<std::string>::allocate(1);
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				boost::pool_allocator<std::string>::deallocate(l_s_strings[i], 1);
			}
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
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
		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = l_allocator.malloc();
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.free(l_s_strings[i]);
			}
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
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
	FE::pool_allocator<std::string> l_allocator;
	benchmark::DoNotOptimize(l_allocator);

	l_allocator.create_pages(1);

	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	for (auto _ : state_p)
	{
		for(FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = l_allocator.allocate(1);
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				l_allocator.deallocate(l_s_strings[i], 1);
			}
		}
	
		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 1)
			{
				l_allocator.deallocate(l_s_strings[i], 1);
			}
		}
	}
}
BENCHMARK(FE_pool_allocator_extreme_test);


void cpp_new_delete_extreme_test(benchmark::State& state_p) noexcept
{
	static std::string* l_s_strings[_MAX_ITERATION_];
	benchmark::DoNotOptimize(l_s_strings);

	for (auto _ : state_p)
	{
		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			l_s_strings[i] = new std::string;
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
		{
			if (i % 2 == 0)
			{
				delete l_s_strings[i];
			}
		}

		for (FE::var::uint32 i = 0; i < _MAX_ITERATION_; ++i)
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