/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/framework/processors.hxx>

#include <FE/framework/framework.hxx>

#include <thread>




BEGIN_NAMESPACE(FE::framework)


processors::processors(FE::int32 max_workers_p, FE::int32 fibers_per_thread_p, FE::size stack_size_p) noexcept
	:	m_scheduler(),
		m_max_workers(max_workers_p),
		m_fibers_per_thread(fibers_per_thread_p),
		m_stack_size(stack_size_p),
		m_should_terminate(false)
{
	FE_ASSERT(max_workers_p > 0, "Assertion Failure: the number of worker threads must be greater than 0.");
	FE_ASSERT((fibers_per_thread_p >= 2) && (fibers_per_thread_p <= 8), "Assertion Failure: the number of fibers per thread must be between 2 and 8.");
	FE_ASSERT(stack_size_p >= FE::system_page_size, "Assertion Failure: the stack size must be greater than or equal to the system page size.");

	m_scheduler = std::make_unique<fiber_scheduler[]>(max_workers_p);
	FE_ASSERT(m_scheduler != nullptr);

	for (var::int32 t = 0; t < max_workers_p; ++t)
	{
		for (var::int32 f = 0; f < fibers_per_thread_p; ++f)
		{
			m_scheduler[t].create_fiber(stack_size_p);
		}
	}
}




void processors::schedule_task(const task& task_p) noexcept
{
	static std::atomic<var::uint32> l_s_next_thread_index = 0; // value overflow is intended

	FE::uint32 l_current_thread_index = l_s_next_thread_index.fetch_add(1, std::memory_order_acq_rel) % m_max_workers;
	m_scheduler[ l_current_thread_index ].schedule_task(task_p);
	m_anesthetic_cv[l_current_thread_index].notify_one(); // resuscitate the thread from anesthesia
}




// not thread-safe.
void processors::execute() noexcept
{
	if (m_threads != nullptr)
	{
		return; // already executing
	}

	m_threads = std::make_unique<std::thread[]>(m_max_workers);
	m_anesthetic_cv = std::make_unique<std::condition_variable[]>(m_max_workers);

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		m_threads[t] = std::thread
		(
			[this, t]()
			{
				FE::fiber_scheduler::tl_s_this_thread_fiber_scheduler = &m_scheduler[t];
				while (m_should_terminate.load(std::memory_order_acquire) == false)
				{
					const int l_result = m_scheduler[t].execute();

					if (l_result == _FE_FAILED_) // no tasks to execute, put the thread to sleep for a while to avoid busy-waiting.
					{
						std::mutex l_anesthetic;
						std::unique_lock<std::mutex> l_lock(l_anesthetic);
						m_anesthetic_cv[t].wait(l_lock);
					}
				}
				FE::fiber_scheduler::tl_s_this_thread_fiber_scheduler = nullptr; // reset the thread-local pointer to the fiber scheduler before exiting the thread.
			}

		);
	}
}


void processors::terminate() noexcept
{
	m_should_terminate.store(true, std::memory_order_release);

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		m_anesthetic_cv[t].notify_all(); // resuscitate the thread from anesthesia
	}

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		if (m_threads[t].joinable() == true)
		{
			m_threads[t].join();
		}
	}
}


END_NAMESPACE