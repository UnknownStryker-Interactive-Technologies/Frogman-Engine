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
