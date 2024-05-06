#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <FE/framework/reflection/function_table.hpp>
#include <FE/core/function.hxx>




FE::boolean Jesus_loves_you() noexcept
{
	//std::cout << "Jesus loves you!" << std::endl;
	return true;
}




// https://learn.microsoft.com/en-us/cpp/build/reference/gf-eliminate-duplicate-strings?view=msvc-170
TEST(function_table, general)
{
	FE::framework::function_table::register_method<FE::c_style_task<FE::boolean(void)>>("FE::boolean Jesus_loves_you() noexcept", Jesus_loves_you);
	EXPECT_TRUE(FE::framework::function_table::check_presence("FE::boolean Jesus_loves_you() noexcept"));

	FE::framework::function_table::invoke("FE::boolean Jesus_loves_you() noexcept");
	auto l_fn = FE::framework::function_table::retrieve("FE::boolean Jesus_loves_you() noexcept");
	auto l_result = l_fn->get_result();
	EXPECT_TRUE(l_result.has_value());
	
	_DISCARD_ auto l_actual_result = std::any_cast<FE::boolean>(l_result);
	EXPECT_TRUE(l_actual_result);
}




void FE_function_table_invocation_overhead_benchmark(benchmark::State& state_p) noexcept
{
	FE_ASSERT(FE::framework::function_table::check_presence("FE::boolean Jesus_loves_you() noexcept") == false, "Could not find the target function.");


	for(auto _ : state_p)
	{
		FE::framework::function_table::invoke("FE::boolean Jesus_loves_you() noexcept");
	}
}

BENCHMARK(FE_function_table_invocation_overhead_benchmark);