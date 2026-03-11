#ifndef _FE_CORE_POOL_MEMORY_RESOURCE_HXX_
#define _FE_CORE_POOL_MEMORY_RESOURCE_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/memory.hxx>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>

#include <memory_resource>




BEGIN_NAMESPACE(FE)


constexpr FE::size xmmword_size = 16;
constexpr FE::size ymmword_size = 32;
constexpr FE::size zmmword_size = 64;
constexpr FE::size dzmmword_size = 128;


namespace internal
{
	enum struct AllocatorType
	{
		_XMMWordAllocator,
		_YMMWordAllocator,
		_ZMMWordAllocator,
		_DZMMWordAllocator,
		_ScalableAllocator,
		_AlignedMalloc
	};

	AllocatorType __select_allocator(std::size_t bytes_p) noexcept;
}


/*
The FE::memory_resource is a class template provides a memory resource that utilizes a bunch of pool allocators for efficient memory management
inheriting from std::pmr::memory_resource.
*/
class memory_resource : public std::pmr::memory_resource
{
public:
	using xmmword_pool_type = FE::block_pool<xmmword_size, FE::align_16bytes>;
	using ymmword_pool_type = FE::block_pool<ymmword_size, FE::align_32bytes>;
	using zmmword_pool_type = FE::block_pool<zmmword_size, FE::align_64bytes>;
	using dzmmword_pool_type = FE::block_pool<dzmmword_size, FE::align_128bytes>;
	using scalable_pool_type = FE::scalable_pool<FE::SIMD_auto_alignment>;

private:
	xmmword_pool_type m_xmmword_block_pool;
	ymmword_pool_type m_ymmword_block_pool;
	zmmword_pool_type m_zmmword_block_pool;
	dzmmword_pool_type m_dzmmword_block_pool;
	scalable_pool_type m_scalable_pool;

public:
	memory_resource() noexcept = default;
	virtual ~memory_resource() noexcept = default;

	memory_resource(memory_resource&& other_p) noexcept;
	memory_resource& operator=(memory_resource&& other_p) noexcept;

	memory_resource(const memory_resource&) = delete;
	memory_resource& operator=(const memory_resource&) = delete;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;

	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;
};


END_NAMESPACE
#endif