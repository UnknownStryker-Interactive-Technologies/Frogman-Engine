#include <FE/framework/processors.hpp>

#include <FE/framework/framework.hpp>
#include <FE/framework/ECS.hpp>

#include <FE/memory.hpp>
#include <FE/clock.hpp>

#include <boost/fiber/all.hpp>




BEGIN_NAMESPACE(FE::framework)


internal::processors::fiber_stack_allocator::fiber_stack_allocator(const std::size_t size = FE::one_MiB) noexcept
	: m_size(size)
{
}

boost::context::stack_context internal::processors::fiber_stack_allocator::allocate() noexcept
{
	void* l_alloc_result = framework::framework_base::get_framework().get_memory_resource()->allocate(m_size);
	FE_ASSERT(l_alloc_result != nullptr, "Assertion failure: memory allocation failed.");

	boost::context::stack_context l_stack_context;
	l_stack_context.size = m_size;
	l_stack_context.sp = static_cast<char*>(l_alloc_result) + l_stack_context.size;

	return l_stack_context;
}

void internal::processors::fiber_stack_allocator::deallocate(boost::context::stack_context& stack_context_p) noexcept
{
	FE_ASSERT(stack_context_p.sp != nullptr);
	void* l_address = static_cast<char*>(stack_context_p.sp) - stack_context_p.size;
	framework::framework_base::get_framework().get_memory_resource()->deallocate(l_address, stack_context_p.size);
}




processor::processor() noexcept
	:	m_host(),
		m_processor(),
		m_fiber_stack_allocator(0),
		m_is_running(false),

		m_fibers{},
		m_queue(framework_base::get_framework().get_memory_resource()),
		m_delta_time_milliseconds{ 0.0 }
{
}

processor::processor(class processors& host_p, FE::size fiber_stack_size_p = FE::one_MiB) noexcept
	:	m_host(&host_p),
		m_processor(),
		m_fiber_stack_allocator(fiber_stack_size_p),
		m_is_running(false),

		m_fibers{},
		m_queue(framework_base::get_framework().get_memory_resource()),
		m_delta_time_milliseconds{ 0.0 }
{
}

processor::~processor() noexcept
{
	join();
}


void processor::fork() noexcept
{
	FE_ASSERT(m_processor.joinable() == false, "Assertion failure: the processor is already running.");
	m_is_running.store(true, std::memory_order_release);

	m_processor = std::thread
	(
		[this]()
		{
			boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();

			for (var::int32 i = 0; i < fibers_per_thread; ++i)
			{
				m_fibers[i] = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processor::__fiber_main, this, i);
			}

			for (boost::fibers::fiber& fiber : m_fibers)
			{
				if (fiber.joinable())
				{
					fiber.join();
				}
			}
		}
	);
}

void processor::join() noexcept
{
	if (m_processor.joinable())
	{
		m_is_running.store(false, std::memory_order_release);
		m_processor.join();
	}
}

void processor::enqueue_task(framework::task task_p) noexcept
{
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
	m_queue.push(task_p);
}


void processor::__fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");

	FE::clock l_delta_clock;
	typename task_queue::value_type task{0, nullptr, nullptr};

	while(host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		if (host_p->m_queue.try_pop(task) == false) // no tasks in the queue
		{
			for (var::int32 i = 0; i < host_p->m_host->m_fiber_host_count; ++i) // steal other processor's task
			{
				if (host_p->m_host->m_processors[i].m_queue.try_pop(task) == false)
				{
					continue;
				}
				goto GotTask; // got a task from another processor
			}
			boost::this_fiber::yield(); // yield if no tasks are available
			continue;
		}
	GotTask: // using the "goto" statement to break the nested scopes
		FE_ASSERT(task._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
		l_delta_clock.start_clock();
		task._system(task._component);
		l_delta_clock.end_clock();
		host_p->m_delta_time_milliseconds[fiber_index_p] = l_delta_clock.get_delta_milliseconds();
	}
}

processor& processor::operator=(processor&& other_p) noexcept
{
	m_host = other_p.m_host;
	m_processor = std::move(other_p.m_processor);
	m_fiber_stack_allocator = std::move(other_p.m_fiber_stack_allocator);
	m_is_running.store(other_p.m_is_running.load(std::memory_order_acquire), std::memory_order_release);

	m_queue = std::move(other_p.m_queue);

	for (var::int32 i = 0; i < fibers_per_thread; ++i)
	{
		m_fibers[i] = std::move(other_p.m_fibers[i]);
		m_delta_time_milliseconds[i] = other_p.m_delta_time_milliseconds[i];
	}
	return *this;
}




processors::processors(framework::ECS& ecs_p, FE::int32 concurrency_p, FE::uint32 gc_batch_count_p = 30, FE::size fiber_stack_size_p = FE::one_MiB) noexcept
	:	m_ecs(ecs_p),
		m_concurrency(concurrency_p),
		m_fiber_host_count(m_concurrency - 4),
		m_is_running(false),
		m_processors(),

		m_renderer_thread(),
		m_physics_thread(),
		m_audio_thread(),
		m_networking_thread(),

		m_game_systems(framework_base::get_framework().get_memory_resource()),
		m_fiber_stack_allocator(fiber_stack_size_p),
		m_game_fiber(),
		m_delta_time_milliseconds(0.0),

		m_gc_fiber(),
		m_gc_delta_time_milliseconds(0.0),
		m_batch_count(gc_batch_count_p)

{
	FE_ASSERT(concurrency_p >= 4, "Assertion failure: the software thread count must be greater than or equal to 4.");
	m_game_systems.reserve(1024); // Preallocate some space to avoid frequent reallocations.
}

processors::~processors() noexcept
{
	if (m_game_fiber.joinable())
	{
		m_game_fiber.join();
	}

	if (m_gc_fiber.joinable())
	{
		m_gc_fiber.join();
	}

	if (m_physics_thread.joinable())
	{
		m_physics_thread.join();
	}

	if (m_networking_thread.joinable())
	{
		m_networking_thread.join();
	}

	if (m_renderer_thread.joinable())
	{
		m_renderer_thread.join();
	}

	if (m_audio_thread.joinable())
	{
		m_audio_thread.join();
	}

	for (var::uint32 i = 0; i < m_concurrency; ++i)
	{
		m_processors[i].join();
	}
}


void processors::fork(FE::system renderer_p, FE::system physics_p, FE::system audio_p, FE::system networking_p) noexcept
{
	FE_ASSERT(renderer_p != nullptr, "Assertion failure: renderer_p cannot be null.");
	FE_ASSERT(physics_p != nullptr, "Assertion failure: renderer_p cannot be null.");
	FE_ASSERT(audio_p != nullptr, "Assertion failure: renderer_p cannot be null.");
	FE_ASSERT(networking_p != nullptr, "Assertion failure: renderer_p cannot be null.");
	
	FE_ASSERT(m_renderer_thread.joinable() == false);
	FE_ASSERT(m_physics_thread.joinable() == false);
	FE_ASSERT(m_audio_thread.joinable() == false);
	FE_ASSERT(m_networking_thread.joinable() == false);
	FE_ASSERT(m_is_running.load(std::memory_order_acquire) == false);
	FE_ASSERT(m_processors == nullptr);
	FE_ASSERT(m_game_systems.empty() == true);

	m_is_running.store(true, std::memory_order_release);

	m_renderer_thread = std::thread(renderer_p, nullptr);
	m_physics_thread = std::thread(physics_p, nullptr);
	m_audio_thread = std::thread(audio_p, nullptr);
	m_networking_thread = std::thread(networking_p, nullptr);

	for (_FE_MAYBE_UNUSED_ auto& [system_name, system_and_target_component_type_hash_list] : m_ecs.m_system_table)
	{
		typename game_system_exec_table::value_type::second_type l_components_group_list{framework_base::get_framework().get_memory_resource()};
		
		for (auto& component_type_hash : system_and_target_component_type_hash_list._second)
		{
			_FE_MAYBE_UNUSED_ auto l_result = m_ecs.m_component_table.find(component_type_hash);
			FE_ASSERT(l_result != m_ecs.m_component_table.end(), "Assertion failed: the component table must have this component type.");
			l_components_group_list.emplace_back(&(l_result->second._second));
		}
		m_game_systems.emplace_back(system_and_target_component_type_hash_list._first, std::move(l_components_group_list));
	}

	m_processors = std::make_unique<processor[]>(m_fiber_host_count);
	for (var::int32 i = 0; i < m_fiber_host_count; ++i)
	{
		m_processors[i] = processor(*this, m_fiber_stack_allocator.stack_size());
		m_processors[i].fork(); // start listening for tasks
	}

	boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();
	m_game_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processors::__game_main, this);
	m_gc_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processors::__gc_main, this);
}

void processors::enqueue_task(framework::task task_p) noexcept
{
	FE_ASSERT(m_processors != nullptr, "Assertion failure: the processors have not been initialized. Call fork() first.");
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");

	typename task_queue::size_type l_remaining_tasks = m_processors[0].count_remaining_tasks();
	var::int32 l_processor_index = 0;
	/*
	Although count_remaining_tasks is not accurate, it returns a clue to infer which processor is less busy.
	Typical gaming cpus do not have more than 64 logical hardware threads. 
	The concurrency can be adjusted by the -max-concurrency=N and the loop is linear. However, for most cases, the N won't be absurdly high.
	*/
	for (var::int32 i = 0; i < m_fiber_host_count; ++i) 
	{
		if (l_remaining_tasks > m_processors[i].count_remaining_tasks()) 
		{
			l_remaining_tasks = m_processors[i].count_remaining_tasks();
			l_processor_index = i;
		}
	}
	m_processors[l_processor_index].enqueue_task(task_p);
}


void processors::__game_main(processors* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE::clock l_delta_clock;

	while (host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		l_delta_clock.start_clock();
		for (typename game_system_exec_table::value_type& system_and_components : host_p->m_game_systems)
		{
			FE_ASSERT(system_and_components._first != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
			
			for(std::pmr::forward_list<FE::components>* const component_list : system_and_components._second)
			{
				for (FE::components& components : *component_list)
				{
					for (FE::component& component : components)
					{
						FE_ASSERT(component != nullptr, "Assertion failure: component pointers cannot be a nullptr.");
						if (component.observer_count() == 0)
						{
							continue; // skip expired components
						}
						system_and_components._first(component.operator->());
					}
				}
			}
		}
		l_delta_clock.end_clock();
		host_p->m_delta_time_milliseconds = l_delta_clock.get_delta_milliseconds();
	}
}

void processors::__gc_main(processors* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE::clock l_delta_clock;

	while (host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		l_delta_clock.start_clock();
		var::uint32 l_death_count = 0;

		for (auto& [identifier, archetype] : host_p->m_ecs.m_archetype_table)
		{
			FE_ASSERT(archetype == nullptr);
			if (archetype.observer_count() == 0)
			{
				host_p->m_ecs.m_archetype_table.erase(identifier);
				++l_death_count;
			}

			if (l_death_count >= host_p->m_batch_count)
			{
				goto Yield;
			}
		}

		for (auto& [type_hash, components_list] : host_p->m_ecs.m_component_table)
		{
			for (FE::components& components : components_list._second)
			{
				for (FE::component& component : components)
				{
					FE_ASSERT(component != nullptr, "Assertion failure: component pointers cannot be a nullptr.");
					if (component.observer_count() == 0)
					{
						components.remove_component(component->m_identifier._index);
						++l_death_count;
					}

					if (l_death_count >= host_p->m_batch_count)
					{
						goto Yield;
					}
				}
			}
		}
	Yield:
		l_delta_clock.end_clock();
		host_p->m_gc_delta_time_milliseconds = l_delta_clock.get_delta_milliseconds();
		boost::this_fiber::yield();
	}
}


END_NAMESPACE