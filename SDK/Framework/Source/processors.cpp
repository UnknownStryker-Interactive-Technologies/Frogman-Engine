#include <FE/framework/processors.hxx>

#include <FE/framework.hxx>
#include <FE/memory.hxx>
#include <FE/clock.hxx>

#include <boost/fiber/all.hpp>

#include <GLFW/glfw3.h>




BEGIN_NAMESPACE(FE::framework)


task::task(const task& other_p) noexcept
	:	m_notifier(other_p.m_notifier),
		_priority(other_p._priority),
		_system(other_p._system),
		_component(other_p._component)
{
}
task& task::operator=(const task& other_p) noexcept
{
	m_notifier = other_p.m_notifier;
	_priority = other_p._priority;
	_system = other_p._system;
	_component = other_p._component;
	return *this;
}

task::task(task&& other_p) noexcept
	:	m_notifier(std::move(other_p.m_notifier)),
		_priority(other_p._priority),
		_system(other_p._system),
		_component(other_p._component)
{
	other_p._priority = TaskType::_Ordinary;
	other_p._system = nullptr;
	other_p._component = nullptr;
}
task& task::operator=(task&& other_p) noexcept
{
	m_notifier = std::move(other_p.m_notifier);

	_priority = other_p._priority;
	other_p._priority = TaskType::_Ordinary;

	_system = other_p._system;
	other_p._system = nullptr;

	_component = other_p._component;
	other_p._component = nullptr;
	return *this;
}




task_queue::task_queue(std::pmr::memory_resource* const memory_resource_p) noexcept
	:	m_urgent_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p)),
		m_ordinary_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p)),
		m_trivial_tasks(std::pmr::polymorphic_allocator<task>(memory_resource_p))
{
}


void task_queue::push(const framework::task& task_p) noexcept
{
	switch (task_p._priority)
	{
	case TaskType::_Urgent:
		m_urgent_tasks.push(task_p);
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
		l_was_successful = m_ordinary_tasks.try_pop(out_task_p);
		if (l_was_successful == false)
		{
			l_was_successful = m_trivial_tasks.try_pop(out_task_p);
		}
	}
	return l_was_successful;
}




internal::processors::fiber_stack_allocator::fiber_stack_allocator(const std::size_t size) noexcept
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




internal::processors::processor::processor() noexcept
	:	m_host(),
		m_processor(),
		m_should_terminate(false),
		m_fibers_per_thread(0),
		m_yield_status(0),
		m_queue(framework_base::get_framework().get_memory_resource()),
		m_fiber_stack_allocator(0),

		m_fibers(),
		m_delta_ms(),
		m_condition_variable()
{
}

internal::processors::processor::~processor() noexcept
{
	join();
}


void internal::processors::processor::fork(::FE::framework::processors& host_p, FE::uint16 fibers_per_thread_p, FE::size fiber_stack_size_p) noexcept
{
	m_host = &host_p;

	FE_ASSERT(m_processor.joinable() == false, "Assertion failure: the processor is already running.");
	m_should_terminate.store(false, std::memory_order_release);

	m_fibers_per_thread = fibers_per_thread_p;
	m_fiber_stack_allocator = fiber_stack_size_p;

	m_fibers = std::make_unique<boost::fibers::fiber[]>(fibers_per_thread_p);
	m_delta_ms = std::make_unique<var::float64[]>(fibers_per_thread_p);

	m_processor = boost::thread
	(
		[this]()
		{
			boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();

			for (var::uint32 i = 0; i < m_fibers_per_thread; ++i)
			{
				m_fibers[i] = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &processor::__fiber_main, this, i);
			}

			for (var::uint32 i = 0; i < m_fibers_per_thread; ++i)
			{
				if (m_fibers[i].joinable())
				{
					m_fibers[i].join();
				}
			}
		}
	);
}

void internal::processors::processor::join() noexcept
{
	if (m_processor.joinable())
	{
		m_should_terminate.store(true, std::memory_order_release);
		wake();
		m_processor.join();
	}
}

void internal::processors::processor::schedule_task(const framework::task& task_p) noexcept
{
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
	m_queue.push(task_p);
}

FE::uint64 __create_fibers_host_sleep_mask(FE::uint16 fibers_per_thread_p) noexcept
{
	FE_ASSERT(fibers_per_thread_p <= 64, "Cannot assign fibers more than 64 per thread.");

	var::uint64 l_mask = 0;
	for (var::uint16 i = 0; i < fibers_per_thread_p; ++i)
	{
		l_mask = l_mask | 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
		l_mask = l_mask << 1;
	}
	return l_mask;
}

void internal::processors::processor::__fiber_main(processor* const host_p, FE::int32 fiber_index_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE_ASSERT(fiber_index_p >= 0, "Assertion failure: host_p cannot be null.");

	boost::mutex l_mutex;
	FE::clock l_delta_clock;
	FE::uint64 l_fibers_host_sleep_mask = __create_fibers_host_sleep_mask(host_p->m_fibers_per_thread); // if 3 fibers are assigned to a thread, the mask is 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000111;
	host_p->m_yield_status = host_p->m_yield_status xor host_p->m_yield_status;
	typename task_queue::value_type l_task;

	while (host_p->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		if (host_p->m_queue.try_pop(l_task) == false) // no tasks in the queue
		{
			host_p->m_yield_status = host_p->m_yield_status | 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001; // mark this fiber as yielded
			host_p->m_yield_status = host_p->m_yield_status << 1; // shift the bit to the left by one.

			boost::this_fiber::yield(); // yield if no tasks are available
			if (host_p->m_yield_status == l_fibers_host_sleep_mask) // all fibers have yielded
			{
				host_p->m_yield_status = host_p->m_yield_status xor host_p->m_yield_status; // set all bits to zero.
				boost::unique_lock<boost::mutex> l_unique_lock(l_mutex);
				host_p->m_condition_variable.wait(l_unique_lock); // wait until notified of new tasks
			}
			continue;
		}
		FE_ASSERT(l_task._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");
		l_delta_clock.start_clock();
		l_task._system(l_task._component);
		l_delta_clock.end_clock();

		if (l_task.is_waitable())
		{
			l_task.notify_completion();
		}

		host_p->m_delta_ms[fiber_index_p] = l_delta_clock.get_delta_milliseconds();
	}
}




void thread::fork(FE::system system_p, FE::component_base* const arguments_p, FE::size fiber_stack_size_p) noexcept
{
	FE_ASSERT(m_host.joinable() == false, "Assertion failure: the thread is already running.");

	m_host = boost::thread(__thread_main, system_p, arguments_p, fiber_stack_size_p);
}

void thread::join() noexcept
{
	if (m_host.joinable())
	{
		m_host.join();
	}
}

void thread::__thread_main(FE::system system_p, FE::component_base* const arguments_p, FE::size fiber_stack_size_p) noexcept
{
	boost::fibers::fiber l_fiber(std::allocator_arg, internal::processors::fiber_stack_allocator(fiber_stack_size_p), system_p, arguments_p);
	if (l_fiber.joinable())
	{
		l_fiber.join();
	}
}




processors::processors(FE::int32 concurrency_p, FE::uint16 fibers_per_thread_p, FE::size fiber_stack_size_p) noexcept
	:	m_concurrency((FE::uint16)concurrency_p),
		m_fibers_per_thread(fibers_per_thread_p),
		m_fiber_stack_size((FE::uint32)fiber_stack_size_p),
		m_processors()
{
	FE_ASSERT(concurrency_p > 0, "Assertion failure: fibers_per_thread_p must be greater than zero.");

	FE_ASSERT(fibers_per_thread_p <= 64, "Assertion failure: cannot create more than 64 fibers per thread.");
	FE_ASSERT(fibers_per_thread_p > 0, "Assertion failure: cannot create 0 fiber per thread.");

	FE_ASSERT(fiber_stack_size_p >= FE::one_KiB, "Assertion failure: fiber_stack_size_p must be at least 1 KiB.");
	FE_ASSERT(fiber_stack_size_p <= 4*FE::one_GiB, "Assertion failure: fiber_stack_size_p must be at less than 4 GiB.");
}


void processors::run() noexcept
{
	FE_ASSERT(m_processors == nullptr, "Assertion failure: the processors have already been initialized. Call shutdown() first.");

	m_processors = std::make_unique<internal::processors::processor[]>(m_concurrency);
	for (var::uint32 i = 0; i < m_concurrency; ++i)
	{
		m_processors[i].fork(*this, m_fibers_per_thread, m_fiber_stack_size);
	}
}

void processors::schedule_task(const framework::task& task_p) noexcept
{
	FE_ASSERT(m_processors != nullptr, "Assertion failure: the processors have not been initialized. Call fork() first.");
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");

	static std::atomic_uint32_t l_s_next_processor_index{ 0 };
	FE::uint32 l_target_processor_index = l_s_next_processor_index.fetch_add(1, std::memory_order_acq_rel) % m_concurrency;
	m_processors[ l_target_processor_index ].schedule_task(task_p);
	m_processors[l_target_processor_index].wake();
}

typename task::handle processors::schedule_waitable_task(framework::task& task_p) noexcept
{
	FE_ASSERT(m_processors != nullptr, "Assertion failure: the processors have not been initialized. Call fork() first.");
	FE_ASSERT(task_p._system != nullptr, "Assertion failure: ECS system function pointers cannot be a nullptr.");

	static boost::fibers::mutex l_s_task_pool_lock;
	{
		static FE::memory_resource l_s_task_pool;  // It is a crime not to allocate futures on the pool.
		std::lock_guard<boost::fibers::mutex> l_lock(l_s_task_pool_lock);
		task_p.m_notifier = std::allocate_shared<task::notifier>(	FE::polymorphic_allocator<task::notifier>(&l_s_task_pool), 
																	std::allocator_arg_t(), FE::polymorphic_allocator<void>(&l_s_task_pool));
		/* I found that the boost::fibers::promise allocates boost::fibers::detail::shared_state on the heap using the std::allocator and manages the object with the boost's intrusive_ptr.
		   There's no way to let boost::fibers::promise new and delete the boost::fibers::detail::shared_state<void> for boost::fibers::future on every schedule_waitable_task() call.
		   I wish there is way to remove this absurd heap allocation, without modifying the boost code; I will definitely rewrite some of the STL and boost TL for the Frogman Engine if I get to have spare time to do so; if you are interested in the template library development project, please checkout the XTL repository in the GitHub: https://github.com/UnknownStryker-Interactive-Technology/XTL. Although the repository is empty now.
		   The FE::memory_resource reduces the allocate/deallocate overhead since it takes 0(1) time to allocate/deallocate objects that are smaller than 128 bytes.
		*/
	}
	static std::atomic_uint32_t l_s_next_processor_index{ 0 };
	FE::uint32 l_target_processor_index = l_s_next_processor_index.fetch_add(1, std::memory_order_acq_rel) % m_concurrency;
	m_processors[l_target_processor_index].schedule_task(task_p);
	m_processors[l_target_processor_index].wake();
	return task_p.m_notifier->get_future();
}

void processors::shutdown() noexcept 
{
	if (m_processors == nullptr)
	{
		return;
	}

	for (var::uint32 i = 0; i < m_concurrency; ++i)
	{
		m_processors[i].join();
	}
	m_processors.reset();
}


END_NAMESPACE