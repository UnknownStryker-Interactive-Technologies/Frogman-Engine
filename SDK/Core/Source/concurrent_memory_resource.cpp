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
#include <FE/concurrent_memory_resource.hxx>




FE::concurrent_memory_resource::concurrent_memory_resource(FE::boolean is_large_page_enabled_p, FE::uint32 max_concurrency_p) noexcept
	:	m_is_large_page_enabled(is_large_page_enabled_p), 
		m_max_concurrency(max_concurrency_p),

		m_thread_local_memory_resource(	(is_large_page_enabled_p == true) ? 
									  		new FE::large::memory_resource[max_concurrency_p]{}
									  	:	(std::pmr::memory_resource*)new FE::memory_resource[max_concurrency_p]{}
									  ),

		m_thread_local_m_deallocation_queue_resources(std::make_unique<std::pmr::synchronized_pool_resource[]>(max_concurrency_p)),
		m_deallocation_queues()
{
	m_deallocation_queues.reserve(max_concurrency_p);

	for (var::uint32 i = 0; i < max_concurrency_p; ++i)
	{
		std::pmr::polymorphic_allocator<internal::concurrent_memory_resource::deallocation_request> l_allocator(&m_thread_local_m_deallocation_queue_resources[i]);
		m_deallocation_queues.emplace_back(l_allocator);
	}
}

FE::concurrent_memory_resource::~concurrent_memory_resource() noexcept
{
	if (m_is_large_page_enabled)
	{
		delete[] (FE::large::memory_resource*)m_thread_local_memory_resource;
		return;
	}

	delete[] (FE::memory_resource*)m_thread_local_memory_resource;
}

FE::concurrent_memory_resource::concurrent_memory_resource(concurrent_memory_resource&& other_p) noexcept
	:	m_is_large_page_enabled(other_p.m_is_large_page_enabled),
		m_max_concurrency(other_p.m_max_concurrency),
		m_thread_local_memory_resource(other_p.m_thread_local_memory_resource),
		m_deallocation_queues(std::move(other_p.m_deallocation_queues))
{
	other_p.m_thread_local_memory_resource = nullptr;
}

FE::concurrent_memory_resource& FE::concurrent_memory_resource::operator=(concurrent_memory_resource&& other_p) noexcept
{
	if (this == &other_p)
	{
		return *this;
	}


	if (m_is_large_page_enabled)
	{
		delete[](FE::large::memory_resource*)m_thread_local_memory_resource;
		return *this;
	}

	delete[](FE::memory_resource*)m_thread_local_memory_resource;


	m_is_large_page_enabled = other_p.m_is_large_page_enabled;
	m_max_concurrency = other_p.m_max_concurrency;

	m_thread_local_memory_resource = other_p.m_thread_local_memory_resource;
	other_p.m_thread_local_memory_resource = nullptr;

	m_deallocation_queues = std::move(other_p.m_deallocation_queues);

	return *this;
}

void* FE::concurrent_memory_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	// compute once
	static FE::size l_s_size_of_page_element = (m_is_large_page_enabled == true) ? sizeof(FE::large::memory_resource) : sizeof(FE::memory_resource);

	std::pmr::memory_resource*  l_resource = (std::pmr::memory_resource*)(((var::byte*)m_thread_local_memory_resource) + (l_s_size_of_page_element * get_current_thread_id()));
	return l_resource->allocate(bytes_p, alignment_p);
}

void  FE::concurrent_memory_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	// compute once
	static FE::size l_s_size_of_page_element = (m_is_large_page_enabled == true) ? sizeof(FE::large::memory_resource) : sizeof(FE::memory_resource);

	FE::uint16 l_current_thread_id = get_current_thread_id();
	FE::uint16 l_page_thread_id = __get_page_thread_id(ptr_p, bytes_p);

	if (l_current_thread_id == l_page_thread_id)
	{
		std::pmr::memory_resource* l_resource = (std::pmr::memory_resource*)(((var::byte*)m_thread_local_memory_resource) + (l_s_size_of_page_element * l_current_thread_id));
		l_resource->deallocate(ptr_p, bytes_p, alignment_p);

		while (m_deallocation_queues[l_current_thread_id].empty() == false)
		{
			internal::concurrent_memory_resource::deallocation_request l_request;
			if (m_deallocation_queues[l_current_thread_id].try_pop(l_request) == true)
			{
				l_resource->deallocate(l_request._ptr, l_request._size, alignment_p);
			}
		}
		return;
	}

	// defer the deallocation to the thread that owns the page
	m_deallocation_queues[l_page_thread_id].push({ ptr_p, bytes_p });
}

bool FE::concurrent_memory_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
{
	return this == &other_p;
}

FE::uint16 FE::concurrent_memory_resource::concurrent_memory_resource::__get_page_thread_id(void* ptr_p, std::size_t bytes_p) const noexcept
{
	// compute once
	static FE::size l_s_page_granularity = (m_is_large_page_enabled == true) ? FE::large::memory_resource::page_granularity_in_bytes : FE::memory_resource::page_granularity_in_bytes;

	// use a pointer arithmetic to find which page the pointer belongs to.
	void* l_page_base = reinterpret_cast<void*>(reinterpret_cast<FE::uintptr>(ptr_p) - (reinterpret_cast<FE::uintptr>(ptr_p) % l_s_page_granularity));

	if (m_is_large_page_enabled)
	{
		switch (large::internal::__select_allocator(bytes_p))
		{
		case internal::AllocatorType::_XMMWordAllocator:
			return ((FE::large::memory_resource::xmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

		case internal::AllocatorType::_YMMWordAllocator:
			return ((FE::large::memory_resource::ymmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

		case internal::AllocatorType::_ZMMWordAllocator:
			return ((FE::large::memory_resource::zmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

		case internal::AllocatorType::_DZMMWordAllocator:
			return ((FE::large::memory_resource::dzmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

		case internal::AllocatorType::_ScalableAllocator:
			return ((FE::large::memory_resource::scalable_pool_type::chunk_type*)l_page_base)->get_thread_id();

		case internal::AllocatorType::_VirtualAlloc:
			_FE_FALLTHROUGH_;
		default:
			return get_current_thread_id(); // the allocation was made by VirtualAlloc, so we do not need to defer the deallocation to another thread.

		}
	}


	switch (internal::__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_XMMWordAllocator:
		return ((FE::memory_resource::xmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

	case internal::AllocatorType::_YMMWordAllocator:
		return ((FE::memory_resource::ymmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

	case internal::AllocatorType::_ZMMWordAllocator:
		return ((FE::memory_resource::zmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

	case internal::AllocatorType::_DZMMWordAllocator:
		return ((FE::memory_resource::dzmmword_pool_type::chunk_type*)l_page_base)->get_thread_id();

	case internal::AllocatorType::_ScalableAllocator:
		return ((FE::memory_resource::scalable_pool_type::chunk_type*)l_page_base)->get_thread_id();

	case internal::AllocatorType::_VirtualAlloc:
		_FE_FALLTHROUGH_;
	default:
		return get_current_thread_id(); // the allocation was made by VirtualAlloc, so we do not need to defer the deallocation to another thread.
	}
}
