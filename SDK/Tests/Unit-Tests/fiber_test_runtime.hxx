#pragma once
#include <gtest/gtest.h>
#include <FE/framework/fiber.hxx>

#include <array>
#include <atomic>
#include <latch>
#include <thread>


class fiber_test_runtime : public ::testing::Test
{
	std::atomic_bool m_stop = false;
	std::latch m_ready{ 2 };
	std::array<FE::fiber_scheduler*, 2> m_schedulers = {};
	std::array<std::thread, 2> m_threads;

protected:
	fiber_test_runtime()
	{
		for (std::size_t i = 0; i < m_threads.size(); ++i)
		{
			m_threads[i] = std::thread([this, i]()
			{
				FE::fiber_scheduler scheduler;
				scheduler.attach_to_current_thread();
				scheduler.create_fiber();
				scheduler.create_fiber();
				m_schedulers[i] = &scheduler;
				m_ready.count_down();

				while (!m_stop.load(std::memory_order_acquire))
				{
					_FE_MAYBE_UNUSED_ int result = scheduler.execute();
				}
				scheduler.detach_from_current_thread();
			});
		}
		m_ready.wait();
	}

	~fiber_test_runtime() override
	{
		stop_and_join();
	}

	FE::fiber_scheduler& scheduler(std::size_t index_p) noexcept
	{
		return *m_schedulers[index_p];
	}

	template<class Function>
	void run(Function function_p)
	{
		struct invocation
		{
			Function* _function;
			std::latch _done{ 1 };
		} data{ &function_p };

		FE::task task{
			[](void* argument) noexcept
			{
				auto* l_data = static_cast<invocation*>(argument);
				(*l_data->_function)();
				l_data->_done.count_down();
			},
			&data,
			TaskPriority::_Normal
		};

		scheduler(0).schedule_task(task);
		data._done.wait();
		stop_and_join();
	}

private:
	void stop_and_join() noexcept
	{
		m_stop.store(true, std::memory_order_release);
		for (std::thread& thread : m_threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}
};
