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
#include <FE/framework/thread_id.hpp>
#include <FE/fqueue.hxx>

#include <atomic>
// std::mutex, std::lock_guard
#include <mutex>

// boost::thread
#include <boost/thread.hpp>

// Microsoft Parallel Patterns Library
#include <concurrent_queue.h>




BEGIN_NAMESPACE(FE::framework)


namespace internal::thread_id
{
	static concurrency::concurrent_queue<var::uint16> s_thread_ids;
	static std::atomic_uint16_t s_next_thread_id = 0;

	class __generator
	{
		var::uint16 m_thread_id;

	public:
		__generator() noexcept
		{
			if (s_thread_ids.try_pop(this->m_thread_id) == false)
			{
				FE_ASSERT(s_next_thread_id.load() < FE::max_value<var::uint16>);
				this->m_thread_id = s_next_thread_id++;
			}
		}

		~__generator() noexcept
		{
			s_thread_ids.push(this->m_thread_id);
		}

		FE::uint16 get_id() const noexcept
		{
			return this->m_thread_id;
		}
	};
}

FE::uint16 get_current_thread_id() noexcept
{
	thread_local static internal::thread_id::__generator tl_s_id_generator;
	return tl_s_id_generator.get_id();
}


END_NAMESPACE


