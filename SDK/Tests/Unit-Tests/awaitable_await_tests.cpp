#include "fiber_test_runtime.hxx"

#include <atomic>
#include <memory_resource>
#include <utility>
#include <vector>


namespace
{
	struct await_data
	{
		std::atomic_bool _allow_completion = false;
		std::atomic_int _value = 0;
		int _observed_value = 0;
	};

	void run_awaited_task(void* argument) noexcept
	{
		await_data* l_data = static_cast<await_data*>(argument);
		while (!l_data->_allow_completion.load(std::memory_order_acquire))
		{
			FE::fiber_scheduler::yield();
		}
		l_data->_value.fetch_add(1, std::memory_order_relaxed);
	}

	void run_other_work(void* argument) noexcept
	{
		static_cast<await_data*>(argument)->_allow_completion.store(true, std::memory_order_release);
	}
}


TEST_F(fiber_test_runtime, ScheduleTaskAsyncAwait)
{
	await_data data;
	run([&]()
	{
		FE::task task{ run_awaited_task, &data, TaskPriority::_Normal };
		FE::awaitable awaitable = scheduler(1).schedule_task_async(task);

		task._system = run_other_work;
		scheduler(0).schedule_task(task);

		awaitable.await();
		data._observed_value = data._value.load(std::memory_order_relaxed);
	});
	EXPECT_EQ(data._observed_value, 1);
}


TEST_F(fiber_test_runtime, ScheduleTasksAsyncAwait)
{
	await_data data;
	run([&]()
 	{
		FE::task task{ run_awaited_task, &data, TaskPriority::_Normal };
		std::pmr::vector<FE::task> tasks{ task, task };
		FE::awaitable awaitable = scheduler(1).schedule_tasks_async(std::move(tasks));

		task._system = run_other_work;
		scheduler(0).schedule_task(task);

		awaitable.await();
		data._observed_value = data._value.load(std::memory_order_relaxed);
	});
	EXPECT_EQ(data._observed_value, 2);
}
