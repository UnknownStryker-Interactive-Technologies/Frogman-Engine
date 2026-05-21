#include <FE/pool/memory_resource.hxx>
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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




FE::internal::AllocatorType _FE_VECTOR_CALL_ FE::internal::__select_allocator(std::size_t bytes_p) noexcept
{
	switch (bytes_p)
	{
	case FE::xmmword_size:
		return FE::internal::AllocatorType::_XMMWordAllocator;

	case FE::ymmword_size:
		return FE::internal::AllocatorType::_YMMWordAllocator;

	case FE::zmmword_size:
		return FE::internal::AllocatorType::_ZMMWordAllocator;

	case FE::dzmmword_size:
		return FE::internal::AllocatorType::_DZMMWordAllocator;

	default:
		switch (bytes_p / FE::xmmword_size)
		{
		case 0:
			return FE::internal::AllocatorType::_XMMWordAllocator;

		case 1:
			return FE::internal::AllocatorType::_YMMWordAllocator;

		case 2:
			_FE_FALLTHROUGH_;
		case 3:
			return FE::internal::AllocatorType::_ZMMWordAllocator;

		case 4:
			_FE_FALLTHROUGH_;
		case 5:
			_FE_FALLTHROUGH_;
		case 6:
			_FE_FALLTHROUGH_;
		case 7:
			return FE::internal::AllocatorType::_DZMMWordAllocator;

		default:
			if (bytes_p > FE::system_page_size)
			{
				return FE::internal::AllocatorType::_AlignedMalloc;
			}
			return FE::internal::AllocatorType::_ScalableAllocator;
		}
	}
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
	switch (FE::internal::__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_XMMWordAllocator:
		return m_xmmword_block_pool.allocate< FE::align_as<xmmword_size, FE::align_16bytes> >();

	case internal::AllocatorType::_YMMWordAllocator:
		return  m_ymmword_block_pool.allocate< FE::align_as<ymmword_size, FE::align_32bytes> >();

	case internal::AllocatorType::_ZMMWordAllocator:
		return m_zmmword_block_pool.allocate< FE::align_as<zmmword_size, FE::align_64bytes> >();

	case internal::AllocatorType::_DZMMWordAllocator:
		return m_dzmmword_block_pool.allocate< FE::align_as<dzmmword_size, FE::align_128bytes> >();

	case internal::AllocatorType::_ScalableAllocator:
		return m_scalable_pool.allocate<std::byte>(bytes_p);

	case internal::AllocatorType::_AlignedMalloc:
		_FE_FALLTHROUGH_;
	default:
		return FE_ALIGNED_ALLOC((FE::int32)bytes_p, FE::SIMD_auto_alignment::size);
	}
	return nullptr;
}

void FE::memory_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	switch (FE::internal::__select_allocator(bytes_p))
	{
	case internal::AllocatorType::_XMMWordAllocator:
		m_xmmword_block_pool.deallocate< FE::align_as<xmmword_size, FE::align_16bytes> >( static_cast< FE::align_as<xmmword_size, FE::align_16bytes>* >( ptr_p ) );
		return;

	case internal::AllocatorType::_YMMWordAllocator:
		m_ymmword_block_pool.deallocate< FE::align_as<ymmword_size, FE::align_32bytes> >( static_cast<FE::align_as<ymmword_size, FE::align_32bytes>*>( ptr_p ) );
		return;

	case internal::AllocatorType::_ZMMWordAllocator:
		m_zmmword_block_pool.deallocate< FE::align_as<zmmword_size, FE::align_64bytes> >( static_cast<FE::align_as<zmmword_size, FE::align_64bytes>*>( ptr_p ) );
		return;

	case internal::AllocatorType::_DZMMWordAllocator:
		m_dzmmword_block_pool.deallocate< FE::align_as<dzmmword_size, FE::align_128bytes> >( static_cast<FE::align_as<dzmmword_size, FE::align_128bytes>*>( ptr_p ) );
		return;

	case internal::AllocatorType::_ScalableAllocator:
		m_scalable_pool.deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
		return;

	case internal::AllocatorType::_AlignedMalloc:
		_FE_FALLTHROUGH_;
	default:
		FE_ALIGNED_FREE(ptr_p);
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


namespace FE::large
{
	memory_resource::memory_resource(memory_resource&& other_p) noexcept
		: m_xmmword_block_pool(std::move(other_p.m_xmmword_block_pool)),
		m_ymmword_block_pool(std::move(other_p.m_ymmword_block_pool)),
		m_zmmword_block_pool(std::move(other_p.m_zmmword_block_pool)),
		m_dzmmword_block_pool(std::move(other_p.m_dzmmword_block_pool)),
		m_scalable_pool(std::move(other_p.m_scalable_pool))
	{
	}

	memory_resource& memory_resource::operator=(memory_resource&& other_p) noexcept
	{
		m_xmmword_block_pool = std::move(other_p.m_xmmword_block_pool);
		m_ymmword_block_pool = std::move(other_p.m_ymmword_block_pool);
		m_zmmword_block_pool = std::move(other_p.m_zmmword_block_pool);
		m_dzmmword_block_pool = std::move(other_p.m_dzmmword_block_pool);
		m_scalable_pool = std::move(other_p.m_scalable_pool);
		return *this;
	}

	void* memory_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
	{
		switch (FE::internal::__select_allocator(bytes_p))
		{
		case internal::AllocatorType::_XMMWordAllocator:
			return m_xmmword_block_pool.allocate< FE::align_as<xmmword_size, FE::align_16bytes> >();

		case internal::AllocatorType::_YMMWordAllocator:
			return  m_ymmword_block_pool.allocate< FE::align_as<ymmword_size, FE::align_32bytes> >();

		case internal::AllocatorType::_ZMMWordAllocator:
			return m_zmmword_block_pool.allocate< FE::align_as<zmmword_size, FE::align_64bytes> >();

		case internal::AllocatorType::_DZMMWordAllocator:
			return m_dzmmword_block_pool.allocate< FE::align_as<dzmmword_size, FE::align_128bytes> >();

		case internal::AllocatorType::_ScalableAllocator:
			return m_scalable_pool.allocate<std::byte>(bytes_p);

		case internal::AllocatorType::_AlignedMalloc:
			_FE_FALLTHROUGH_;
		default:
			return FE_ALIGNED_ALLOC((FE::int32)bytes_p, FE::SIMD_auto_alignment::size);
		}
		return nullptr;
	}

	void memory_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
	{
		switch (FE::internal::__select_allocator(bytes_p))
		{
		case internal::AllocatorType::_XMMWordAllocator:
			m_xmmword_block_pool.deallocate< FE::align_as<xmmword_size, FE::align_16bytes> >(static_cast<FE::align_as<xmmword_size, FE::align_16bytes>*>(ptr_p));
			return;

		case internal::AllocatorType::_YMMWordAllocator:
			m_ymmword_block_pool.deallocate< FE::align_as<ymmword_size, FE::align_32bytes> >(static_cast<FE::align_as<ymmword_size, FE::align_32bytes>*>(ptr_p));
			return;

		case internal::AllocatorType::_ZMMWordAllocator:
			m_zmmword_block_pool.deallocate< FE::align_as<zmmword_size, FE::align_64bytes> >(static_cast<FE::align_as<zmmword_size, FE::align_64bytes>*>(ptr_p));
			return;

		case internal::AllocatorType::_DZMMWordAllocator:
			m_dzmmword_block_pool.deallocate< FE::align_as<dzmmword_size, FE::align_128bytes> >(static_cast<FE::align_as<dzmmword_size, FE::align_128bytes>*>(ptr_p));
			return;

		case internal::AllocatorType::_ScalableAllocator:
			m_scalable_pool.deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
			return;

		case internal::AllocatorType::_AlignedMalloc:
			_FE_FALLTHROUGH_;
		default:
			FE_ALIGNED_FREE(ptr_p);
			return;
		}
	}

	bool memory_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
	{
		const memory_resource* l_other = dynamic_cast<const memory_resource*>(&other_p);
		if (l_other == nullptr)
		{
			return false;
		}

		return &(m_scalable_pool) == &(l_other->m_scalable_pool);
	}
}