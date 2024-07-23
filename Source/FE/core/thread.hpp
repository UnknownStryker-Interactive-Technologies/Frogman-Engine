#ifndef _FE_CORE_THREAD_HPP_
#define _FE_CORE_THREAD_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/function.hxx>

// std
#include <atomic>
#include <thread>

#ifdef _WINDOWS_X86_64_
	#include <processthreadsapi.h>

#elif defined(_LINUX_X86_64_)
	#include <pthread.h>
	#include <unistd.h>
#endif




BEGIN_NAMESPACE(FE)


class task_base;

// needs some work...
class thread final
{
public:
	thread_local static var::uint64 tl_s_this_thread_instance_id;

	using thread_type = std::thread;
	using id_type = var::uint64;

private:
	thread_type m_thread;
	std::any m_task_result;

public:
	_MAYBE_UNUSED_ static constexpr var::uint16 minimum_suitable_thread_count = 4;
	_MAYBE_UNUSED_ static constexpr var::uint16 max_thread_id_digit_length = 24;

	_FORCE_INLINE_ thread() noexcept = default;
	_FORCE_INLINE_ ~thread() noexcept = default;
	thread(const thread&) = delete;
	_FORCE_INLINE_ thread(thread&& rvalue_p) noexcept : m_thread(std::move(rvalue_p.m_thread)) {}

	_FORCE_INLINE_ thread& operator=(const thread&) noexcept = delete;
	_FORCE_INLINE_ thread& operator=(thread&& rvalue_p) noexcept { this->m_thread = std::move(rvalue_p.m_thread); return *this; };

	// fork()

	_FORCE_INLINE_ void join() { this->m_thread.join(); }

	_FORCE_INLINE_ static uint64 calculate_suitable_thread_count() noexcept
	{
		if ( ((::std::thread::hardware_concurrency() >> 1) + (::std::thread::hardware_concurrency() / 8)) < minimum_suitable_thread_count)
		{
			return minimum_suitable_thread_count;
		}
		return static_cast<var::uint64>(::std::thread::hardware_concurrency() >> 1) + static_cast<var::uint64>(::std::thread::hardware_concurrency() / 8);
	}

	_FORCE_INLINE_ boolean is_this_thread_active() const noexcept { return this->m_thread.joinable(); }

	_FORCE_INLINE_ static id_type this_thread_id() noexcept
	{
#ifdef _WINDOWS_X86_64_
		return GetCurrentThreadId();
#elif defined(_LINUX_X86_64_)
		return static_cast<var::uint64>(pthread_self());
#endif
	}

	_FORCE_INLINE_ static id_type this_process_id() noexcept
	{
#ifdef _WINDOWS_X86_64_
		return GetCurrentProcessId();
#elif defined(_LINUX_X86_64_)
		return static_cast<var::uint64>(getpid());
#endif
	}

private:
	_FORCE_INLINE_ static id_type __generate_instance_id() noexcept
	{
		static std::atomic<id_type> l_s_id = 0;
		return ++l_s_id;
	}
};


END_NAMESPACE
#endif