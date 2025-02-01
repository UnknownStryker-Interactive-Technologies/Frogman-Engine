#include <gtest/gtest.h>
#include <benchmark/benchmark.h>


// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/memory.hpp>
#include <FE/algorithm/string.hxx>
#include <FE/smart_ptr.hxx>
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




TEST(SIMD, zmmword)
{
	var::int64 l_int64[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	FE::zmmword l_zmmword;
	l_zmmword.storeu(l_int64);

	FE::zmmword l_zmmword2;
	l_zmmword2.store(l_zmmword.load());

	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[0], 1);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[1], 2);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[2], 3);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[3], 4);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[4], 5);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[5], 6);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[6], 7);
	EXPECT_EQ(reinterpret_cast<var::int64*>(l_zmmword2.load())[7], 8);
}

void FE_zmm_copy_benchmark(benchmark::State& state_p) noexcept
{
	var::int64 l_int64[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	FE::zmmword l_zmmword;
	l_zmmword.storeu(l_int64);

	FE::zmmword l_zmmword2;

	for (auto _ : state_p)
	{
		l_zmmword2.store(l_zmmword.load());
		l_zmmword.store(l_zmmword2.load());
	}
}
BENCHMARK(FE_zmm_copy_benchmark);

void FE_512bit_copy_benchmark(benchmark::State& state_p) noexcept
{
	var::int64 l_first[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	var::int64 l_second[8]{};

	for (auto _ : state_p)
	{
		l_second[0] = l_first[0];
		l_second[1] = l_first[1];
		l_second[2] = l_first[2];
		l_second[3] = l_first[3];
		l_second[4] = l_first[4];
		l_second[5] = l_first[5];
		l_second[6] = l_first[6];
		l_second[7] = l_first[7];

		l_first[0] = l_second[0];
		l_first[1] = l_second[1];
		l_first[2] = l_second[2];
		l_first[3] = l_second[3];
		l_first[4] = l_second[4];
		l_first[5] = l_second[5];
		l_first[6] = l_second[6];
		l_first[7] = l_second[7];
	}
}
BENCHMARK(FE_512bit_copy_benchmark);




TEST(SIMD, ymmword)
{
	var::int32 l_int32[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	FE::ymmword l_ymmword;
	l_ymmword.storeu(l_int32);

	FE::zmmword l_ymmword2;
	l_ymmword2.store(l_ymmword.load());

	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[0], 1);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[1], 2);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[2], 3);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[3], 4);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[4], 5);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[5], 6);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[6], 7);
	EXPECT_EQ(reinterpret_cast<var::int32*>(l_ymmword2.load())[7], 8);
}

void FE_ymm_copy_benchmark(benchmark::State& state_p) noexcept
{
	var::int32 l_int32[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	FE::ymmword l_ymmword;
	l_ymmword.storeu(l_int32);

	FE::ymmword l_ymmword2;

	for (auto _ : state_p)
	{
		l_ymmword2.store(l_ymmword.load());
		l_ymmword.store(l_ymmword2.load());
	}
}
BENCHMARK(FE_ymm_copy_benchmark);

void FE_256bit_copy_benchmark(benchmark::State& state_p) noexcept
{
	var::int32 l_first[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	var::int32 l_second[8]{};

	for (auto _ : state_p)
	{
		l_second[0] = l_first[0];
		l_second[1] = l_first[1];
		l_second[2] = l_first[2];
		l_second[3] = l_first[3];
		l_second[4] = l_first[4];
		l_second[5] = l_first[5];
		l_second[6] = l_first[6];
		l_second[7] = l_first[7];

		l_first[0] = l_second[0];
		l_first[1] = l_second[1];
		l_first[2] = l_second[2];
		l_first[3] = l_second[3];
		l_first[4] = l_second[4];
		l_first[5] = l_second[5];
		l_first[6] = l_second[6];
		l_first[7] = l_second[7];
	}
}
BENCHMARK(FE_256bit_copy_benchmark);




TEST(SIMD, xmmword)
{
	var::int16 l_int16[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	FE::xmmword l_xmmword;
	l_xmmword.storeu(l_int16);

	FE::xmmword l_xmmword2;
	l_xmmword2.store(l_xmmword.load());

	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[0], 1);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[1], 2);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[2], 3);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[3], 4);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[4], 5);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[5], 6);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[6], 7);
	EXPECT_EQ(reinterpret_cast<var::int16*>(l_xmmword2.load())[7], 8);
}

void FE_128bit_copy_benchmark(benchmark::State& state_p) noexcept
{
	var::int16 l_first[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	var::int16 l_second[8]{};

	for (auto _ : state_p)
	{
		l_second[0] = l_first[0];
		l_second[1] = l_first[1];
		l_second[2] = l_first[2];
		l_second[3] = l_first[3];
		l_second[4] = l_first[4];
		l_second[5] = l_first[5];
		l_second[6] = l_first[6];
		l_second[7] = l_first[7];

		l_first[0] = l_second[0];
		l_first[1] = l_second[1];
		l_first[2] = l_second[2];
		l_first[3] = l_second[3];
		l_first[4] = l_second[4];
		l_first[5] = l_second[5];
		l_first[6] = l_second[6];
		l_first[7] = l_second[7];
	}
}
BENCHMARK(FE_128bit_copy_benchmark);

void FE_xmmword_copy_benchmark(benchmark::State& state_p) noexcept
{
	FE::xmmword l_xmmword;
	var::int16 l_int16[8]{ 1, 2, 3, 4, 5, 6, 7, 8 };
	l_xmmword.storeu(l_int16);

	FE::xmmword l_xmmword2;

	for (auto _ : state_p)
	{
		l_xmmword2.store(l_xmmword.load());
		l_xmmword.store(l_xmmword2.load());
	}
}
BENCHMARK(FE_xmmword_copy_benchmark);




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
BENCHMARK(FE_aligned_memcpy_benchmark);

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
BENCHMARK(std_memcpy_benchmark);


void FE_aligned_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		FE::memmove<FE::Address::_Aligned>(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(FE_aligned_memmove_benchmark);

void std_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		std::memmove(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(std_memmove_benchmark);


void FE_aligned_memzero_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		FE::memzero<FE::Address::_Aligned>(l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memzero_benchmark);

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
BENCHMARK(std_memset_benchmark);


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
BENCHMARK(FE_allocate_unique_single_benchmark);

void std_allocate_unique_single_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int> l_ptr = std::make_unique<int>();
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_single_benchmark);




void FE_allocate_unique_single_with_args_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<int> l_ptr = FE::allocate_unique<int>(std::pmr::get_default_resource(), 42);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(FE_allocate_unique_single_with_args_benchmark);

void std_allocate_unique_single_with_args_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int> l_ptr = std::make_unique<int>(42);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_single_with_args_benchmark);




// Benchmark for FE::unique_ptr<int[]> vs std::unique_ptr<int[]>
void FE_allocate_unique_array_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::unique_ptr<int[]> l_ptr = FE::allocate_unique<int[]>(10);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(FE_allocate_unique_array_benchmark);

void std_allocate_unique_array_benchmark(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		std::unique_ptr<int[]> l_ptr = std::make_unique<int[]>(10);
		benchmark::DoNotOptimize(l_ptr);
	}
}
BENCHMARK(std_allocate_unique_array_benchmark);








// Test cases for smart_ptr<T, RefType::_Owner>
TEST(SmartPtrOwner, DefaultConstructor)
{
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr;
	EXPECT_TRUE(l_ptr.is_null());
}

TEST(SmartPtrOwner, ConstructorWithAllocatorAndArgs)
{
	std::pmr::monotonic_buffer_resource l_resource;
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr(&l_resource, 42);
	EXPECT_EQ(*l_ptr, 42);
}

TEST(SmartPtrOwner, MoveConstructor)
{
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr1(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr2(std::move(l_ptr1));
	EXPECT_EQ(*l_ptr2, 42);

	EXPECT_TRUE(l_ptr1.is_null());
}

TEST(SmartPtrOwner, MoveAssignment)
{
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr1(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr2;
	l_ptr2 = std::move(l_ptr1);
	EXPECT_EQ(*l_ptr2, 42);

	EXPECT_TRUE(l_ptr1.is_null());
}

TEST(SmartPtrOwner, Reset)
{
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr(std::pmr::get_default_resource(), 42);
	l_ptr.reset();

	EXPECT_TRUE(l_ptr.is_null());
}

TEST(SmartPtrOwner, Swap)
{
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr1(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Owner> l_ptr2(std::pmr::get_default_resource(), 24);
	l_ptr1.swap(l_ptr2);
	EXPECT_EQ(*l_ptr1, 24);
	EXPECT_EQ(*l_ptr2, 42);
}




// Test cases for smart_ptr<T, RefType::_Observer>
TEST(SmartPtrObserver, DefaultConstructor)
{
	FE::smart_ptr<int, FE::RefType::_Observer> l_ptr;
	EXPECT_TRUE(l_ptr.is_expired());
}

TEST(SmartPtrObserver, ConstructorWithOwner)
{
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr(owner_ptr);
	EXPECT_EQ(*observer_ptr, 42);
}

TEST(SmartPtrObserver, MoveConstructor)
{
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr1(owner_ptr);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr2(std::move(observer_ptr1));
	EXPECT_EQ(*observer_ptr2, 42);
	EXPECT_TRUE(observer_ptr1.is_null());
}

TEST(SmartPtrObserver, MoveAssignment)
{
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr1(owner_ptr);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr2;
	observer_ptr2 = std::move(observer_ptr1);
	EXPECT_EQ(*observer_ptr2, 42);
	EXPECT_TRUE(observer_ptr1.is_null());
}

TEST(SmartPtrObserver, Reset)
{
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr(owner_ptr);
	observer_ptr.reset();
	EXPECT_TRUE(observer_ptr.is_null());
}

TEST(SmartPtrObserver, Swap)
{
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr1(std::pmr::get_default_resource(), 42);
	FE::smart_ptr<int, FE::RefType::_Owner> owner_ptr2(std::pmr::get_default_resource(), 24);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr1(owner_ptr1);
	FE::smart_ptr<int, FE::RefType::_Observer> observer_ptr2(owner_ptr2);
	observer_ptr1.swap(observer_ptr2);
	EXPECT_EQ(*observer_ptr1, 24);
	EXPECT_EQ(*observer_ptr2, 42);
}
