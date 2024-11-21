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
#include <FE/private/allocator_base.hpp>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>

#include <memory_resource>




BEGIN_NAMESPACE(FE)


class scalable_pool_resource : public std::pmr::memory_resource, public FE::internal::allocator_base
{
public:
	using pool_type = FE::pool<FE::PoolType::_Scalable, FE::SIMD_auto_alignment>;

	pool_type m_pool;

public:
	scalable_pool_resource() noexcept = default;
	virtual ~scalable_pool_resource() noexcept = default;

	scalable_pool_resource(scalable_pool_resource&& other_p) noexcept;
	scalable_pool_resource& operator=(scalable_pool_resource&& other_p) noexcept;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;

private:
	scalable_pool_resource(const scalable_pool_resource&) = delete;
	scalable_pool_resource& operator=(const scalable_pool_resource&) = delete;
};

// thread-unsafe!
class cache_aligned_pool_resource : public std::pmr::memory_resource, public FE::internal::allocator_base
{
public:
	using pool_type = FE::pool<FE::PoolType::_Scalable, FE::align_CPU_L1_cache_line>;

	pool_type m_pool;

public:
	cache_aligned_pool_resource() noexcept = default;
	virtual ~cache_aligned_pool_resource() noexcept = default;

	cache_aligned_pool_resource(cache_aligned_pool_resource&& other_p) noexcept;
	cache_aligned_pool_resource& operator=(cache_aligned_pool_resource&& other_p) noexcept;

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept override;
	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;

private:
	cache_aligned_pool_resource(const cache_aligned_pool_resource&) = delete;
	cache_aligned_pool_resource& operator=(const cache_aligned_pool_resource&) = delete;
};


END_NAMESPACE
#endif