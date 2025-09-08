#include <FE/pool/memory_resource.hpp>
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




_FE_FORCE_INLINE_ FE::internal::AllocatorType __select_allocator(std::size_t bytes_p) noexcept
{
	FE::internal::AllocatorType l_allocator_type; 

	switch (bytes_p)
	{
	case FE::internal::xmmword_size:
		return FE::internal::AllocatorType::_XMMWordAllocator;

	case FE::internal::ymmword_size:
		return FE::internal::AllocatorType::_YMMWordAllocator;

	case FE::internal::zmmword_size:
		return FE::internal::AllocatorType::_ZMMWordAllocator;

	case FE::internal::dzmmword_size:
		return FE::internal::AllocatorType::_DZMMWordAllocator;

	default:
		break;
	}

	constexpr std::size_t l_smallest_block_size = FE::internal::xmmword_size;
	l_allocator_type = static_cast<FE::internal::AllocatorType>( bytes_p / l_smallest_block_size );
	return l_allocator_type;
}




FE::memory_resource::memory_resource(FE::memory_resource&& other_p) noexcept
	:	m_xmmword_block_pool( std::move(other_p.m_xmmword_block_pool) ),
		m_ymmword_block_pool( std::move(other_p.m_ymmword_block_pool) ),
		m_zmmword_block_pool( std::move(other_p.m_zmmword_block_pool) ),
		m_dzmmword_block_pool( std::move(other_p.m_dzmmword_block_pool) ),
		m_scalable_pool( std::move(other_p.m_scalable_pool) )
{}

FE::memory_resource& FE::memory_resource::operator=(FE::memory_resource&& other_p) noexcept
{
	m_xmmword_block_pool = std::move(other_p.m_xmmword_block_pool);
	m_ymmword_block_pool = std::move(other_p.m_ymmword_block_pool);
	m_zmmword_block_pool = std::move(other_p.m_zmmword_block_pool);
	m_dzmmword_block_pool = std::move(other_p.m_dzmmword_block_pool);
	m_scalable_pool = std::move(other_p.m_scalable_pool);
	return *this;
}

void* FE::memory_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	void* l_allocation_result;

	switch (__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_XMMWordAllocator:
		l_allocation_result = m_xmmword_block_pool.allocate< FE::align_as<internal::xmmword_size, FE::align_16bytes> >();
		break;

	case internal::AllocatorType::_YMMWordAllocator:
		l_allocation_result =  m_ymmword_block_pool.allocate< FE::align_as<internal::ymmword_size, FE::align_32bytes> >();
		break;

	case internal::AllocatorType::_ZMMWordAllocator:
		l_allocation_result = m_zmmword_block_pool.allocate< FE::align_as<internal::zmmword_size, FE::align_64bytes> >();
		break;

	case internal::AllocatorType::_DZMMWordAllocator:
		l_allocation_result = m_dzmmword_block_pool.allocate< FE::align_as<internal::dzmmword_size, FE::align_128bytes> >();
		break;

	default:
		return m_scalable_pool.allocate<std::byte>((FE::int32)bytes_p);
	}

	if (l_allocation_result == nullptr) _FE_UNLIKELY_
	{
		return m_scalable_pool.allocate<std::byte>((FE::int32)bytes_p);
	}

	return l_allocation_result;
}

void FE::memory_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	bool l_deallocation_result;

	switch (__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_XMMWordAllocator:
		l_deallocation_result = m_xmmword_block_pool.deallocate< FE::align_as<internal::xmmword_size, FE::align_16bytes> >( static_cast< FE::align_as<internal::xmmword_size, FE::align_16bytes>* >( ptr_p ) );
		break;

	case internal::AllocatorType::_YMMWordAllocator:
		l_deallocation_result = m_ymmword_block_pool.deallocate< FE::align_as<internal::ymmword_size, FE::align_32bytes> >( static_cast<FE::align_as<internal::ymmword_size, FE::align_32bytes>*>( ptr_p ) );
		break;

	case internal::AllocatorType::_ZMMWordAllocator:
		l_deallocation_result = m_zmmword_block_pool.deallocate< FE::align_as<internal::zmmword_size, FE::align_64bytes> >( static_cast<FE::align_as<internal::zmmword_size, FE::align_64bytes>*>( ptr_p ) );
		break;

	case internal::AllocatorType::_DZMMWordAllocator:
		l_deallocation_result = m_dzmmword_block_pool.deallocate< FE::align_as<internal::dzmmword_size, FE::align_128bytes> >( static_cast<FE::align_as<internal::dzmmword_size, FE::align_128bytes>*>( ptr_p ) );
		break;

	default:
		l_deallocation_result = m_scalable_pool.deallocate<std::byte>( static_cast<std::byte*>(ptr_p), (FE::int32)bytes_p );
		FE_ASSERT(l_deallocation_result == true, "Assertion failed: the deallocation of the scalable pool failed; ptr_p is an alien pointer.");
		return;
	}

	if (l_deallocation_result == false)
	{
		l_deallocation_result = m_scalable_pool.deallocate<std::byte>( static_cast<std::byte*>(ptr_p), (FE::int32)bytes_p );
		FE_ASSERT(l_deallocation_result == true, "Assertion failed: the deallocation of the scalable pool failed; ptr_p is an alien pointer.");
		return;
	}
}

bool FE::memory_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
{
	const memory_resource* l_other = dynamic_cast<const memory_resource*>(&other_p);
	if (l_other == nullptr)
	{
		return false;
	}

	return &(m_scalable_pool) == &(l_other->m_scalable_pool);
}
