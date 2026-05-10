#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/hash.hxx>

#include <absl/container/node_hash_map.h>




TEST(absl, node_hash_map)
{
	constexpr size_t capacity = 1000;

	absl::node_hash_map<int, void*> map;
	map.reserve(capacity); // Reserve space for 100k elements

	for(int i = 0; i < capacity; ++i)
	{
		// Insert and delete repeatedly
		for (int j = 0; j < capacity; ++j)
		{
			map.emplace(j, nullptr);
		}

		FE_ASSERT(map.size() == capacity);

		for (int j = 0; j < capacity; ++j)
		{
			map.erase(j);
		}
	}
}




void std_hash_benchmark(benchmark::State& state_p)
{
	std::hash<std::string> l_hasher;
	static std::string l_string = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";

	for (auto _ : state_p)
	{
		auto l_result = l_hasher(l_string);
		(void)l_result;
	}
}

BENCHMARK(std_hash_benchmark);


void city_hash_benchmark(benchmark::State& state_p)
{
	auto l_content = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";
	size_t l_length = strlen(l_content);


	for (auto _ : state_p)
	{
		auto l_result = CityHash64(l_content, l_length);
		(void)l_result;
	}
}

BENCHMARK(city_hash_benchmark);











static void std_unordered_map_insertion(benchmark::State& state)
{
	std::unordered_map<int, int> l_map;
	for (auto _ : state)
	{
		l_map.insert({ 42, 3 });
	}
}
BENCHMARK(std_unordered_map_insertion);

static void std_pmr_unordered_map_insertion(benchmark::State& state)
{
	std::pmr::unsynchronized_pool_resource resource;
	std::pmr::unordered_map<int, int> l_map(&resource);
	for (auto _ : state)
	{
		l_map.insert({ 42, 3 });
	}
}
BENCHMARK(std_pmr_unordered_map_insertion);





static void std_unordered_map_lookup(benchmark::State& state)
{
	std::unordered_map<int, int> l_map;
	l_map.insert({ 42, 3 });
	for (auto _ : state)
	{
		auto l_iterator = l_map.find(42);
		if (l_iterator != l_map.end())
		{
			benchmark::DoNotOptimize(l_iterator->second);
		}
	}
}
BENCHMARK(std_unordered_map_lookup);