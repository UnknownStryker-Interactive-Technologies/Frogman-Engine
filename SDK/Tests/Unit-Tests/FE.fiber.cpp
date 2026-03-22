#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/fiber.hxx>




void system(FE::component_base* const component_p) noexcept
{
	(void)component_p;

}


class data : public FE::component_base
{
public:
	FE::fiber_scheduler* _scheduler;
	int _sum;
};


TEST(Fiber, Execute)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();

	FE::task task;
	task._system = system;
	task._component = nullptr;
	task._task_type = TaskPriority::_Normal;

	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}

TEST(Fiber, DoNotYieldDuringExecution)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = system;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = system;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}


void systemA(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 8;
}

void systemB(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	//l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 5;
}

TEST(Fiber, YieldDuringExecution1)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemA;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemB;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}


void systemC(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 8;
}

void systemD(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 5;
}

TEST(Fiber, YieldDuringExecution2)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemC;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemD;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}


void systemE(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 8;
}

void systemF(FE::component_base* const component_p) noexcept
{
	data* l_data_p = FE::polymorphic_cast<data*>(component_p);
	if (l_data_p == nullptr)
	{
		return; // invalid component pointer; just return.
	}

	l_data_p->_scheduler->switch_fiber_context();

	l_data_p->_sum = 7 + 5;

	l_data_p->_scheduler->switch_fiber_context();
}

TEST(Fiber, YieldDuringExecution3)
{
	FE::fiber_scheduler scheduler;
	scheduler.create_fiber();
	scheduler.create_fiber();

	data data;
	data._scheduler = &scheduler;

	FE::task task;
	task._system = systemE;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	task._system = systemF;
	task._component = &data;
	task._task_type = TaskPriority::_Normal;
	scheduler.schedule_task(task);

	_FE_MAYBE_UNUSED_ int result = scheduler.execute();
}