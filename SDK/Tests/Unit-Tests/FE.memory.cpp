#include <gtest/gtest.h>
#include <benchmark/benchmark.h>


// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/memory.hxx>
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

TEST(memset, General)
{
	constexpr auto l_length = 10;
	var::byte l_arr[l_length];
	
	FE::memset(l_arr, 0, l_length * sizeof(var::byte));
		
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
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_dest, l_source, _MAGICAL_SIZE_);
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memcpy_benchmark);


#include <immintrin.h> // For SIMD intrinsics
#include <cstddef> // For size_t
#include <cstdint> // For uintptr_t
// This is a port from Frogman-Engine\SDK\Experimental\asm\__x86_64_memcpy.asm.
__forceinline void __x86_64_memcpy(void* dest_p, const void* source_p, size_t bytes_to_copy_p) noexcept {
	const uint8_t* src = static_cast<const uint8_t*>(source_p);
	uint8_t* dest = static_cast<uint8_t*>(dest_p);

	// SIMD case for larger sizes
	while (bytes_to_copy_p >= 32) {
		_mm256_store_si256(reinterpret_cast<__m256i*>(dest), _mm256_load_si256(reinterpret_cast<const __m256i*>(src)));
		src += 32;
		dest += 32;
		bytes_to_copy_p -= 32;
	}

	// Handle remaining bytes with a jump table
	switch (bytes_to_copy_p) {
	case 31: dest[30] = src[30];
	case 30: dest[29] = src[29];
	case 29: dest[28] = src[28];
	case 28: dest[27] = src[27];
	case 27: dest[26] = src[26];
	case 26: dest[25] = src[25];
	case 25: dest[24] = src[24];
	case 24: dest[23] = src[23];
	case 23: dest[22] = src[22];
	case 22: dest[21] = src[21];
	case 21: dest[20] = src[20];
	case 20: dest[19] = src[19];
	case 19: dest[18] = src[18];
	case 18: dest[17] = src[17];
	case 17: dest[16] = src[16];
	case 16:
		_mm_store_si128(reinterpret_cast<__m128i*>(dest), _mm_load_si128(reinterpret_cast<const __m128i*>(src)));
		src += 16;
		dest += 16;
		bytes_to_copy_p -= 16;
		break;
	case 15: dest[14] = src[14];
	case 14: dest[13] = src[13];
	case 13: dest[12] = src[12];
	case 12: dest[11] = src[11];
	case 11: dest[10] = src[10];
	case 10: dest[9] = src[9];
	case 9: dest[8] = src[8];
	case 8:
		*reinterpret_cast<uint64_t*>(dest) = *reinterpret_cast<const uint64_t*>(src);
		src += 8;
		dest += 8;
		bytes_to_copy_p -= 8;
		break;
	case 7: dest[6] = src[6];
	case 6: dest[5] = src[5];
	case 5: dest[4] = src[4];
	case 4:
		*reinterpret_cast<uint32_t*>(dest) = *reinterpret_cast<const uint32_t*>(src);
		src += 4;
		dest += 4;
		bytes_to_copy_p -= 4;
		break;
	case 3: dest[2] = src[2];
	case 2:
		*reinterpret_cast<uint16_t*>(dest) = *reinterpret_cast<const uint16_t*>(src);
		src += 2;
		dest += 2;
		bytes_to_copy_p -= 2;
		break;
	case 1:
		*dest = *src;
		break;
	case 0:
		return;
	}
}
void FE_memcpy_v2_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);

	for (auto _ : state_p)
	{
		__x86_64_memcpy(l_dest, l_source, _MAGICAL_SIZE_);
		__x86_64_memcpy(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_memcpy_v2_benchmark);

void std_memcpy_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);

	for (auto _ : state_p)
	{
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


void FE_aligned_memset_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		FE::memset<FE::Address::_Aligned>(l_dest, null, _MAGICAL_SIZE_);
		FE::memset<FE::Address::_Aligned>(l_dest, 1, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memset_benchmark);

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