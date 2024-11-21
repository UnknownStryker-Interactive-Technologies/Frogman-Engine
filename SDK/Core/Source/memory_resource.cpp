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
#include <FE/pool/memory_resource.hpp>




BEGIN_NAMESPACE(FE)

scalable_pool_resource::scalable_pool_resource(scalable_pool_resource&& other_p) noexcept : m_pool(std::move(other_p.m_pool)) {}

scalable_pool_resource& scalable_pool_resource::operator=(scalable_pool_resource&& other_p) noexcept
{
	this->m_pool = std::move(other_p.m_pool);
	return *this;
}

void* scalable_pool_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	return this->m_pool.allocate<std::byte>(bytes_p);
}

void scalable_pool_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	this->m_pool.deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
}

bool scalable_pool_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
{
	const scalable_pool_resource* l_other = dynamic_cast<const scalable_pool_resource*>(&other_p);
	if (l_other == nullptr)
	{
		return false;
	}

	return &(this->m_pool) == &(l_other->m_pool);
}




cache_aligned_pool_resource::cache_aligned_pool_resource(cache_aligned_pool_resource&& other_p) noexcept : m_pool(std::move(other_p.m_pool)) {}

cache_aligned_pool_resource& cache_aligned_pool_resource::operator=(cache_aligned_pool_resource&& other_p) noexcept
{
	this->m_pool = std::move(other_p.m_pool);
	return *this;
}

void* cache_aligned_pool_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	return this->m_pool.allocate<std::byte>(bytes_p);
}

void cache_aligned_pool_resource::do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	this->m_pool.deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
}

bool cache_aligned_pool_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
{
	const cache_aligned_pool_resource* l_other = dynamic_cast<const cache_aligned_pool_resource*>(&other_p);
	if (l_other == nullptr)
	{
		return false;
	}

	return &(this->m_pool) == &(l_other->m_pool);
}


END_NAMESPACE