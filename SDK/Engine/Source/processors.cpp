/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/processors.hxx>

#include <FE/framework/framework.hxx>

#include <thread>




BEGIN_NAMESPACE(FE)


processors::processors(FE::int32 max_workers_p, FE::int32 fibers_per_thread_p, FE::size stack_size_p) noexcept
	:	m_scheduler(),
		m_max_workers(max_workers_p),
		m_owned_workers(0),
		m_fibers_per_thread(fibers_per_thread_p),
		m_stack_size(stack_size_p),
		m_should_terminate(false)
{
	FE_ASSERT(max_workers_p > 0, "Assertion Failure: the number of worker threads must be greater than 0.");
	FE_ASSERT((fibers_per_thread_p >= 2) && (fibers_per_thread_p <= 8), "Assertion Failure: the number of fibers per thread must be between 2 and 8.");
	FE_ASSERT(stack_size_p >= FE::system_page_size, "Assertion Failure: the stack size must be greater than or equal to the system page size.");

	m_scheduler = std::make_unique<fiber_scheduler[]>(max_workers_p);
	m_anesthesia = std::make_unique<anesthesia[]>(max_workers_p);
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
	FE_ASSERT(m_owned_workers.load(std::memory_order_acquire) < m_max_workers, "Assertion Failure: every worker thread is owned by a thread-owning task; no worker is left to run this task.");

	static std::atomic<var::uint32> l_s_next_thread_index = 0; // value overflow is intended

	FE::uint32 l_current_thread_index = m_owned_workers.load(std::memory_order_acquire) + (l_s_next_thread_index.fetch_add(1, std::memory_order_acq_rel) % (m_max_workers - m_owned_workers.load(std::memory_order_acquire)));
	m_scheduler[ l_current_thread_index ].schedule_task(task_p);
	__wake(l_current_thread_index); // resuscitate the thread from anesthesia
}

void processors::schedule_tasks(const fiber_flow& tasks_p) noexcept
{
	FE_ASSERT(m_owned_workers.load(std::memory_order_acquire) < m_max_workers, "Assertion Failure: every worker thread is owned by a thread-owning task; no worker is left to run this task.");

	static std::atomic<var::uint32> l_s_next_thread_index = 0; // value overflow is intended

	tasks_p.sort_tasks_topologically(fiber_flow::auth{});
	FE::uint32 l_current_thread_index = m_owned_workers.load(std::memory_order_acquire) + (l_s_next_thread_index.fetch_add(1, std::memory_order_acq_rel) % (m_max_workers - m_owned_workers.load(std::memory_order_acquire)));
	m_scheduler[l_current_thread_index].schedule_tasks( std::move( tasks_p.get_topologically_ordered_tasks(fiber_flow::auth{}) ) );
	__wake(l_current_thread_index); // resuscitate the thread from anesthesia
}


FE::awaitable processors::schedule_task_async(const task& task_p) noexcept
{
	FE_ASSERT(m_owned_workers.load(std::memory_order_acquire) < m_max_workers, "Assertion Failure: every worker thread is owned by a thread-owning task; no worker is left to run this task.");

	static std::atomic<var::uint32> l_s_next_thread_index = 0; // value overflow is intended

	FE::uint32 l_current_thread_index = m_owned_workers.load(std::memory_order_acquire) + (l_s_next_thread_index.fetch_add(1, std::memory_order_acq_rel) % (m_max_workers - m_owned_workers.load(std::memory_order_acquire)));
	FE::awaitable l_handle = m_scheduler[l_current_thread_index].schedule_task_async(task_p);
	__wake(l_current_thread_index); // resuscitate the thread from anesthesia
	return l_handle;
}

FE::awaitable processors::schedule_tasks_async(const fiber_flow& tasks_p) noexcept
{
	FE_ASSERT(m_owned_workers.load(std::memory_order_acquire) < m_max_workers, "Assertion Failure: every worker thread is owned by a thread-owning task; no worker is left to run this task.");

	static std::atomic<var::uint32> l_s_next_thread_index = 0; // value overflow is intended

	tasks_p.sort_tasks_topologically(fiber_flow::auth{});
	FE::uint32 l_current_thread_index = m_owned_workers.load(std::memory_order_acquire) + (l_s_next_thread_index.fetch_add(1, std::memory_order_acq_rel) % (m_max_workers - m_owned_workers.load(std::memory_order_acquire)));
	FE::awaitable l_handle = m_scheduler[l_current_thread_index].schedule_tasks_async(std::move(tasks_p.get_topologically_ordered_tasks(fiber_flow::auth{})));
	__wake(l_current_thread_index); // resuscitate the thread from anesthesia
	return l_handle;
}


void processors::schedule_thread_owning_task(const task& task_p) noexcept
{
	FE_ASSERT(m_owned_workers.load(std::memory_order_acquire) + 1 < m_max_workers, "Assertion Failure: at least one worker thread must be left for the other tasks.");

	FE::uint32 l_thread_index = static_cast<FE::uint32>(m_owned_workers.load(std::memory_order_acquire));
	m_owned_workers.fetch_add(1, std::memory_order_acq_rel);
	m_scheduler[l_thread_index].schedule_task(task_p);
	__wake(l_thread_index); // resuscitate the thread from anesthesia
}


// not thread-safe.
void processors::activate() noexcept
{
	if (m_threads != nullptr)
	{
		return; // already executing
	}

	m_threads = std::make_unique<std::thread[]>(m_max_workers);

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		m_threads[t] = std::thread
		(
			[this, t]()
			{
				m_scheduler[t].attach_to_current_thread();
				while (m_should_terminate.load(std::memory_order_acquire) == false)
				{
					// Read the enqueue count before looking for a task: a task queued after execute() finds none raises it.
					FE::uint64 l_enqueue_count = m_anesthesia[t]._enqueue_count.load(std::memory_order_acquire);
					const int l_result = m_scheduler[t].execute();

					if (l_result == _FE_FAILED_) // no tasks to activate, put the thread to sleep for a while to avoid busy-waiting.
					{
						std::unique_lock<std::mutex> l_lock(m_anesthesia[t]._lock);
						m_anesthesia[t]._cv.wait(l_lock, [this, t, l_enqueue_count]() noexcept
						{
							return (m_anesthesia[t]._enqueue_count.load(std::memory_order_acquire) != l_enqueue_count) || m_should_terminate.load(std::memory_order_acquire);
						});
					}
				}
				m_scheduler[t].detach_from_current_thread(); // reset the thread-local pointer to the fiber scheduler before exiting the thread.
			}

		);
	}
}

bool processors::is_active() const noexcept
{
	return m_should_terminate.load(std::memory_order_acquire) == false;
}

void processors::terminate() noexcept
{
	m_should_terminate.store(true, std::memory_order_release);

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		{
			std::lock_guard<std::mutex> l_lock(m_anesthesia[t]._lock); // a thread between its check and its wait sees the flag
		}
		m_anesthesia[t]._cv.notify_all(); // resuscitate the thread from anesthesia
	}

	for (var::int32 t = 0; t < m_max_workers; ++t)
	{
		if (m_threads[t].joinable() == true)
		{
			m_threads[t].join();
		}
	}
}


void processors::yield_back_if_not_locked() noexcept
{
	FE::fiber_scheduler::yield_back_if_not_locked();
}

void processors::yield() noexcept
{
	FE::fiber_scheduler::yield();
}

bool processors::is_fiber() noexcept
{
	return FE::fiber_scheduler::get_current_fiber_scheduler()->is_fiber();
}

FE::fiber_scheduler* processors::get_current_fiber_scheduler() noexcept
{
	return FE::fiber_scheduler::get_current_fiber_scheduler();
}


void processors::__wake(FE::uint32 thread_index_p) noexcept
{
	m_anesthesia[thread_index_p]._enqueue_count.fetch_add(1, std::memory_order_acq_rel);
	{
		// Taking the lock orders this notify after a sleeping thread's check: either the thread sees the new count, or it
		// is already waiting and gets the notify.
		std::lock_guard<std::mutex> l_lock(m_anesthesia[thread_index_p]._lock);
	}
	m_anesthesia[thread_index_p]._cv.notify_one();
}


END_NAMESPACE