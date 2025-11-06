#ifndef _FE_FREE_PROCESSORS_HXX_
#define _FE_FREE_PROCESSORS_HXX_
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

#include <concurrent_queue.h>

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


enum struct TaskType : var::uint32
{
	_Urgent = 0,
	_Ordinary = 1,
	_Trivial = 2
};

class processor;
class processors;

class task
{
	friend class processors;

public:
	using notifier = boost::fibers::promise<void>;
	using handle = boost::fibers::future<void>;

private:
	std::shared_ptr<notifier> m_notifier;

public:
	TaskType _priority;
	FE::system _system;
	FE::component_base* _component;

public:
	task() noexcept = default;
	~task() noexcept = default;

	task(const task& other_p) noexcept;
	task& operator=(const task& other_p) noexcept;

	task(task&& other_p) noexcept;
	task& operator=(task&& other_p) noexcept;

	_FE_FORCE_INLINE_ FE::boolean is_waitable() const noexcept { return (m_notifier != nullptr); }
	_FE_FORCE_INLINE_ void notify_completion() noexcept
	{
		if (m_notifier != nullptr)
		{
			m_notifier->set_value();
		}
	}
};

class task_queue
{
public:
	using queue_type = concurrency::concurrent_queue<task, std::pmr::polymorphic_allocator<task>>;
	using value_type = queue_type::value_type;
	using size_type = queue_type::size_type;

private:
	queue_type m_urgent_tasks;
	queue_type m_ordinary_tasks;
	queue_type m_trivial_tasks;

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
}




class processor
{
	class processors* m_host;
	boost::thread m_processor;
	std::atomic_bool m_should_terminate;
	var::uint16 m_fibers_per_thread;
	var::uint64 m_yield_status;
	task_queue m_queue;
	internal::processors::fiber_stack_allocator m_fiber_stack_allocator;

	std::unique_ptr<boost::fibers::fiber[]> m_fibers;
	std::unique_ptr<var::float64[]> m_delta_ms;

	boost::condition_variable m_condition_variable;


public:
	processor() noexcept;
	~processor() noexcept;

	void fork(processors& host_p, FE::uint16 fibers_per_thread_p = 3, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	void join() noexcept;
	void schedule_task(const framework::task& task_p) noexcept;

	_FE_FORCE_INLINE_ FE::boolean should_terminate() const noexcept { return m_should_terminate.load(std::memory_order_acquire); }
	_FE_FORCE_INLINE_ FE::float64 get_delta_milliseconds(FE::int32 fiber_index_p) const noexcept { return m_delta_ms[fiber_index_p]; }
	_FE_FORCE_INLINE_ void wake() noexcept { m_condition_variable.notify_one(); }

private:
	static void __fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept;

public:
	processor(const processor&) = delete;
	processor& operator=(const processor&) = delete;
};




class game_thread
{
	friend class processor;
	using game_system_exec_table = std::pmr::vector< FE::pair<	FE::system, // the system function pointer
																std::pmr::vector< std::pmr::forward_list<FE::internal::ECS::components>* > // the list of components the system will operate on
																>
													>;
	framework::ECS& m_ecs;
	internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
	std::atomic_bool m_should_terminate;
	boost::fibers::fiber m_game_fiber;
	var::float64 m_delta_ms;

	boost::fibers::fiber m_gc_fiber;
	boost::fibers::fiber m_gc_reachability_analysis_fiber;
	var::float64 m_gc_delta_ms;
	var::uint64 m_gc_iter_per_frame;

	game_system_exec_table m_game_systems; // the element index is the system execution order

public:
	game_thread(framework::ECS& ecs_p, var::uint64 gc_batch_count_p = 100, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	~game_thread() noexcept = default;

public:
	void run() noexcept;
	void shutdown() noexcept;

private:
	_FE_FORCE_INLINE_ FE::float64 get_delta_milliseconds() const noexcept { return m_delta_ms; }
	_FE_FORCE_INLINE_ FE::float64 get_gc_delta_milliseconds() const noexcept { return m_gc_delta_ms; }

	static void __game_main(game_thread* const host_p) noexcept;
	static void __gc_main(game_thread* const host_p) noexcept;
	static void __reachability_analysis_main(game_thread* const host_p) noexcept;
	void __reachability_analysis_recursive(FE::component_view<FE::component_base> root_p, FE::component_view<FE::component_base> child_p) noexcept; // TODO: refactor to be non-recursive
};




class processors
{
	friend class processor;
	using game_system_exec_table = std::pmr::vector< FE::pair<	FE::system, // the system function pointer
																std::pmr::vector< std::pmr::forward_list<FE::internal::ECS::components>* > // the list of components the system will operate on
																>
													>;
	FE::uint32 m_concurrency;
	FE::uint32 m_fiber_host_count;
	FE::uint16 m_fibers_per_thread;
	std::unique_ptr<processor[]> m_processors;
	internal::processors::fiber_stack_allocator m_fiber_stack_allocator;

	game_thread m_game_thread;
	boost::thread m_renderer_thread;
	boost::thread m_physics_thread;
	boost::thread m_audio_thread;
	boost::thread m_networking_thread;

public:
	processors(framework::ECS& ecs_p, FE::int32 concurrency_p, FE::uint16 fibers_per_thread_p = 3, FE::uint32 gc_batch_count_p = 100, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	~processors() noexcept = default;

	void run(	FE::system renderer_p, FE::component_base* renderer_args_p,
				FE::system physics_p, FE::component_base* physics_args_p,
				FE::system audio_p, FE::component_base* audio_args_p,
				FE::system networking_p, FE::component_base* networking_args_p) noexcept;
	void schedule_task(const framework::task& task_p) noexcept;
	typename task::handle schedule_waitable_task(framework::task& task_p) noexcept;
	void shutdown() noexcept;

public:
	processors(const processors&) = delete;
	processors& operator=(const processors&) = delete;
};


END_NAMESPACE
#endif