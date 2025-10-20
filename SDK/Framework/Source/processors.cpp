#include <FE/framework/processors.hpp>

#include <FE/framework.h>
#include <FE/memory.hpp>
#include <FE/clock.hpp>

#include <boost/fiber/all.hpp>

#include <GLFW/glfw3.h>




BEGIN_NAMESPACE(FE::framework)




task_queue::task_queue(std::pmr::memory_resource* const memory_resource_p) noexcept
	:	m_urgent_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p)),
		m_priored_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p)),
		m_ordinary_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p)),
		m_trivial_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p))
{
}

void task_queue::push(framework::task task_p) noexcept
{
	switch (task_p._priority)
	{
	case TaskType::_Urgent:
		m_urgent_tasks.push(task_p);
		break;

	case TaskType::_Priored:
		m_priored_tasks.push(task_p);
		break;

	case TaskType::_Ordinary:
		m_ordinary_tasks.push(task_p);
		break;

	case TaskType::_Trivial:
		m_trivial_tasks.push(task_p);
		break;

	default:
		break;
	}
}

FE::boolean task_queue::try_pop(framework::task& out_task_p) noexcept
{
	var::boolean l_was_successful = m_urgent_tasks.try_pop(out_task_p);
	if (l_was_successful == false)
	{
		l_was_successful = m_priored_tasks.try_pop(out_task_p);
		if (l_was_successful == false)
		{
			l_was_successful = m_ordinary_tasks.try_pop(out_task_p);
			if (l_was_successful == false)
			{
				l_was_successful = m_trivial_tasks.try_pop(out_task_p);
			}
		}
	}
	return l_was_successful;
}




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

void processor::push_task(framework::task task_p) noexcept
{
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
	m_queue.push(task_p);
}


void processor::__fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");

	FE::clock l_delta_clock;
	typename task_queue::value_type task{TaskType::_Ordinary, nullptr, nullptr};

	while(host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		if (host_p->m_queue.try_pop(task) == false) // no tasks in the queue
		{
			for (var::int32 i = 0; i < host_p->m_host->m_fiber_host_count; ++i) // steal other processor's task
			{
				if ( reinterpret_cast<processor*>( host_p->m_host->m_processors.get() )[i].m_queue.try_pop(task) == false )
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




processors::processors(framework::ECS& ecs_p, FE::int32 concurrency_p, FE::uint32 gc_batch_count_p = 100, FE::size fiber_stack_size_p = FE::one_MiB) noexcept
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
		m_gc_reachability_analysis_fiber(),
		m_gc_delta_time_milliseconds(0.0),
		m_gc_iter_per_frame(gc_batch_count_p)

{
	FE_ASSERT(concurrency_p >= 6, "Assertion failure: the software thread count must be greater than or equal to 6.");
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
		reinterpret_cast<processor*>( m_processors.get() )[i].join();
		reinterpret_cast<processor*>(m_processors.get())[i].~processor();
	}
}


void processors::fork(	FE::system renderer_p, FE::component_base* renderer_args_p,
						FE::system physics_p, FE::component_base* physics_args_p,
						FE::system audio_p, FE::component_base* audio_args_p,
						FE::system networking_p, FE::component_base* networking_args_p) noexcept
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

	m_renderer_thread = std::thread(renderer_p, renderer_args_p);
	m_physics_thread = std::thread(physics_p, physics_args_p);
	m_audio_thread = std::thread(audio_p, audio_args_p);
	m_networking_thread = std::thread(networking_p, networking_args_p);

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

	m_processors = std::make_unique<std::byte[]>( m_fiber_host_count * sizeof(processor) );
	const processor* const l_end = ((processor*)m_processors.get()) + m_fiber_host_count;
	for (processor* ptr = (processor*)m_processors.get(); ptr != l_end; ++ptr)
	{
		new(ptr) processor(*this, m_fiber_stack_allocator.stack_size());
		ptr->fork();
	}

	boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();
	m_game_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processors::__game_main, this);
	m_gc_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processors::__gc_main, this);
	m_gc_reachability_analysis_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processors::__reachability_analysis_main, this);
}

void processors::push_task(framework::task task_p) noexcept
{
	FE_ASSERT(m_processors != nullptr, "Assertion failure: the processors have not been initialized. Call fork() first.");
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");

	static std::atomic_uint64_t s_next_processor_index{ 0 };
	reinterpret_cast<processor*>( m_processors.get() )[ s_next_processor_index.fetch_add(1, std::memory_order_acq_rel) % m_fiber_host_count ].push_task(task_p);
}


void processors::__game_main(processors* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE::clock l_delta_clock;

	while (host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		l_delta_clock.start_clock();
		glfwPollEvents();
		for (typename game_system_exec_table::value_type& system_and_components : host_p->m_game_systems)
		{
			FE_ASSERT(system_and_components._first != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
			
			for (std::pmr::forward_list<FE::internal::ECS::components>* const component_list : system_and_components._second)
			{
				for (FE::internal::ECS::components& components : *component_list)
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
		boost::this_fiber::yield();
	}
}

void processors::__gc_main(processors* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE::clock l_delta_clock;

	while (host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		l_delta_clock.start_clock();
		{
			var::uint64 l_batch_count = 0;
			var::uint64 l_current_idx = 0;
			std::lock_guard<boost::fibers::recursive_mutex> l_lock(host_p->m_ecs.m_fiber_lock);
			for (auto iterator = host_p->m_ecs.m_archetype_table.begin(); iterator != host_p->m_ecs.m_archetype_table.end(); ++iterator)
			{
				if (iterator->second == nullptr)
				{
					continue; // skip expired entities
				}

				if (iterator->second.observer_count() == 0)
				{
					iterator = host_p->m_ecs.m_archetype_table.erase(iterator); // remove expired entities
				}

				++l_batch_count;
				if (l_batch_count >= host_p->m_gc_iter_per_frame)
				{
					l_delta_clock.end_clock();
					host_p->m_gc_delta_time_milliseconds = l_delta_clock.get_delta_milliseconds();

					host_p->m_ecs.m_fiber_lock.unlock();
					boost::this_fiber::yield();
					host_p->m_ecs.m_fiber_lock.lock();

					l_delta_clock.start_clock();
					l_current_idx += l_batch_count;
					l_batch_count = l_batch_count xor l_batch_count; // reset batch count to zero
					iterator = std::next(host_p->m_ecs.m_archetype_table.begin(), l_current_idx); // refresh the iterator after yielding
				}
			}
		}

		boost::this_fiber::yield();

		{
			var::uint64 l_batch_count = 0;
			var::uint64 l_current_idx = 0;
			std::lock_guard<boost::fibers::recursive_mutex> l_lock(host_p->m_ecs.m_fiber_lock);
			for (auto iterator = host_p->m_ecs.m_component_table.begin(); iterator != host_p->m_ecs.m_component_table.end(); ++iterator)
			{
				for (FE::internal::ECS::components& components : iterator->second._second)
				{
					for (FE::component& component : components)
					{
						FE_ASSERT(component != nullptr, "Assertion failure: component pointers cannot be a nullptr.");
						if (component.observer_count() == 0)
						{
							components.remove_component(component->m_metadata->_index);
						}
						else if (component->m_metadata->m_gc_metadata->_is_circular_reference.load(std::memory_order_acquire) == true)
						{
							components.remove_component(component->m_metadata->_index); // remove circular-referenced components
						}

						++l_batch_count;
						if (l_batch_count >= host_p->m_gc_iter_per_frame)
						{
							l_delta_clock.end_clock();
							host_p->m_gc_delta_time_milliseconds = l_delta_clock.get_delta_milliseconds();

							host_p->m_ecs.m_fiber_lock.unlock();
							boost::this_fiber::yield();
							host_p->m_ecs.m_fiber_lock.lock();

							l_delta_clock.start_clock();
							l_current_idx += l_batch_count;
							l_batch_count = l_batch_count xor l_batch_count; // reset batch count to zero
							iterator = std::next(host_p->m_ecs.m_component_table.begin(), l_current_idx); // refresh the iterator after yielding
						}
					}
				}
			}
		}
	}
}

void processors::__reachability_analysis_main(processors* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: data_p cannot be null.");

	while (host_p->m_is_running.load(std::memory_order_acquire) == true)
	{
		{
			std::lock_guard<boost::fibers::recursive_mutex> l_lock(host_p->m_ecs.m_fiber_lock);
			for (auto iterator = host_p->m_ecs.m_component_table.begin(); iterator != host_p->m_ecs.m_component_table.end(); ++iterator)
			{
				for (FE::internal::ECS::components& components : iterator->second._second)
				{
					for (FE::component& component : components)
					{
						FE_ASSERT(component != nullptr);
						if (component.observer_count() != 1)
						{
							continue; // could not suspect a circular reference
						}
						host_p->__reachability_analysis_recursive(component, component); // examine the circular reference
					}
				}
			}
		}
		boost::this_fiber::yield();
	}
}

void processors::__reachability_analysis_recursive(FE::component_view<FE::component_base> root_p, FE::component_view<FE::component_base> child_p) noexcept
{
	FE_ASSERT(root_p.is_valid () == true, "Assertion failure: root_p cannot be null.");
	FE_ASSERT(root_p->m_metadata != nullptr, "Assertion failure: root_p's metadata cannot be null.");
	FE_ASSERT(root_p->m_metadata->m_gc_metadata != nullptr, "Assertion failure: root_p's gc_metadata cannot be null.");

	FE_ASSERT(child_p.is_valid() == true, "Assertion failure: child_p cannot be null.");
	FE_ASSERT(child_p->m_metadata != nullptr, "Assertion failure: child_p's metadata cannot be null.");
	FE_ASSERT(child_p->m_metadata->m_gc_metadata != nullptr, "Assertion failure: child_p's gc_metadata cannot be null.");

	static var::uint64 s_recursion_depth = 0;
	static var::boolean s_should_exit = false;

	if (s_should_exit == true)
	{
		--s_recursion_depth;
		if (s_recursion_depth == 0)
		{
			s_should_exit = false;
		}
		return;
	}
	if (s_recursion_depth >= m_gc_iter_per_frame)
	{
		s_should_exit = true;
		return;
	}
	++s_recursion_depth;


	for (FE::component_view<FE::component_base>* subcomponent_view : child_p->m_metadata->m_gc_metadata->_member_components)
	{
		if (root_p.operator->() == subcomponent_view->operator->()) // examine the circular reference
		{
			root_p->m_metadata->m_gc_metadata->_is_circular_reference.store(true, std::memory_order_release);
			return; // circular reference detected
		}
		__reachability_analysis_recursive(root_p , *subcomponent_view);
	}

	for (FE::entity<FE::archetype_base>* subentity_view : child_p->m_metadata->m_gc_metadata->_member_entities)
	{
		for (auto iterator = (*subentity_view)->m_component_view_table.begin(); iterator != (*subentity_view)->m_component_view_table.end(); ++iterator)
		{
			if (root_p.operator->() == iterator->second.operator->()) // examine the circular reference
			{
				root_p->m_metadata->m_gc_metadata->_is_circular_reference.store(true, std::memory_order_release);
				return; // circular reference detected
			}
			__reachability_analysis_recursive(root_p, iterator->second);
		}
	}
}


END_NAMESPACE