#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
#include <FE/framework/fiber.hxx>




void system(void*) noexcept
{
}


TEST(Fiber, Execute)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();

	FE::task task;
	task._system = system;
	task._argument = nullptr;
	task._task_type = TaskPriority::_Normal;

	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}




template <typename T>
struct data
{
	FE::fiber_scheduler* _scheduler;
	T _data;
};


TEST(Fiber, DoNotYieldDuringExecution)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data<int> data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = system;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = system;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}




void systemA(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 8;
}

void systemB(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	//l_data->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 5;
}


TEST(Fiber, YieldDuringExecution1)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data<int> data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemA;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemB;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}




void systemC(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 8;
}

void systemD(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 5;
}


TEST(Fiber, YieldDuringExecution2)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data<int> data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemC;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemD;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}




void systemE(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 8;
}

void systemF(void* argument) noexcept
{
	data<int>* l_data_p = static_cast<data<int>*>(argument);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_data = 7 + 5;

	l_data_p->_scheduler->switch_fiber_context();
}


TEST(Fiber, YieldDuringExecution3)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data<int> data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemE;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemF;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}




void allocate_string(void* argument) noexcept
{
	data<char*>* l_data = static_cast<data<char*>*>(argument);
	if (l_data == nullptr)
	{
		return; // invalid component pointer; just return.
	}
	l_data->_data = new char[strlen("Hi, world!")];
}

void assign_string(void* argument) noexcept
{
	data<char*>* l_data = static_cast<data<char*>*>(argument);
	if (l_data == nullptr)
	{
		return; // invalid component pointer; just return.
	}
	memcpy(l_data->_data, "Hi, world!", strlen("Hi, world!"));
}

void print_string(void* argument) noexcept
{
	data<char*>* l_data = static_cast<data<char*>*>(argument);
	if (l_data == nullptr)
	{
		return; // invalid component pointer; just return.
	}
	std::cout << l_data->_data << std::endl;
}

void deallocate_string(void* argument) noexcept
{
	data<char*>* l_data = static_cast<data<char*>*>(argument);
	if (l_data == nullptr)
	{
		return; // invalid component pointer; just return.
	}
	delete[] l_data->_data;
	l_data->_data = nullptr;
}

TEST(Fiber, OrderedTasks)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data<char*> data;
	data._scheduler = &scheduler;

	FE::fiber_flow flow;

	FE::task task;
	task._system = allocate_string;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	FE::fiber_flow::task allocate_string = flow.emplace(task);

	task._system = assign_string;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	FE::fiber_flow::task assign_string = flow.emplace(task);

	task._system = print_string;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	FE::fiber_flow::task print_string = flow.emplace(task);

	task._system = deallocate_string;
	task._argument = &data;
	task._task_type = TaskPriority::_Normal;
	FE::fiber_flow::task deallocate_string = flow.emplace(task);

	allocate_string.runs_before(assign_string);
	assign_string.runs_before(print_string);
	print_string.runs_before(deallocate_string);

	flow.sort_tasks_topologically(FE::fiber_flow::auth{}); // Sort tasks topologically
	scheduler.schedule_tasks(std::move(flow.get_topologically_ordered_tasks(FE::fiber_flow::auth{})));

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
	result = scheduler.execute();
	result = scheduler.execute();
	result = scheduler.execute();
}




namespace
{
	// A spider web: one hub in the middle, web_ring_count rings of web_spoke_count nodes around it, one rim on the outside.
	//
	//  hub --> every node of r0
	//  r0:  s0 > s1 > s2 > s3 > s4 > s5     ring threads  = runs_after (the direction flips on every ring)
	//       v    v    v    v    v    v      spoke threads = runs_before
	//  r1:  s0 < s1 < s2 < s3 < s4 < s5
	//       v    v    v    v    v    v
	//  r2:  s0 > s1 > s2 > s3 > s4 > s5
	//       v    v    v    v    v    v
	//  r3:  s0 < s1 < s2 < s3 < s4 < s5
	//  every node of r3 --> rim
	constexpr size_t web_spoke_count = 6;
	constexpr size_t web_ring_count = 4;
	constexpr size_t web_hub = 0;
	constexpr size_t web_rim = 1 + web_ring_count * web_spoke_count;
	constexpr size_t web_node_count = web_rim + 1;

	constexpr size_t web_node(size_t ring_p, size_t spoke_p) noexcept
	{
		return 1 + ring_p * web_spoke_count + spoke_p;
	}

	struct web_trace
	{
		std::array<size_t, web_node_count> _position{}; // the place in line at which each node ran
		std::array<int, web_node_count> _runs{};        // how many times each node ran
		size_t _sequence = 0;                           // how many tasks have run so far
	};

	// fiber_flow keys its vertices by the system function pointer, so every node needs a function of its own.
	template <size_t Id>
	void web_system(void* argument) noexcept
	{
		static_assert(Id < web_node_count);

		web_trace* l_trace = static_cast<web_trace*>(argument);
		if (l_trace == nullptr)
		{
			return; // invalid argument pointer; just return.
		}
		l_trace->_position[Id] = l_trace->_sequence++;
		l_trace->_runs[Id] += 1;
	}

	using web_system_t = decltype(FE::task::_system);

	template <size_t... Ids>
	constexpr std::array<web_system_t, sizeof...(Ids)> make_web_systems(std::index_sequence<Ids...>) noexcept
	{
		return { { &web_system<Ids>... } };
	}

	constexpr auto web_systems = make_web_systems(std::make_index_sequence<web_node_count>{});
}




namespace
{
	constexpr int task_count = 6;

	// Every task writes down the place in line at which it ran: 1 = first, 0 = never ran.
	struct run_order
	{
		int _place[task_count] = {};
		int _count = 0;

		int place(char name_p) const noexcept
		{
			return _place[name_p - 'A'];
		}
	};

	void record(void* argument, char name_p) noexcept
	{
		run_order* l_order = static_cast<run_order*>(argument);
		if (l_order == nullptr)
		{
			return; // invalid argument pointer; just return.
		}
		l_order->_place[name_p - 'A'] = ++l_order->_count;
	}

	// fiber_flow tells tasks apart by their function pointer, so every task needs a function of its own.
	void run_a(void* argument) noexcept { record(argument, 'A'); }
	void run_b(void* argument) noexcept { record(argument, 'B'); }
	void run_c(void* argument) noexcept { record(argument, 'C'); }
	void run_d(void* argument) noexcept { record(argument, 'D'); }
	void run_e(void* argument) noexcept { record(argument, 'E'); }
	void run_f(void* argument) noexcept { record(argument, 'F'); }
}




// Six tasks that depend on each other. "X -> Y" means X must run before Y.
// B and C (and D and E) don't depend on each other, so they may run in either order.
TEST(Fiber, DependentTasks)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	run_order order;
	FE::fiber_flow flow;

	FE::task task;
	task._argument = &order;
	task._task_type = TaskPriority::_Normal;

	task._system = run_a;
	FE::fiber_flow::task a = flow.emplace(task);

	task._system = run_b;
	FE::fiber_flow::task b = flow.emplace(task);

	task._system = run_c;
	FE::fiber_flow::task c = flow.emplace(task);

	task._system = run_d;
	FE::fiber_flow::task d = flow.emplace(task);

	task._system = run_e;
	FE::fiber_flow::task e = flow.emplace(task);

	task._system = run_f;
	FE::fiber_flow::task f = flow.emplace(task);

	a.runs_before(b); // A -> B
	a.runs_before(c); // A -> C
	d.runs_after(b);  // B -> D
	d.runs_after(c);  // C -> D
	e.runs_after(c);  // C -> E
	f.runs_after(d);  // D -> F
	f.runs_after(e);  // E -> F

	flow.sort_tasks_topologically(FE::fiber_flow::auth{});
	scheduler.schedule_tasks(std::move(flow.get_topologically_ordered_tasks(FE::fiber_flow::auth{})));

	for (int i = 0; i < task_count; ++i)
	{
		ASSERT_EQ(scheduler.execute(), _FE_SUCCEEDED_);
	}
	EXPECT_EQ(scheduler.execute(), _FE_FAILED_); // nothing is left to run

	// Every task ran exactly once.
	ASSERT_EQ(order._count, task_count);
	for (int place : order._place)
	{
		EXPECT_GT(place, 0);
	}

	// No task ran before a task it depends on.
	EXPECT_LT(order.place('A'), order.place('B'));
	EXPECT_LT(order.place('A'), order.place('C'));
	EXPECT_LT(order.place('B'), order.place('D'));
	EXPECT_LT(order.place('C'), order.place('D'));
	EXPECT_LT(order.place('C'), order.place('E'));
	EXPECT_LT(order.place('D'), order.place('F'));
	EXPECT_LT(order.place('E'), order.place('F'));
}