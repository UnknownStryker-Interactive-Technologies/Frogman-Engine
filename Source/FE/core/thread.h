#ifndef _FE_CORE_THREAD_H_
#define _FE_CORE_THREAD_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>

// std
#include <thread>

#ifdef _WINDOWS_X86_64_
	#include <processthreadsapi.h>

#elif defined(_LINUX_X86_64_)
	#include <pthread.h>
	#include <unistd.h>
#endif




BEGIN_NAMESPACE(FE::thread)


_FORCE_INLINE_ uint64 this_thread_id() noexcept
{
#ifdef _WINDOWS_X86_64_
	return GetCurrentThreadId();
#elif defined(_LINUX_X86_64_)
	return static_cast<var::uint64>(pthread_self());
#endif
}

_FORCE_INLINE_ uint64 this_process_id() noexcept
{
#ifdef _WINDOWS_X86_64_
	return GetCurrentProcessId();
#elif defined(_LINUX_X86_64_)
	return static_cast<var::uint64>(getpid());
#endif
}

END_NAMESPACE
#endif