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