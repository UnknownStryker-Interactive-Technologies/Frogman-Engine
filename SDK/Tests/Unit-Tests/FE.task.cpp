#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <FE/framework/framework.hpp>




TEST(async_task, latent_task)
{
	FE::framework::framework_base::get_engine().get_task_scheduler().launch_latent_event(
		[]()
		{
			std::cerr << "\nHello Latent Event!\n"; 
		}, 1000 /*milliseconds*/
	);

	std::cerr << "\nHello Event!\n";
}