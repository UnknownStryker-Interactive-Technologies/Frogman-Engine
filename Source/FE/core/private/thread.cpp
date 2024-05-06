// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/thread.hpp>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/function.hxx>


#ifdef _WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#elif defined(_LINUX_X86_64_)
#include <pthread.h>
#endif


thread_local FE::var::uint64 FE::thread::tl_s_this_thread_instance_id = 0;


FE::thread::thread(thread& other_p) noexcept : m_thread(::std::move(other_p.m_thread)) {}
FE::thread::thread(thread&& rvalue_p) noexcept : m_thread(::std::move(rvalue_p.m_thread)) {}

FE::thread& FE::thread::operator=(thread& other_p) noexcept
{
	this->m_thread = ::std::move(other_p.m_thread);
	return *this;
}

FE::thread& FE::thread::operator=(thread&& rvalue_p) noexcept
{
	this->m_thread = ::std::move(rvalue_p.m_thread);
	return *this;
};

void FE::thread::fork(FE::task_base* const function_p) noexcept
{
	FE_ASSERT(function_p == nullptr, "ERROR: function_p is nullptr.");

	this->m_thread = ::std::thread
	(
		[&, function_p]()
		{
			tl_s_this_thread_instance_id = __generate_instance_id();
			function_p->operator()();
		}
	);
}


void FE::thread::swap(thread& in_out_other_p) noexcept
{
	algorithm::utility::swap(*this, in_out_other_p);
}

FE::uint64 FE::thread::calculate_suitable_thread_count() noexcept
{
	if ( ((::std::thread::hardware_concurrency() >> 1) + (::std::thread::hardware_concurrency() / 8)) < minimum_suitable_thread_count)
	{
		return minimum_suitable_thread_count;
	}

	return static_cast<var::uint64>(::std::thread::hardware_concurrency() >> 1) + static_cast<var::uint64>(::std::thread::hardware_concurrency() / 8);
}

FE::uint64 FE::thread::this_thread_id() noexcept
{
#ifdef _WINDOWS_X86_64_
	return GetCurrentThreadId();
#elif defined(_LINUX_X86_64_)
	return (FE::var::uint64)pthread_self();
#endif
}

FE::uint64 FE::thread::__generate_instance_id() noexcept
{
	static var::uint64 l_s_id = 0;
	return ++l_s_id;
}