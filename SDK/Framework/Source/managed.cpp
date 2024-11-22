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
#include <FE/framework/managed.hpp>
#include <FE/fqueue.hxx>

// std::mutex, std::lock_guard
#include <mutex>




BEGIN_NAMESPACE(FE::framework)


namespace internal::thread_id
{
	static FE::fqueue<FE::aligned<var::uint8, FE::align_CPU_L1_cache_line>, 255> s_thread_ids;
	static std::uint8_t s_next_thread_id = 0;
	static std::mutex s_lock;

	class __generator
	{
		var::uint8 m_thread_id;

	public:
		__generator() noexcept
		{
			std::lock_guard<std::mutex> l_lock(s_lock);
			if (s_thread_ids.is_empty() == false)
			{
				this->m_thread_id = s_thread_ids.pop()._data;
				return;
			}
			FE_ASSERT(s_next_thread_id < 255);
			this->m_thread_id = s_next_thread_id;
			++s_next_thread_id;
		}

		~__generator() noexcept
		{
			std::lock_guard<std::mutex> l_lock(s_lock);
			s_thread_ids.push((FE::aligned<var::uint8, FE::align_CPU_L1_cache_line>)this->m_thread_id);
		}

		FE::uint8 get_id() const noexcept
		{
			return this->m_thread_id;
		}
	};
}

FE::uint8 get_current_thread_id() noexcept
{
	thread_local static internal::thread_id::__generator tl_s_id_generator;
	return tl_s_id_generator.get_id();
}


std::unique_ptr<FE::block_pool<managed::ref_block_size, FE::SIMD_auto_alignment>[]> managed::s_thread_local_ref_tables;

void managed::initialize(var::uint32 max_concurrency_p) noexcept
{
	managed::s_thread_local_ref_tables = std::make_unique<FE::block_pool<ref_block_size, FE::SIMD_auto_alignment>[]>(max_concurrency_p);
}

void managed::shutdown() noexcept
{
	managed::s_thread_local_ref_tables.reset();
}


END_NAMESPACE
