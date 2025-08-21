#include <gtest/gtest.h>
#include <benchmark/benchmark.h>


// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/memory.hpp>
#include <FE/algorithm/string.hxx>
using namespace FE;




TEST(memmove, string_insertion)
{
	std::unique_ptr<char[]> l_string(new char[64] {"Freddy's Pizza\0"});
	ASCII l_string2[] = " Fazbear";
	uint64 l_string2_len = algorithm::string::length(l_string2);

	uint64 l_target_location = algorithm::string::find_the_first(l_string.get(), "'")->_begin;
	auto l_rest = algorithm::string::find_the_first(l_string.get(), "'s Pizza");
	uint64 l_rest_length = l_rest->_end - l_rest->_begin;

	FE::memmove(l_string.get() + l_target_location + l_string2_len, l_string.get() + l_target_location, l_rest_length);

	FE::memcpy(l_string.get() + l_target_location, l_string2, l_string2_len);

	EXPECT_TRUE(algorithm::string::compare(l_string.get(), "Freddy Fazbear's Pizza"));
}

TEST(memmove, General)
{
	std::unique_ptr<var::int64[]> l_array(new var::int64[]{ 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1});

	FE::memmove(l_array.get() + 3, l_array.get() + 1, sizeof(int64) * 6);

	EXPECT_EQ(l_array.get()[0], 1);
	EXPECT_EQ(l_array.get()[1], 0);
	EXPECT_EQ(l_array.get()[2], 1);
	EXPECT_EQ(l_array.get()[3], 0);
	EXPECT_EQ(l_array.get()[4], 1);
	EXPECT_EQ(l_array.get()[5], 2);
	EXPECT_EQ(l_array.get()[6], 3);
	EXPECT_EQ(l_array.get()[7], 4);
	EXPECT_EQ(l_array.get()[8], 5);
}

TEST(memcpy, General)
{
	constexpr auto l_length = 10;
	std::unique_ptr<int[]> l_source(new int [l_length]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
	std::unique_ptr<int[]> l_destination(new int [l_length]{0});

	FE::memcpy(l_destination.get(), l_source.get(), l_length * sizeof(int));

	for ( var::int32 i = 0; i < l_length; ++i)
	{
		EXPECT_EQ(l_destination.get()[i], 1);
	}
}

TEST(memzero, General)
{
	constexpr auto l_length = 10;
	var::byte l_arr[l_length];
	
	FE::memzero(l_arr, l_length * sizeof(var::byte));
		
	for ( var::int32 i = 0; i < l_length; ++i)
	{
		EXPECT_EQ(l_arr[i], 0);
	}
}




#define _MAGICAL_SIZE_ 102400

void FE_aligned_memcpy_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);
	
	for (auto _ : state_p)
	{
		benchmark::ClobberMemory();
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_dest, l_source, _MAGICAL_SIZE_);
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memcpy_benchmark)->Iterations(100000);

void std_memcpy_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);

	for (auto _ : state_p)
	{
		benchmark::ClobberMemory();
		std::memcpy(l_dest, l_source, _MAGICAL_SIZE_);
		std::memcpy(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(std_memcpy_benchmark)->Iterations(100000);


void FE_aligned_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		FE::memmove(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(FE_aligned_memmove_benchmark)->Iterations(100000);

void std_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		std::memmove(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(std_memmove_benchmark)->Iterations(100000);


void FE_aligned_memzero_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		FE::memzero<FE::Address::_Aligned>(l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memzero_benchmark)->Iterations(100000);

void std_memset_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		std::memset(l_dest, null, _MAGICAL_SIZE_);
		std::memset(l_dest, 1, _MAGICAL_SIZE_);
	}
}
BENCHMARK(std_memset_benchmark)->Iterations(100000);


#undef _MAGICAL_SIZE_ 








// Test cases for unique_ptr<T>
TEST(UniquePtr, DefaultConstructor)
{
	FE::unique_ptr<int> l_ptr;
	EXPECT_EQ(l_ptr.get(), nullptr);
}

TEST(UniquePtr, ConstructorWithAllocator)
{
	FE::unique_ptr<int> l_ptr(std::pmr::polymorphic_allocator<int>{});
	EXPECT_NE(l_ptr.get(), nullptr);

	FE::unique_ptr<int> l_ptr2 = FE::allocate_unique<int>();
	EXPECT_NE(l_ptr2.get(), nullptr);
}

TEST(UniquePtr, ConstructorWithAllocatorAndArgs)
{
	FE::unique_ptr<int> l_ptr(std::pmr::polymorphic_allocator<int>{}, 42);
	EXPECT_EQ(*l_ptr, 42);
}

TEST(UniquePtr, MoveConstructor)
{
	FE::unique_ptr<int> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 42);
	FE::unique_ptr<int> l_ptr2(std::move(l_ptr1));
	EXPECT_EQ(*l_ptr2, 42);
	EXPECT_EQ(l_ptr1.get(), nullptr);
}

TEST(UniquePtr, MoveAssignment)
{
	FE::unique_ptr<int> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 42);
	FE::unique_ptr<int> l_ptr2;
	l_ptr2 = std::move(l_ptr1);
	EXPECT_EQ(*l_ptr2, 42);
	EXPECT_EQ(l_ptr1.get(), nullptr);
}

TEST(UniquePtr, Reset)
{
	FE::unique_ptr<int> l_ptr(std::pmr::polymorphic_allocator<int>{}, 42);
	l_ptr.reset();
	EXPECT_EQ(l_ptr.get(), nullptr);
}

TEST(UniquePtr, Swap)
{
	FE::unique_ptr<int> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 42);
	FE::unique_ptr<int> l_ptr2(std::pmr::polymorphic_allocator<int>{}, 24);
	l_ptr1.swap(l_ptr2);
	EXPECT_EQ(*l_ptr1, 24);
	EXPECT_EQ(*l_ptr2, 42);
}

// Test cases for unique_ptr<T[]>
TEST(UniquePtrArray, DefaultConstructor)
{
	FE::unique_ptr<int[]> l_ptr;
	EXPECT_EQ(l_ptr.get(), nullptr);

	FE::unique_ptr<int[]> l_ptr2 = FE::allocate_unique<int[]>(7);
	EXPECT_NE(l_ptr2.get(), nullptr);
}

TEST(UniquePtrArray, ConstructorWithAllocator)
{
	FE::unique_ptr<int[]> l_ptr(std::pmr::polymorphic_allocator<int>{}, 10);
	EXPECT_NE(l_ptr.get(), nullptr);
}

TEST(UniquePtrArray, ConstructorWithAllocatorAndArgs)
{
	FE::unique_ptr<int[]> l_ptr(std::pmr::polymorphic_allocator<int>{}, 10, 42);
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(l_ptr[i], 42);
	}
}

TEST(UniquePtrArray, MoveConstructor)
{
	FE::unique_ptr<int[]> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 10, 42);
	FE::unique_ptr<int[]> l_ptr2(std::move(l_ptr1));
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(l_ptr2[i], 42);
	}
	EXPECT_EQ(l_ptr1.get(), nullptr);
}

TEST(UniquePtrArray, MoveAssignment)
{
	FE::unique_ptr<int[]> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 10, 42);
	FE::unique_ptr<int[]> l_ptr2;
	l_ptr2 = std::move(l_ptr1);
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(l_ptr2[i], 42);
	}
	EXPECT_EQ(l_ptr1.get(), nullptr);
}

TEST(UniquePtrArray, Reset)
{
	FE::unique_ptr<int[]> l_ptr(std::pmr::polymorphic_allocator<int>{}, 10, 42);
	l_ptr.reset();
	EXPECT_EQ(l_ptr.get(), nullptr);
}

TEST(UniquePtrArray, Swap)
{
	FE::unique_ptr<int[]> l_ptr1(std::pmr::polymorphic_allocator<int>{}, 10, 42);
	FE::unique_ptr<int[]> l_ptr2(std::pmr::polymorphic_allocator<int>{}, 10, 24);
	l_ptr1.swap(l_ptr2);
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(l_ptr1[i], 24);
		EXPECT_EQ(l_ptr2[i], 42);
	}
}




// Benchmark for FE::unique_ptr<int> vs std::unique_ptr<int>
void FE_allocate_unique_single_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<int> l_ptr = FE::allocate_unique<int>();
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(FE_allocate_unique_single_benchmark)->Iterations(100000);

void std_allocate_unique_single_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int> l_ptr = std::make_unique<int>();
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_single_benchmark)->Iterations(100000);




void FE_allocate_unique_single_with_args_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<int> l_ptr = FE::allocate_unique<int>(std::pmr::get_default_resource(), 42);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(FE_allocate_unique_single_with_args_benchmark)->Iterations(100000);

void std_allocate_unique_single_with_args_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int> l_ptr = std::make_unique<int>(42);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_single_with_args_benchmark)->Iterations(100000);




// Benchmark for FE::unique_ptr<int[]> vs std::unique_ptr<int[]>
void FE_allocate_unique_array_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<int[]> l_ptr = FE::allocate_unique<int[]>(10);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(FE_allocate_unique_array_benchmark)->Iterations(100000);

void std_allocate_unique_array_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int[]> l_ptr = std::make_unique<int[]>(10);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_array_benchmark)->Iterations(100000);
