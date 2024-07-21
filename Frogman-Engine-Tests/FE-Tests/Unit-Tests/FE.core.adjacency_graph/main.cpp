#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#include <FE/core/adjacency_graph.hxx>

// std
#include <array>
#include <map>
#include <utility>

// Set up GitHub CI pipeline.
/*
Profile Guided Optimization: https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170

TO DO: 
1. variable table
                                 
																														  |--- int32 variable hash map
																														  |--- float64 variable hash map
																														  |--- FE::string variable hash map
std::unordered_map<Key: type name string, Value: FE::adjacency_graph<Value: varaible_meta_data>> -|--- FE::array variable hash map
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
<<<<<<< HEAD
	|  FE::string m_raw_name --------------|----|                                         |
=======
	|  FE::string m_name --------------|----|                                         |
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
	                                           |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
													                                  |
																					  *
																					  
<<<<<<< HEAD
Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
=======
Memory Layer Traversal Order: Entry.FE::string m_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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
	FE_ABORT_IF(benchmark::ReportUnrecognizedArguments(argc_p, argv_p) == true, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
    int32 l_exit_code = RUN_ALL_TESTS();
	std::cerr << "\n\n";
	benchmark::RunSpecifiedBenchmarks();
	std::cerr << "\n\n";
    benchmark::Shutdown();
    return l_exit_code;
}


TEST(AdjacencyGraph, adjacent_node_iteration)
{
	{
		std::array<std::pair<var::size, const char*>, 5> l_answer_sheet = { std::pair<var::size, const char*>{3, "Pineapple"}, std::pair<var::size, const char*>{5, "Orange"}, std::pair<var::size, const char*>{6, "Rotten apple"}, std::pair<var::size, const char*>{7, "Mango"}, std::pair<var::size, const char*>{9, "Melon"} };
		FE::adjacency_graph<var::size, std::string> l_adjacency_graph = { {6, "Rotten apple"}, {3, "Pineapple"}, {7, "Mango"}, {9, "Melon"}, {5, "Orange"} };

		auto l_adjacency_graph_iterator = l_adjacency_graph.begin();
		auto l_adjacency_graph_end = l_adjacency_graph.end();
		for(var::size i = 0; l_adjacency_graph_iterator != l_adjacency_graph_end; ++i)
		{
			EXPECT_EQ(l_answer_sheet[i].second, l_adjacency_graph_iterator->_value);
			++l_adjacency_graph_iterator;
		}

		l_adjacency_graph.erase(l_adjacency_graph.begin());
	}
}


/*
run-tests.sh Release FE.core.adjacency_graph

2024-06-19T13:42:01+00:00
Running /workspace/Frogman-Engine-Lab/Frogman-Engine-Tests/FE-Tests/Unit-Tests/FE.core.adjacency_graph/FE.core.adjacency_graph_test
Run on (16 X 2799.99 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 512 KiB (x8)
  L3 Unified 16384 KiB (x2)
Load Average: 1.76, 1.96, 1.95
-----------------------------------------------------------------------------------------------------
Benchmark                                                           Time             CPU   Iterations
-----------------------------------------------------------------------------------------------------
FE_adjacency_graph_adjacent_node_iteration_speed_benchmark       3.41 ns         3.41 ns    204651939
FE_map_adjacent_node_iteration_speed_benchmark                   13.6 ns         13.6 ns     50113856
*/
void FE_adjacency_graph_adjacent_node_iteration_speed_benchmark(benchmark::State& state_p) noexcept
{
	FE::adjacency_graph<var::size, std::string> l_adjacency_graph = { {6, "Rotten apple"}, {3, "Pineapple"}, {7, "Mango"}, {9, "Melon"}, {5, "Orange"} };
	
	for (_MAYBE_UNUSED_ auto _ : state_p)
	{
		auto l_end = l_adjacency_graph.end();
		for(auto begin = l_adjacency_graph.begin(); begin != l_end; ++begin)
		{
			benchmark::DoNotOptimize(begin);
		}
	}
}
BENCHMARK(FE_adjacency_graph_adjacent_node_iteration_speed_benchmark);


void FE_map_adjacent_node_iteration_speed_benchmark(benchmark::State& state_p) noexcept
{
	
	std::map<var::size, std::string> l_map = { {6, "Rotten apple"}, {3, "Pineapple"}, {7, "Mango"}, {9, "Melon"}, {5, "Orange"} };
	
	for (_MAYBE_UNUSED_ auto _ : state_p)
	{
		auto l_end = l_map.end();
		for(auto begin = l_map.begin(); begin != l_end; ++begin)
		{
			benchmark::DoNotOptimize(begin);
		}
	}
}
BENCHMARK(FE_map_adjacent_node_iteration_speed_benchmark);