#ifndef _FE_FRAMEWORK_TASK_HPP_
#define _FE_FRAMEWORK_TASK_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/clock.hpp>

// std::shared_ptr
#include <memory>

// std::pmr::synchronized_pool_resource
#include <memory_resource>

// std::mutex, std::lock_guard
#include <mutex>

// std::thread
#include <thread>

#include <taskflow.hpp>

// boost::function
#include <boost/function.hpp>

#include <FE/synchronized_access.hxx>




BEGIN_NAMESPACE(FE::framework)


FE::uint16 get_current_thread_id() noexcept;


class latent_event
{
public:
	using function_type = boost::function<void()>;

private:
    function_type m_event;
	var::float64 m_delay_in_milliseconds;
	FE::clock m_timer;

public:
	latent_event(const function_type& task_p, FE::float64 delay_in_milliseconds_p) noexcept;
	latent_event(const latent_event& other_p) noexcept;
	latent_event(latent_event&& other_p) noexcept;

	~latent_event() noexcept = default;

	latent_event& operator=(const latent_event& other_p) noexcept;
	latent_event& operator=(latent_event&& other_p) noexcept;

	FE::boolean is_ready() noexcept;
	void operator()();
};


class framework_base;

class task_scheduler
{
	friend class framework_base;

	std::atomic_bool m_is_inturrupted;

	tf::Executor m_executor;

	task_scheduler(uint32 max_concurrency_p) noexcept;
	~task_scheduler() noexcept;

public:
	tf::Executor& access_executor() noexcept;

	void interrupt() noexcept;
	void launch_latent_event(const typename latent_event::function_type& task_p, FE::uint64 delay_in_milliseconds_p) noexcept;
};


END_NAMESPACE
#endif