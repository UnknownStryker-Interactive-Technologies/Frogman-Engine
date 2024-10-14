
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/hash.hpp>

// robin hood
#include <robin_hood.h>

/*
Profile Guided Optimization: https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170

TO DO: 
1. variable table
                                 
																														  |--- int32 variable hash map
																														  |--- float64 variable hash map
																														  |--- FE::string variable hash map
std::unordered_map<Key: type name string, Value: std::unordered_map<Key: varaible name string, Value: varaible address>> -|--- FE::array variable hash map
																														  |--- std::list variable hash map
																														  |--- std::deque variable hash map
2. memory layout reflection for serialization  																		      |--- etc...
                                                                                      *
	                                                                                  |
	Shallower <----- Memory Hierarchy Depth ----> Deeper                      Lower Memory Address
                                                      |----------------|              |
	                                                  | - FE::string - |              |
	|----------------------------------|    ----------|FE::smart_ptr   |              |
	|  Target Entry Non-Trivial Object |    |         |length, capacity|              |
	|- member variables -              |    |         |----------------|              |
	|  FE::string m_raw_name --------------|----|                                         |
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


void robin_hood_hash_benchmark(benchmark::State& state_p)
{
	const char* l_content = "_FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ count<char> count_chars(const char* string_p, const char target_p) noexcept";
	size_t l_length = strlen(l_content);

	for (auto _ : state_p)
	{
		auto l_result = robin_hood::hash_bytes(l_content, l_length);
		(void)l_result;
	}
}

BENCHMARK(robin_hood_hash_benchmark);