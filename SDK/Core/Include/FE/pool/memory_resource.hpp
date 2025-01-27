#ifndef _FE_POOL_MEMORY_RESOURCE_HPP_
#define _FE_POOL_MEMORY_RESOURCE_HPP_
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
#include <FE/memory.hpp>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>

#include <memory_resource>
#include <mutex>




BEGIN_NAMESPACE(FE)


namespace internal
{
	constexpr FE::size double_zmmword_size = 128;
	constexpr FE::size quad_zmmword_size = 256;
	constexpr FE::size octa_zmmword_size = 512;

	enum struct AllocatorType
	{
		_DoubleZMMWordAllocator = 1,
		_QuadZMMWordAllocator = 2,
		_OctaZMMWordAllocator = 3
	};
}


/*
The FE::memory_resource is a class template provides a memory resource that utilizes a bunch of pool allocators for efficient memory management
inheriting from std::pmr::memory_resource and FE::internal::allocator_base.
*/
class memory_resource : public std::pmr::memory_resource
{
	FE::block_pool<FE::PoolPageCapacity::_16MB, internal::double_zmmword_size, FE::SIMD_auto_alignment> m_dzmmword_block_pool;
	FE::block_pool<FE::PoolPageCapacity::_16MB, internal::quad_zmmword_size, FE::SIMD_auto_alignment> m_qzmmword_block_pool;
	FE::block_pool<FE::PoolPageCapacity::_16MB, internal::octa_zmmword_size, FE::SIMD_auto_alignment> m_ozmmword_block_pool;
	FE::scalable_pool<FE::PoolPageCapacity::_16MB, FE::SIMD_auto_alignment> m_scalable_pool;

public:
	memory_resource() noexcept = default;
	virtual ~memory_resource() noexcept = default;

	memory_resource(memory_resource&& other_p) noexcept;
	memory_resource& operator=(memory_resource&& other_p) noexcept;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;

	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;

private:
	memory_resource(const memory_resource&) = delete;
	memory_resource& operator=(const memory_resource&) = delete;
};


END_NAMESPACE
#endif