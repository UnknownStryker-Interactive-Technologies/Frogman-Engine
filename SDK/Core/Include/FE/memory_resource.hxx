#ifndef _FE_CORE_POOL_MEMORY_RESOURCE_HXX_
#define _FE_CORE_POOL_MEMORY_RESOURCE_HXX_
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
#include <FE/memory.hxx>
#include <FE/block_pool.hxx>
#include <FE/private/arena.hxx>
#include <FE/private/scalable_pool.hxx>

#include <memory_resource>
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier




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
		_VirtualAlloc
	};
	AllocatorType _FE_VECTOR_CALL_ __select_allocator(std::size_t bytes_p) noexcept;
}


/*
The Frogman Engine Runtime Memory Resource; do not assume allocation granularity.
*/
class alignas(FE::CPU_L1_cache_line::size) memory_resource : public std::pmr::memory_resource
{
public:
	using xmmword_pool_type = FE::block_allocator<xmmword_size, FE::align_16bytes>;
	using ymmword_pool_type = FE::block_allocator<ymmword_size, FE::align_32bytes>;
	using zmmword_pool_type = FE::block_allocator<zmmword_size, FE::align_64bytes>;
	using dzmmword_pool_type = FE::block_allocator<dzmmword_size, FE::align_128bytes>;
	using scalable_pool_type = pool<PoolType::_Scalable, FE::SIMD_auto_alignment>;

	static_assert(xmmword_pool_type::page_granularity_in_bytes == 64 * FE::one_KiB);
	static_assert(ymmword_pool_type::page_granularity_in_bytes == 64 * FE::one_KiB);
	static_assert(zmmword_pool_type::page_granularity_in_bytes == 64 * FE::one_KiB);
	static_assert(dzmmword_pool_type::page_granularity_in_bytes == 64 * FE::one_KiB);
	static_assert(scalable_pool_type::page_granularity_in_bytes == 64 * FE::one_KiB);
	constexpr static FE::int32 page_granularity_in_bytes = scalable_pool_type::page_granularity_in_bytes;

private:
	xmmword_pool_type m_xmmword_block_pool;
	ymmword_pool_type m_ymmword_block_pool;
	zmmword_pool_type m_zmmword_block_pool;
	dzmmword_pool_type m_dzmmword_block_pool;
	scalable_pool_type m_scalable_pool;

	page_aligned_allocator<std::byte> m_fallback_allocator;

public:
	memory_resource() noexcept;
	virtual ~memory_resource() noexcept override;

	memory_resource(memory_resource&& other_p) noexcept;
	memory_resource& operator=(memory_resource&& other_p) noexcept;

	memory_resource(const memory_resource&) = delete;
	memory_resource& operator=(const memory_resource&) = delete;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void  do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;

	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;
};


namespace large
{
	namespace internal
	{
		FE::internal::AllocatorType _FE_VECTOR_CALL_ __select_allocator(std::size_t bytes_p) noexcept;
	}
	/*
	The Frogman Engine Runtime Large Memory Resource; do not assume allocation granularity.
	*/
	class alignas(FE::CPU_L1_cache_line::size) memory_resource : public std::pmr::memory_resource
	{
	public:
		using xmmword_pool_type = FE::large::block_allocator<xmmword_size, FE::align_16bytes>;
		using ymmword_pool_type = FE::large::block_allocator<ymmword_size, FE::align_32bytes>;
		using zmmword_pool_type = FE::large::block_allocator<zmmword_size, FE::align_64bytes>;
		using dzmmword_pool_type = FE::large::block_allocator<dzmmword_size, FE::align_128bytes>;
		using scalable_pool_type = pool<PoolType::_ScalableLargePage, FE::SIMD_auto_alignment>;
		//using super_large_area_type = pool<PoolType::_SuperLargeArea, FE::SIMD_auto_alignment>;

		static_assert(xmmword_pool_type::page_granularity_in_bytes == 2 * FE::one_MiB);
		static_assert(ymmword_pool_type::page_granularity_in_bytes == 2 * FE::one_MiB);
		static_assert(zmmword_pool_type::page_granularity_in_bytes == 2 * FE::one_MiB);
		static_assert(dzmmword_pool_type::page_granularity_in_bytes == 2 * FE::one_MiB);
		static_assert(scalable_pool_type::page_granularity_in_bytes == 2 * FE::one_MiB);
		//static_assert(super_large_area_type::page_granularity_in_bytes == 1 * FE::one_GiB);
		constexpr static FE::int32 page_granularity_in_bytes = scalable_pool_type::page_granularity_in_bytes;
		//constexpr static FE::int32 super_large_page_granularity_in_bytes = super_large_area_type::page_granularity_in_bytes;

	private:
		xmmword_pool_type m_xmmword_block_pool;
		ymmword_pool_type m_ymmword_block_pool;
		zmmword_pool_type m_zmmword_block_pool;
		dzmmword_pool_type m_dzmmword_block_pool;
		scalable_pool_type m_scalable_pool;

		//super_large_area_type m_super_large_area;

		page_aligned_allocator<std::byte> m_fallback_allocator;

	public:
		memory_resource() noexcept;
		virtual ~memory_resource() noexcept override;

		memory_resource(memory_resource&& other_p) noexcept;
		memory_resource& operator=(memory_resource&& other_p) noexcept;

		memory_resource(const memory_resource&) = delete;
		memory_resource& operator=(const memory_resource&) = delete;

	protected:
		virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
		virtual void  do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;

		virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;
	};
}


END_NAMESPACE
#pragma warning(pop)
#endif