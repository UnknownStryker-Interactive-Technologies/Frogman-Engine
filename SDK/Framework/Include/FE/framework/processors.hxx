#ifndef _FE_FRAMEWORK_PROCESSORS_HXX_
#define _FE_FRAMEWORK_PROCESSORS_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

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


// TODO: implement fiber based future and promise!
BEGIN_NAMESPACE(FE::framework)


class processors final
{
	friend class FE::mutex;
	friend class FE::shared_mutex;

	std::unique_ptr<fiber_scheduler[]> m_scheduler;
	FE::int32 m_max_workers;
	FE::int32 m_fibers_per_thread;
	FE::size m_stack_size;
	std::atomic_bool m_should_terminate;
	std::unique_ptr<std::thread[]> m_threads;
	std::unique_ptr<std::condition_variable[]> m_anesthetic_cv;

public:
	processors(FE::int32 max_workers_p, FE::int32 fibers_per_thread_p, FE::size stack_size_p) noexcept;
	~processors() noexcept = default;

	// Thread-safe.
	void schedule_task(const task& task_p) noexcept;

	// Not thread-safe; must not be called concurrently.
	void execute() noexcept;

	// Thread-safe. Calling it from any thread will signal the fiber scheduler to terminate as soon as possible.
	void terminate() noexcept;
};


END_NAMESPACE
#endif