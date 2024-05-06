#ifndef _FE_CORE_PRIVATE_POOL_ALLOCATOR_BASE_HXX_
#define _FE_CORE_PRIVATE_POOL_ALLOCATOR_BASE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/allocator_base.hpp>
#include <FE/core/pool.hxx>

//std
#include <memory>




BEGIN_NAMESPACE(FE)


template<class Alignment>
class pool_allocator_base : public allocator_base
{
public:
	using alignment_type = Alignment;
	using pool_type = FE::generic_pool<capacity<512 MB>::size, Alignment, FE::aligned_allocator<internal::pool::chunk<void, POOL_TYPE::_GENERIC, capacity<512 MB>::size, Alignment>>, FE::aligned_allocator<std::pair<const FE::memory_region_t, internal::pool::chunk<void, POOL_TYPE::_GENERIC, capacity<512 MB>::size, Alignment>>>>;

protected:
	static std::unique_ptr<pool_type> s_pool;

public:
	_FORCE_INLINE_ static void create_pool_allocator_resource(count_t pages_p) noexcept
	{
		s_pool = std::make_unique<pool_type>();
		s_pool->create_pages(pages_p);
	}

	_FORCE_INLINE_ static void destroy_pool_allocator_resource() noexcept
	{
		s_pool.reset();
	}
};


template<class Alignment>
std::unique_ptr<typename FE::pool_allocator_base<Alignment>::pool_type> FE::pool_allocator_base<Alignment>::s_pool;


END_NAMESPACE
#endif 
