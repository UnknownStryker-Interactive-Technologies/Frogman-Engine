#ifndef _FE_PROCESSORS_HXX_
#define _FE_PROCESSORS_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/framework/fiber.hxx>

#include <atomic>
#include <condition_variable>
#include <mutex>




CLASS_FORWARD_DECLARATION(FE, mutex)
CLASS_FORWARD_DECLARATION(FE, shared_mutex)




BEGIN_NAMESPACE(FE)


class processors final
{
	friend class FE::mutex;
	friend class FE::shared_mutex;

	std::unique_ptr<fiber_scheduler[]> m_scheduler;
	FE::int32 m_max_workers;
	var::int32 m_owned_workers; // workers [0, m_owned_workers) run one task each and are left out of the round robin
	FE::int32 m_fibers_per_thread;
	FE::size m_stack_size;
	std::atomic_bool m_should_terminate;
	std::unique_ptr<std::thread[]> m_threads;
	// One per worker thread, each on its own cache line so that waking one thread does not invalidate another's line.
	// A schedule call raises the thread's enqueue count and notifies under its lock, and a worker that found no task
	// sleeps until the count differs from the one it read before looking, so no wake-up is lost.
#pragma warning(push)
#pragma warning(disable:4324) // structure was padded due to alignment specifier
	struct alignas(FE::CPU_L1_cache_line::size) anesthesia
	{
		std::condition_variable _cv;
		std::mutex _lock;
		std::atomic<var::uint64> _enqueue_count = 0;
	};
#pragma warning(pop)
	std::unique_ptr<anesthesia[]> m_anesthesia;

public:
	processors(FE::int32 max_workers_p, FE::int32 fibers_per_thread_p, FE::size stack_size_p) noexcept;
	~processors() noexcept = default;

	// Thread-safe.
	void schedule_task(const task& task_p) noexcept;
	void schedule_tasks(const fiber_flow& tasks_p) noexcept;

	FE::awaitable schedule_task_async(const task& task_p) noexcept;
	FE::awaitable schedule_tasks_async(const fiber_flow& tasks_p) noexcept;

	// Not thread-safe; call it before any other schedule call. Gives task_p a worker thread of its own: the schedule
	// calls above never queue a task on that thread, so a task that does not return (the renderer's main loop) cannot
	// hold back the tasks queued behind it. At least one worker thread must be left for the other tasks.
	void schedule_thread_owning_task(const task& task_p) noexcept;

	// Not thread-safe; must not be called concurrently.
	void activate() noexcept;

	// Thread-safe. Calling it from any thread will signal the fiber scheduler to terminate as soon as possible.
	void terminate() noexcept;

private:
	// Wakes the worker thread thread_index_p after a task was queued for it.
	void __wake(FE::uint32 thread_index_p) noexcept;
};


END_NAMESPACE
#endif