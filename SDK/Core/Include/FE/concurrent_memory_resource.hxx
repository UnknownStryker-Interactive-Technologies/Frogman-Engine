#ifndef _FE_FRAMEWORK_CONCURRENT_MEMORY_RESOURCE_HXX_
#define _FE_FRAMEWORK_CONCURRENT_MEMORY_RESOURCE_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/memory_resource.hxx>

#include <memory_resource>

#include <concurrent_queue.h>




BEGIN_NAMESPACE(FE)


namespace internal::concurrent_memory_resource
{
	struct deallocation_request
	{
		void* _ptr;
		var::size _size;
	};
}


class concurrent_memory_resource : public std::pmr::memory_resource
{
	static_assert(FE::memory_resource::page_granularity_in_bytes == 64 * FE::one_KiB);
	static_assert(FE::large::memory_resource::page_granularity_in_bytes == 2 * FE::one_MiB);
	//static_assert(FE::large::memory_resource::super_large_page_granularity_in_bytes == 1 * FE::one_GiB);

	var::boolean m_is_large_page_enabled;
	var::uint32 m_max_concurrency;

	std::pmr::memory_resource* m_thread_local_memory_resource;

	using deallocation_queue = concurrency::concurrent_queue
		<
		internal::concurrent_memory_resource::deallocation_request,
		std::pmr::polymorphic_allocator<internal::concurrent_memory_resource::deallocation_request>
		>;
	std::unique_ptr<std::pmr::synchronized_pool_resource[]> m_thread_local_m_deallocation_queue_resources;
	std::vector<deallocation_queue> m_deallocation_queues;

public:
	concurrent_memory_resource(FE::boolean is_large_page_enabled_p, FE::uint32 max_concurrency_p) noexcept;
	virtual ~concurrent_memory_resource() noexcept override;

	concurrent_memory_resource(concurrent_memory_resource&& other_p) noexcept;
	concurrent_memory_resource& operator=(concurrent_memory_resource&& other_p) noexcept;

	concurrent_memory_resource(const concurrent_memory_resource&) = delete;
	concurrent_memory_resource& operator=(const concurrent_memory_resource&) = delete;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void  do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;

	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;

	FE::uint16 __get_page_thread_id(void* ptr_p, std::size_t bytes_p) const noexcept;
};


END_NAMESPACE
#endif