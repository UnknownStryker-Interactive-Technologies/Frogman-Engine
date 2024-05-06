#ifndef _FE_CORE_THREAD_HPP_
#define _FE_CORE_THREAD_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <thread>




BEGIN_NAMESPACE(FE)


struct task_base;


class thread final
{
	std::thread m_thread;

public:
	thread_local static var::uint64 tl_s_this_thread_instance_id;

	typedef std::thread thread_type;
	typedef var::uint64 id_type;

	_MAYBE_UNUSED_ static constexpr var::uint16 minimum_suitable_thread_count = 6;
	_MAYBE_UNUSED_ static constexpr var::uint16 max_thread_id_digit_length = 24;

	thread() noexcept = default;
	~thread() noexcept = default;
	thread(thread& other_p) noexcept;
	thread(thread&& rvalue_p) noexcept;

	thread& operator=(thread& other_p) noexcept;
	thread& operator=(thread&& rvalue_p) noexcept;

	_FORCE_INLINE_ boolean is_this_thread_active() noexcept
	{
		return this->m_thread.joinable();
	}

	void fork(FE::task_base* const function_p) noexcept;

	_FORCE_INLINE_ void join() noexcept
	{
		this->m_thread.join();
	}
	
	void swap(thread& in_out_other_p) noexcept;
	static uint64 calculate_suitable_thread_count() noexcept;

	static uint64 this_thread_id() noexcept;

private:
	static uint64 __generate_instance_id() noexcept;
};


END_NAMESPACE
#endif