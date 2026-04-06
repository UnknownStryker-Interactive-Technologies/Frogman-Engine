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
#include <FE/framework/fiber.hxx>

#include <FE/framework/thread_id.hxx>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>




extern "C" void _FE_CDECL_ __switch_to_the_fiber(struct fiber_impl* const from_p, struct fiber_impl* const to_p, size_t simd_vector_size_p);
extern "C" void _FE_CDECL_ __switch_to_new_fiber(struct fiber_impl* const from_p, struct fiber_impl* const to_p, size_t simd_vector_size_p, void(FE::fiber_scheduler::* to_return_to_p)(), struct fiber_impl* const thread_p);
extern "C" void _FE_CDECL_ __fork_fiber(struct fiber_impl* const out_thread_p, struct fiber_impl* const fiber_p, size_t simd_vector_size_p, void(FE::fiber_scheduler::* to_return_to_p)());
extern "C" void _FE_CDECL_ __join_fiber(struct fiber_impl* const thread_p, size_t simd_vector_size_p);

void _FE_CDECL_ __create_fiber(fiber_impl* const out_fiber_p, size_t stack_size_p) noexcept
{
	FE_ASSERT(out_fiber_p != nullptr);
	FE_ASSERT(stack_size_p >= FE::system_page_size);

	{
		stack_size_p = FE::calculate_aligned_memory_size_in_bytes<var::byte, FE::page_alignment>(stack_size_p) + FE::system_page_size;
		
		out_fiber_p->_absolute_begin_of_stack = VirtualAlloc(nullptr, stack_size_p, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		FE_ASSERT(out_fiber_p->_absolute_begin_of_stack != nullptr);

		{
			DWORD l_previous_protection_mask;
			_FE_MAYBE_UNUSED_ auto l_result = VirtualProtect(out_fiber_p->_absolute_begin_of_stack, FE::system_page_size, PAGE_READWRITE | PAGE_GUARD, &l_previous_protection_mask);
			FE_ASSERT(l_result != 0);
		}

		out_fiber_p->_stack_base = (var::byte*)out_fiber_p->_absolute_begin_of_stack + stack_size_p;
		out_fiber_p->_stack_limit = (var::byte*)out_fiber_p->_absolute_begin_of_stack + FE::system_page_size;

		// Multiple of 4096 is always multiple of 16, 32, and 64.
		var::uintptr l_fiber_page_ptr = (var::uintptr)out_fiber_p->_stack_base;
		l_fiber_page_ptr -= sizeof(fiber_context); // allocate space for fiber context on the stack.
		out_fiber_p->_context_ptr = (fiber_context*)l_fiber_page_ptr;
		memset(out_fiber_p->_context_ptr, 0, sizeof(fiber_context));

		static_assert(sizeof(SIMD) == alignof(fiber_context));
		static_assert(sizeof(fiber_context) % 16 == 0);

		out_fiber_p->_context_ptr->_rsp = (var::byte*)l_fiber_page_ptr;
		out_fiber_p->_host_thread_id = FE::framework::get_current_thread_id();
	}
	{
		//mxcsr& l_mxcsr = out_fiber_p->_context_ptr->_mxcsr;
		//l_mxcsr._invalid_op_flag			= 0b0;
		//l_mxcsr._denormal_flag			= 0b0;
		//l_mxcsr._divide_by_zero_flag		= 0b0;
		//l_mxcsr._overflow_flag			= 0b0;
		//l_mxcsr._underflow_flag			= 0b0;
		//l_mxcsr._precision_flag			= 0b0;

		//l_mxcsr._denormals_are_zero		= 0b0;

		//l_mxcsr._invalid_op_mask = 0b1;
		//l_mxcsr._denormal_operand_mask = 0b1;
		//l_mxcsr._divide_by_zero_mask = 0b1;
		//l_mxcsr._overflow_mask = 0b1;
		//l_mxcsr._underflow_mask = 0b1;
		//l_mxcsr._precision_mask = 0b1;

		//l_mxcsr._rounding_control			= 0b00;

		//l_mxcsr._flush_to_zero			= 0b0;
	}
	{
		//fpcw& l_fpcw = out_fiber_p->_context_ptr->_fpcw;
		//l_fpcw._invalid_op_mask = 0b1;
		//l_fpcw._denormal_operand_mask = 0b1;
		//l_fpcw._divide_by_zero_mask = 0b1;
		//l_fpcw._overflow_mask = 0b1;
		//l_fpcw._underflow_mask = 0b1;
		//l_fpcw._precision_mask = 0b1;

		//l_fpcw._reserved = 0b01; // Matches the standard Windows thread initialization (0x027F) for legacy x87 compatibility; I do not want any jumpscares.

		//l_fpcw._precision_control = 0b10;

		//l_fpcw._rounding_control		= 0b00;
		//l_fpcw._infinity_control		= 0b0;
	}
}




FE::fiber_scheduler::fiber_scheduler() noexcept
	:	m_fiber_pool(),
		m_active_fibers(),
		m_fibers(0),
		m_task_queue(),
		m_thread_contexts(std::make_unique<thread_context[]>(FE::framework::framework_base::get_framework().get_program_options().get_max_concurrency()))
{
}

void _FE_CDECL_ FE::fiber_scheduler::create_fiber(FE::size stack_size_p) noexcept
{
	++m_fibers;
	m_fiber_pool.push(stack_size_p);
}

void _FE_CDECL_ FE::fiber_scheduler::schedule_task(const task& task_p) noexcept
{
	m_task_queue.push(task_p);
}

int _FE_CDECL_ FE::fiber_scheduler::execute() noexcept
{
	FE::task l_to_execute;
	if (m_task_queue.try_pop(l_to_execute) == false)
	{
		// publish a sleep-wait task in the game framework job system loop.
		return _FE_FAILED_; // no more tasks to execute.
	}

	FE::fiber l_to_switch = m_fiber_pool.pop(); // get a fiber from the pool to run the task.
	l_to_switch.m_impl->_context_ptr->_r12 = (var::uint64)l_to_execute._system; // smuggle the system function pointer into r12; the fiber entry point will call this function pointer to execute the task.
	l_to_switch.m_impl->_context_ptr->_r13 = (var::uint64)l_to_execute._component; // smuggle the component pointer into r13; the fiber entry point will pass this as an argument to the system function when calling it.
	l_to_switch.m_impl->_context_ptr->_r14 = (var::uint64)this; // capture 'this' pointer for later use in the assembly.
	l_to_switch.m_impl->_task_type = l_to_execute._task_type;

	tl_s_current_fiber = l_to_switch.m_impl;

	auto l_this_thread = &(m_thread_contexts[FE::framework::get_current_thread_id()]._thread_context);
	__fork_fiber(l_this_thread, l_to_switch.m_impl, sizeof(SIMD), &FE::fiber_scheduler::switch_fiber_context); // debug it! Errr

	// set a pointer to the fiber to reclaim
	l_to_switch.m_impl = tl_s_current_fiber;

	for (auto& active_fiber_queue : m_active_fibers)
	{
		while (active_fiber_queue.is_empty() == false)
		{
			FE::fiber l_to_recycle = active_fiber_queue.pop();
			l_to_recycle.m_impl->_context_ptr->_rsp = (var::byte*)l_to_recycle.m_impl->_context_ptr; // reset the rsp
			l_to_recycle.m_impl->_task_type = TaskPriority::_None; // reset the task type to the default value.
			m_fiber_pool.push( std::move(l_to_recycle) );
		}
	}
	l_to_switch.m_impl->_context_ptr->_rsp = (var::byte*)l_to_switch.m_impl->_context_ptr; // reset the rsp
	l_to_switch.m_impl->_task_type = TaskPriority::_None; // reset the task type to the default value.
	m_fiber_pool.push( std::move(l_to_switch) ); // reclaim it.
	return _FE_SUCCEEDED_;
}

void _FE_CDECL_ FE::fiber_scheduler::switch_fiber_context() noexcept
{
	static_assert(offsetof(fiber_context, _simd6) == 0);
	static_assert(offsetof(fiber_context, _simd7) == sizeof(SIMD));
	static_assert(offsetof(fiber_context, _simd8) == sizeof(SIMD) * 2);
	static_assert(offsetof(fiber_context, _simd9) == sizeof(SIMD) * 3);
	static_assert(offsetof(fiber_context, _simd10) == sizeof(SIMD) * 4);
	static_assert(offsetof(fiber_context, _simd11) == sizeof(SIMD) * 5);
	static_assert(offsetof(fiber_context, _simd12) == sizeof(SIMD) * 6);
	static_assert(offsetof(fiber_context, _simd13) == sizeof(SIMD) * 7);
	static_assert(offsetof(fiber_context, _simd14) == sizeof(SIMD) * 8);
	static_assert(offsetof(fiber_context, _simd15) == sizeof(SIMD) * 9);

	static_assert(offsetof(fiber_context, _r12) == sizeof(SIMD) * 10);
	static_assert(offsetof(fiber_context, _r13) == sizeof(SIMD) * 10 + sizeof(var::uint64));
	static_assert(offsetof(fiber_context, _r14) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 2);
	static_assert(offsetof(fiber_context, _r15) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 3);

	static_assert(offsetof(fiber_context, _mxcsr) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4);
	static_assert(offsetof(fiber_context, _fpcw) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr));

	static_assert(offsetof(fiber_context, _rbx) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr) + sizeof(fpcw));
	static_assert(offsetof(fiber_context, _rbp) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr) + sizeof(fpcw) + sizeof(var::uint64));
	static_assert(offsetof(fiber_context, _rdi) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr) + sizeof(fpcw) + sizeof(var::uint64) * 2);
	static_assert(offsetof(fiber_context, _rsi) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr) + sizeof(fpcw) + sizeof(var::uint64) * 3);
	static_assert(offsetof(fiber_context, _rsp) == sizeof(SIMD) * 10 + sizeof(var::uint64) * 4 + sizeof(mxcsr) + sizeof(fpcw) + sizeof(var::uint64) * 4);
	
	static_assert(offsetof(fiber_impl, _context_ptr) == 0);
	static_assert(offsetof(fiber_impl, _absolute_begin_of_stack) == 8);
	static_assert(offsetof(fiber_impl, _stack_base) == 16);
	static_assert(offsetof(fiber_impl, _stack_limit) == 24);
	static_assert(offsetof(fiber_impl, _task_type) == 36);

	if (tl_s_current_fiber == nullptr) _FE_UNLIKELY_
	{
		return; // no active fiber to switch to.
	}

	TaskPriority l_priority = TaskPriority::_Critical;
	for (auto& active_fiber_queue : m_active_fibers)
	{
		if (active_fiber_queue.is_empty() == true)
		{
			FE::task l_to_execute;
			if (m_task_queue.try_pop(l_to_execute) == false)
			{
				l_priority = (TaskPriority)(l_priority + 1);
				continue;
			}

			fiber_impl* l_from = tl_s_current_fiber;
			FE::fiber l_from_fiber;
			l_from_fiber.m_impl = l_from;

			if (tl_s_current_fiber->_task_type == TaskPriority::_None) // flag for fibers that are not currently running any tasks; they can be immediately recycled back to the pool without being pushed to the active queue.
			{
				l_from->_context_ptr->_rsp = (var::byte*)l_from->_context_ptr; // reset the rsp
				m_fiber_pool.push(std::move(l_from_fiber));
			}
			else
			{
				m_active_fibers[l_from->_task_type].push(std::move(l_from_fiber));
			}

			FE::fiber l_to_switch = m_fiber_pool.pop(); // get a fiber from the pool to run the task.
			l_to_switch.m_impl->_task_type = l_to_execute._task_type;
			l_to_switch.m_impl->_context_ptr->_r12 = (var::uint64)l_to_execute._system;
			l_to_switch.m_impl->_context_ptr->_r13 = (var::uint64)l_to_execute._component; // smuggle the component pointer into r12; the fiber entry point will pass this as an argument to the system function when calling it.
			l_to_switch.m_impl->_context_ptr->_r14 = (var::uint64)this; // capture 'this' pointer for later use in the assembly.

			tl_s_current_fiber = l_to_switch.m_impl;
			__switch_to_new_fiber( l_from, l_to_switch.m_impl, sizeof(SIMD), &FE::fiber_scheduler::switch_fiber_context, &(m_thread_contexts[FE::framework::get_current_thread_id()]._thread_context)); // try passing the rsp to the func
			l_to_switch.m_impl = nullptr; // prevent double free
			return; // exit yield
		}

		// is the current active fiber list's priority higher than or equal to the currently running fiber's priority?
		if (tl_s_current_fiber->_task_type >= l_priority)
		{
			FE::fiber l_to_switch = active_fiber_queue.pop();

			fiber_impl* l_from = tl_s_current_fiber;
			FE::fiber l_from_fiber;
			l_from_fiber.m_impl = l_from;

			if (tl_s_current_fiber->_task_type == TaskPriority::_None) // flag for fibers that are not running any task; they should be returned to the pool instead of being put back to the active fiber list.
			{
				l_from->_context_ptr->_rsp = (var::byte*)l_from->_context_ptr; // reset the rsp
				m_fiber_pool.push(std::move(l_from_fiber));
			}
			else
			{
				m_active_fibers[l_from->_task_type].push(std::move(l_from_fiber));
			}

			tl_s_current_fiber = l_to_switch.m_impl;
			__switch_to_the_fiber( l_from, l_to_switch.m_impl, sizeof(SIMD));
			l_to_switch.m_impl = nullptr; // prevent double free
			return; // exit yield
		}

		l_priority = (TaskPriority)(l_priority + 1);
	}
}




_FE_CDECL_ FE::fiber::fiber(size_t stack_size_p) noexcept
	:	m_impl()
{
	m_impl = (fiber_impl*)tl_s_fiber_pool.allocate(sizeof(fiber_impl), alignof(fiber_impl));
	FE_ASSERT(m_impl != nullptr);

	__create_fiber(m_impl, stack_size_p);
}

_FE_CDECL_ FE::fiber::~fiber() noexcept
{
	if (m_impl == nullptr)
	{
		return; // moved-from state; nothing to clean up.
	}

	_FE_MAYBE_UNUSED_ auto l_result = VirtualFree(m_impl->_absolute_begin_of_stack, 0, MEM_RELEASE);
	FE_ASSERT(l_result != 0);

	tl_s_fiber_pool.deallocate(m_impl, sizeof(fiber_impl), alignof(fiber_impl));
	m_impl = nullptr;
}


_FE_CDECL_ FE::fiber::fiber(FE::fiber&& other_p) noexcept
	:	m_impl(other_p.m_impl)
{
	other_p.m_impl = nullptr;
}

FE::fiber& _FE_CDECL_ FE::fiber::operator=(FE::fiber&& other_p) noexcept
{
	m_impl = other_p.m_impl;
	other_p.m_impl = nullptr;
	return *this;
}


thread_local FE::memory_resource FE::fiber::tl_s_fiber_pool;
thread_local fiber_impl* FE::fiber_scheduler::tl_s_current_fiber = nullptr;


FE::thread_context::thread_context() noexcept
	: _thread_context()
{
	_thread_context._absolute_begin_of_stack = &m_page;

	_thread_context._stack_base = (var::byte*)_thread_context._absolute_begin_of_stack + sizeof(m_page);
	_thread_context._stack_limit = (var::byte*)_thread_context._absolute_begin_of_stack;

	// Multiple of 4096 is always multiple of 16, 32, and 64.
	var::uintptr l_fiber_page_ptr = (var::uintptr)_thread_context._stack_base;
	l_fiber_page_ptr -= sizeof(fiber_context); // allocate space for fiber context on the stack.
	_thread_context._context_ptr = (fiber_context*)l_fiber_page_ptr;
	memset(_thread_context._context_ptr, 0, sizeof(fiber_context));

	static_assert(sizeof(SIMD) == alignof(fiber_context));
	static_assert(sizeof(fiber_context) % 16 == 0);

	_thread_context._context_ptr->_rsp = (var::byte*)l_fiber_page_ptr;
	_thread_context._host_thread_id = FE::framework::get_current_thread_id();
}
