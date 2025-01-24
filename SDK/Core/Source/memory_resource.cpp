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
	constexpr std::size_t l_smallest_block_size = 128;
	FE::internal::AllocatorType l_allocator_type = static_cast<FE::internal::AllocatorType>((bytes_p / l_smallest_block_size) + 1);

	if (l_smallest_block_size == bytes_p) _FE_UNLIKELY_
	{
		return FE::internal::AllocatorType::_DoubleZMMWordAllocator;
	}

	return l_allocator_type;
}


FE::memory_resource::memory_resource(FE::memory_resource&& other_p) noexcept
	: m_dzmmword_block_pool( std::move(other_p.m_dzmmword_block_pool) ),
	  m_qzmmword_block_pool( std::move(other_p.m_qzmmword_block_pool) ),
	  m_ozmmword_block_pool( std::move(other_p.m_ozmmword_block_pool) ),
	  m_scalable_pool( std::move(other_p.m_scalable_pool) )
{}

FE::memory_resource& FE::memory_resource::operator=(FE::memory_resource&& other_p) noexcept
{
	this->m_dzmmword_block_pool = std::move(other_p.m_dzmmword_block_pool);
	this->m_qzmmword_block_pool = std::move(other_p.m_qzmmword_block_pool);
	this->m_ozmmword_block_pool = std::move(other_p.m_ozmmword_block_pool);
	this->m_scalable_pool = std::move(other_p.m_scalable_pool);
	return *this;
}

void* FE::memory_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	switch (__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_DoubleZMMWordAllocator:
		return this->m_dzmmword_block_pool.allocate< FE::align_as<internal::double_zmmword_size, FE::align_128bytes> >();

	case internal::AllocatorType::_QuadZMMWordAllocator:
		return this->m_qzmmword_block_pool.allocate< FE::align_as<internal::quad_zmmword_size, FE::align_128bytes> >();

	case internal::AllocatorType::_OctaZMMWordAllocator:
		return this->m_ozmmword_block_pool.allocate< FE::align_as<internal::octa_zmmword_size, FE::align_128bytes> >();

	default:
		return this->m_scalable_pool.allocate<std::byte>(bytes_p);
	}
}

void FE::memory_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	switch (__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_DoubleZMMWordAllocator:
		this->m_dzmmword_block_pool.deallocate< FE::align_as<internal::double_zmmword_size, FE::align_128bytes> >( static_cast< FE::align_as<internal::double_zmmword_size, FE::align_128bytes>* >( ptr_p ) );
		return;

	case internal::AllocatorType::_QuadZMMWordAllocator:
		this->m_qzmmword_block_pool.deallocate< FE::align_as<internal::quad_zmmword_size, FE::align_128bytes> >( static_cast< FE::align_as<internal::quad_zmmword_size, FE::align_128bytes> *>( ptr_p ) );
		return;

	case internal::AllocatorType::_OctaZMMWordAllocator:
		this->m_ozmmword_block_pool.deallocate< FE::align_as<internal::octa_zmmword_size, FE::align_128bytes> >( static_cast< FE::align_as<internal::octa_zmmword_size, FE::align_128bytes> *>( ptr_p ) );
		return;

	default:
		this->m_scalable_pool.deallocate<std::byte>( static_cast<std::byte*>(ptr_p), bytes_p);
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

	return &(this->m_scalable_pool) == &(l_other->m_scalable_pool);
}
