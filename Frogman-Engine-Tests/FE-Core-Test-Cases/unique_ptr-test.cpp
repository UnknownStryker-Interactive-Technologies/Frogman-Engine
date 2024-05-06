#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <FE/core/smart_ptrs.h>
#include <FE/core/string.hxx>
#include <string>
#include <memory>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.




TEST(unique_ptr, construct_by_value_assignment)
{
	using namespace FE;

	{
		{
			FE::unique_ptr<var::int64> l_unique_ptr = 64;
		}

		FE::unique_ptr<var::int64> l_unique_ptr;
		l_unique_ptr = 64;
	}


	{
		{
			FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
		}

		FE::unique_ptr<std::string> l_unique_ptr;
		l_unique_ptr = std::string("std::string");
	}
}




#ifdef _HAS_CXX20_
#include <FE/core/pair.hxx>
TEST(unique_ptr, size)
{
	using namespace FE;

	{
		EXPECT_EQ(sizeof(FE::unique_ptr<int>), 8);
	}
}
#endif




TEST(unique_ptr, release)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
		FE::unique_ptr<std::string>::pointer l_pointer = l_unique_ptr.release();
		EXPECT_EQ(l_unique_ptr.get(), nullptr);
		l_unique_ptr.get_allocator().deallocate(l_pointer, 1);
	}
}




TEST(unique_ptr, reset)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
		l_unique_ptr.reset();
		EXPECT_EQ(l_unique_ptr.get(), nullptr);
	}
}




TEST(unique_ptr, swap)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
		FE::unique_ptr<std::string> l_another = std::string();

		l_unique_ptr.swap(l_another);
		EXPECT_EQ(strcmp(l_another->c_str(), "std::string"), 0);
	}
}




TEST(unique_ptr, make_unique)
{
	{
		FE::unique_ptr<std::string> l_unique_ptr = FE::make_unique<std::string>();

		EXPECT_FALSE(l_unique_ptr == nullptr);
	}

	{
		FE::unique_ptr<std::string> l_unique_ptr = FE::make_unique<std::string>("std::string");

		EXPECT_FALSE(l_unique_ptr == nullptr);

		EXPECT_EQ(strcmp(l_unique_ptr->c_str(), "std::string"), 0);
	}
}








TEST(unique_ptr, construct_array_by_value_assignment)
{
	using namespace FE;

	{
		{
			FE::unique_ptr<var::int64[]> l_unique_ptr = {0, 1, 2, 3};
		}

		FE::unique_ptr<var::int64[]> l_unique_ptr;
		l_unique_ptr = { 0, 1, 2, 3 };
	}

	{
		{
			FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
		}

		FE::unique_ptr<std::string[]> l_unique_ptr;
		l_unique_ptr = { "std::string" };
	}




	{
		FE::unique_ptr<var::int64[]> l_unique_ptr = FE::reserve{ 10 };
	}
}




TEST(unique_ptr, release_array)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
		FE::size_t l_prev_size = l_unique_ptr.capacity();

		FE::unique_ptr<std::string[]>::pointer l_pointer = l_unique_ptr.release();
		EXPECT_EQ(l_unique_ptr.get(), nullptr);
		l_unique_ptr.get_allocator().deallocate(l_pointer, l_prev_size);
	}
}




TEST(unique_ptr, reset_array)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
		l_unique_ptr.reset();
		EXPECT_EQ(l_unique_ptr.get(), nullptr);
	}
}




TEST(unique_ptr, swap_array)
{
	using namespace FE;

	{
		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
		FE::unique_ptr<std::string[]> l_another = { };

		l_unique_ptr.swap(l_another);
		EXPECT_EQ(strcmp(l_another[0].c_str(), "std::string"), 0);
	}
}




TEST(unique_ptr, make_unique_array)
{
	{
		FE::unique_ptr<std::string[]> l_unique_ptr = FE::make_unique<std::string[]>(4);
		EXPECT_FALSE(l_unique_ptr == nullptr);
	}

	{
		FE::unique_ptr<std::string[]> l_unique_ptr = FE::make_unique<std::string[]>({ "std", "string" });

		EXPECT_FALSE(l_unique_ptr == nullptr);

		EXPECT_EQ(strcmp(l_unique_ptr[0].c_str(), "std"), 0);
		EXPECT_EQ(strcmp(l_unique_ptr[1].c_str(), "string"), 0);
	}
}




void FE_unique_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<std::byte[]> l_unique_ptr = FE::make_unique<std::byte[]>(1024);
		benchmark::DoNotOptimize(l_unique_ptr);
	}
}
BENCHMARK(FE_unique_ptr_RAII_construction_and_destruction);


void std_unique_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<std::byte[]> l_unique_ptr = std::make_unique<std::byte[]>(1024);
		benchmark::DoNotOptimize(l_unique_ptr);
	}
}
BENCHMARK(std_unique_ptr_RAII_construction_and_destruction);


void FE_exclusive_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::exclusive_ptr<std::byte[]> l_exclusive_ptr = FE::make_exclusive<std::byte[]>(1024);
		benchmark::DoNotOptimize(l_exclusive_ptr);
	}
}
BENCHMARK(FE_exclusive_ptr_RAII_construction_and_destruction);