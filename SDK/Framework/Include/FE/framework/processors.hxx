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
#include <FE/pair.hxx>

#include <FE/framework/ECS.hxx>

#include <concurrent_priority_queue.h>

#include <boost/fiber/fiber.hpp> // boost::fibers::fiber
#include <boost/fiber/future/future.hpp> // boost::fibers::future
#include <boost/fiber/future/promise.hpp> // boost::fibers::promise
#include <boost/fiber/recursive_mutex.hpp> // boost::fibers::recursive_mutex

#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

#include <FE/memory.hxx> // FE::unique_ptr
#include <FE/pool/memory_resource.hxx> // FE::memory_resource




BEGIN_NAMESPACE(FE::framework)


class processor;
class processors;


enum struct TaskType : var::int32
{
	_RealTime = 1,
	_Urgent = 2,
	_Ordinary = 3,
	_Trivial = 4
};


class task
{
	friend class processors;
public:
	using notifier = boost::fibers::promise<void>;
	using handle = boost::fibers::future<void>;

private:
	mutable std::shared_ptr<notifier> m_notifier;

public:
	FE::system _system;
	FE::component_base* _component;
	TaskType _task_type;

	// helper functions
	_FE_FORCE_INLINE_ FE::boolean is_waitable() const noexcept { return (m_notifier != nullptr); }
	_FE_FORCE_INLINE_ void notify_completion() noexcept
	{
		if (m_notifier != nullptr)
		{
			m_notifier->set_value();
		}
	}

	_FE_FORCE_INLINE_ friend bool operator>(const task& lhs_p, const task& rhs_p) noexcept
	{
		return static_cast<var::int32>(lhs_p._task_type) > static_cast<var::int32>(rhs_p._task_type);
	}
};


class task_queue
{
public:
	using queue_type = concurrency::concurrent_priority_queue<task, std::greater<task>, std::pmr::polymorphic_allocator<task>>;
	using value_type = queue_type::value_type;
	using size_type = queue_type::size_type;

private:
	queue_type m_queue;

public:
	task_queue(std::pmr::memory_resource* const memory_resource_p) noexcept;
	~task_queue() noexcept = default;

	void push(const framework::task& task_p) noexcept;
	FE::boolean try_pop(framework::task& out_task_p) noexcept;
};




namespace internal::processors
{
	class fiber_stack_allocator // uses the framework TLGPMP
	{
		std::size_t m_size;

	public:
		using traits_type = boost::fibers::fixedsize_stack;

		fiber_stack_allocator() noexcept = default;
		~fiber_stack_allocator() noexcept = default;

		fiber_stack_allocator(std::size_t size = FE::one_MiB) noexcept;
		boost::context::stack_context allocate() noexcept;
		void deallocate(boost::context::stack_context& sctx) noexcept;

		std::size_t stack_size() const noexcept { return m_size; }
	};


	struct fiber
	{
		boost::fibers::fiber _fiber;
		var::float64 _delta_ms = 0.0;
		TaskType _current_task_type = TaskType::_Trivial;
	};


	class processor
	{
		class processors* m_host;
		boost::thread m_processor;
		std::atomic_bool m_should_terminate;
		var::uint16 m_fibers_per_thread;
		var::uint64 m_yield_status;
		task_queue m_queue;
		internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
		std::pmr::vector<fiber> m_fibers;

		boost::condition_variable m_condition_variable;

	public:
		processor() noexcept;
		~processor() noexcept;

		void fork(::FE::framework::processors& host_p, FE::uint16 fibers_per_thread_p = 3, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
		void join() noexcept;

		_FE_FORCE_INLINE_ void schedule_task(const framework::task& task_p) noexcept
		{
			FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
			m_queue.push(task_p);
		}

		_FE_FORCE_INLINE_ FE::boolean should_terminate() const noexcept { return m_should_terminate.load(std::memory_order_acquire); }
		_FE_FORCE_INLINE_ FE::float64 get_delta_milliseconds(FE::int32 fiber_index_p) const noexcept { return m_fibers[fiber_index_p]._delta_ms; }
		_FE_FORCE_INLINE_ void wake() noexcept { m_condition_variable.notify_one(); }

	private:
		static void __fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept;

	public:
		processor(const processor&) = delete;
		processor& operator=(const processor&) = delete;
	};
}


class thread
{
	boost::thread m_host;

public:
	thread() noexcept = default;
	~thread() noexcept = default;

	void fork(FE::system system_p, FE::component_base* const arguments_p = nullptr, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	void join() noexcept;

private:
	static void __thread_main(FE::system system_p, FE::component_base* const arguments_p, FE::size fiber_stack_size_p) noexcept;
};


class processors
{
	friend class processor;

	FE::uint16 m_concurrency;
	FE::uint16 m_fibers_per_thread;
	FE::uint32 m_fiber_stack_size;
	std::unique_ptr<internal::processors::processor[]> m_processors; 

public:
	processors(FE::int32 concurrency_p, FE::uint16 fibers_per_thread_p = 3, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	~processors() noexcept = default;

	void run() noexcept;
	void schedule_task(const framework::task& task_p) noexcept;
	typename task::handle schedule_waitable_task(const framework::task& task_p) noexcept;
	void shutdown() noexcept;

public:
	processors(const processors&) = delete;
	processors& operator=(const processors&) = delete;
};


END_NAMESPACE
#endif