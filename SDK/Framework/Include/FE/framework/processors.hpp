#ifndef _FE_FREE_PROCESSORS_HPP_
#define _FE_FREE_PROCESSORS_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/pair.hxx>

#include <FE/framework/component_base.hpp>
#include <FE/framework/system.hpp>

#include <boost/fiber/fiber.hpp> // boost::fibers::fiber
#include <boost/fiber/buffered_channel.hpp>

#include <concurrent_priority_queue.h>

#include <thread> // std::thread




BEGIN_NAMESPACE(FE::framework)


struct task
{
	var::uint64 _priority;
	FE::system _system;
	FE::component_base* _component;
};

struct priority_comparator
{
	bool operator()(const task& lhs_p, const task& rhs_p) const noexcept
	{
		return lhs_p._priority < rhs_p._priority; // lower priority value means higher priority
	}
};

using task_queue = concurrency::concurrent_priority_queue<task, priority_comparator, std::pmr::polymorphic_allocator<task>>;

constexpr FE::int32 fibers_per_thread = 4;




namespace internal::processors
{
	class fiber_stack_allocator // uses the framework TLGPMP
	{
		std::size_t m_size;

	public:
		using traits_type = boost::fibers::fixedsize_stack;

		fiber_stack_allocator() noexcept = default;
		~fiber_stack_allocator() noexcept = default;

		fiber_stack_allocator(std::size_t size) noexcept;
		boost::context::stack_context allocate() noexcept;
		void deallocate(boost::context::stack_context& sctx) noexcept;

		std::size_t stack_size() const noexcept { return m_size; }
	};
}




class processor
{
	class processors* m_host;
	std::thread m_processor;
	internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
	std::atomic_bool m_is_running;

	boost::fibers::fiber m_fibers[fibers_per_thread];
	task_queue m_queue;
	var::float64 m_delta_time_milliseconds[fibers_per_thread];

public:
	processor() noexcept;
	processor(class processors& host_p, FE::size fiber_stack_size_p) noexcept;
	~processor() noexcept;

	void fork() noexcept;
	void join() noexcept;
	void enqueue_task(framework::task task_p) noexcept;
	_FE_FORCE_INLINE_ typename task_queue::size_type count_remaining_tasks() noexcept { return m_queue.size(); }

	_FE_FORCE_INLINE_ FE::boolean is_running() const noexcept { return m_is_running.load(std::memory_order_acquire); }
	_FE_FORCE_INLINE_ FE::float64 get_delta_time_milliseconds(FE::int32 fiber_index_p) const noexcept { return m_delta_time_milliseconds[fiber_index_p]; }

private:
	static void __fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept;

public:
	processor(const processor&) = delete;
	processor& operator=(const processor&) = delete;

	processor& operator=(processor&& other_p) noexcept;
};




class processors
{
	friend class processor;
	using game_system_exec_table = std::pmr::vector< FE::pair<	FE::system, // the system function pointer
																std::pmr::vector< std::pmr::forward_list<FE::components>* > // the list of components the system will operate on
																>
													>;
	framework::ECS& m_ecs;
	FE::uint32 m_concurrency;
	FE::int32 m_fiber_host_count;
	std::atomic_bool m_is_running;
	std::unique_ptr<processor[]> m_processors;

	std::thread m_renderer_thread;
	std::thread m_physics_thread;
	std::thread m_audio_thread;
	std::thread m_networking_thread;

	game_system_exec_table m_game_systems; // the element index is the system execution order
	internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
	boost::fibers::fiber m_game_fiber;
	var::float64 m_delta_time_milliseconds;

	boost::fibers::fiber m_gc_fiber;
	var::float64 m_gc_delta_time_milliseconds;
	var::uint32 m_batch_count;

public:
	processors(framework::ECS& ecs_p, FE::int32 concurrency_p, FE::uint32 gc_batch_count_p, FE::size fiber_stack_size_p) noexcept;
	~processors() noexcept;

	void fork(FE::system renderer_p, FE::system physics_p, FE::system audio_p, FE::system networking_p) noexcept;
	void enqueue_task(framework::task task_p) noexcept;

	_FE_FORCE_INLINE_ FE::boolean is_running() const noexcept { return m_is_running.load(std::memory_order_acquire); }
	_FE_FORCE_INLINE_ FE::float64 get_delta_time_milliseconds() const noexcept { return m_delta_time_milliseconds; }
	_FE_FORCE_INLINE_ FE::float64 get_gc_delta_time_milliseconds() const noexcept { return m_gc_delta_time_milliseconds; }

private:
	static void __game_main(processors* const host_p) noexcept;
	static void __gc_main(processors* const host_p) noexcept;

public:
	processors(const processors&) = delete;
	processors& operator=(const processors&) = delete;
};


END_NAMESPACE
#endif