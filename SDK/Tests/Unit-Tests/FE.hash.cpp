//
//#include <gtest/gtest.h>
//#include <benchmark/benchmark.h>
//// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
//#include <FE/hash.hpp>
//
//// robin hood
//#include <robin_hood.h>
//
//
//
//void std_hash_benchmark(benchmark::State& state_p)
//{
//	std::hash<std::string> l_hasher;
//	static std::string l_string = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";
//
//	for (auto _ : state_p)
//	{
//		auto l_result = l_hasher(l_string);
//		(void)l_result;
//	}
//}
//
//BENCHMARK(std_hash_benchmark);
//
//
//void city_hash_benchmark(benchmark::State& state_p)
//{
//	auto l_content = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";
//	size_t l_length = strlen(l_content);
//
//
//	for (auto _ : state_p)
//	{
//		auto l_result = CityHash64(l_content, l_length);
//		(void)l_result;
//	}
//}
//
//BENCHMARK(city_hash_benchmark);
//
//
//void robin_hood_hash_benchmark(benchmark::State& state_p)
//{
//	const char* l_content = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";
//	size_t l_length = strlen(l_content);
//
//	for (auto _ : state_p)
//	{
//		auto l_result = robin_hood::hash_bytes(l_content, l_length);
//		(void)l_result;
//	}
//}
//
//BENCHMARK(robin_hood_hash_benchmark);