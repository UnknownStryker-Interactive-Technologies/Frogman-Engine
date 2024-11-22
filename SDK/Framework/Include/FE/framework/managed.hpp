#ifndef _FE_FRAMEWORK_MANAGED_HPP_
#define _FE_FRAMEWORK_MANAGED_HPP_
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
#include <FE/pool/block_pool.hxx>

#include <atomic>

// std::unique_ptr
#include <memory>

// std::pmr::polymorphic_allocator
#include <memory_resource>




BEGIN_NAMESPACE(FE::framework)


FE::uint8 get_current_thread_id() noexcept;

class framework_base;

template<typename T, class Allocator>
class unique_ptr;

template<typename T>
class weak_ptr;


template<typename T>
struct ref_block
{
	std::atomic<T*> _reference = nullptr;
	std::atomic<var::int64> _ref_count = 0;
};


class managed
{
	friend class framework_base;

	template<typename T, class Allocator>
	friend class unique_ptr;

	template<typename T>
	friend class weak_ptr;

public:
	managed() noexcept = default;
	virtual ~managed() noexcept = default;

protected:
	template<typename T>
	static ref_block<T>* allocate_ref_block() noexcept
	{
		return managed::s_thread_local_ref_tables[get_current_thread_id()].allocate<ref_block<T>>();
	}

	template<typename T>
	static void deallocate_ref_block(ref_block<T>* block_p) noexcept
	{
		return managed::s_thread_local_ref_tables[get_current_thread_id()].deallocate<ref_block<T>>(block_p);
	}

private:
	constexpr static FE::size ref_block_size = sizeof(ref_block<void>);
	static std::unique_ptr<FE::block_pool<ref_block_size, FE::SIMD_auto_alignment>[]> s_thread_local_ref_tables;

	static void initialize(var::uint32 max_concurrency_p) noexcept;
	static void shutdown() noexcept;
};


template<typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
class unique_ptr
{
	template<typename T>
	friend class weak_ptr;

public:
	using pointer = T*;
	using element_type = T;
	using allocator_type = Allocator;
	
private:
	ref_block<T>* m_ptr;
	_FE_NO_UNIQUE_ADDRESS_ Allocator m_allocator;

public:
	template<typename... Arguments>
	unique_ptr(const Allocator& allocator_p = Allocator(), Arguments&&... arguments) noexcept
		: m_ptr(), m_allocator(allocator_p)
	{
		this->m_ptr = managed::allocate_ref_block<T>();

		this->m_ptr->_reference.store(this->m_allocator.allocate(1), std::memory_order_relaxed);
		this->m_ptr->_ref_count.store(0, std::memory_order_relaxed);

		new( this->m_ptr->_reference.load(std::memory_order_relaxed) ) T(std::forward<Arguments>(arguments)...);
	}

	unique_ptr(const unique_ptr&) = delete;

	unique_ptr(unique_ptr&& other_p) noexcept : m_ptr(other_p.m_ptr), m_allocator(other_p.m_allocator)
	{
		other_p.m_ptr = nullptr;
	}

	~unique_ptr() noexcept
	{
		if (this->m_ptr == nullptr)
		{
			return;
		}

		T* l_ptr = this->m_ptr->_reference.exchange(nullptr, std::memory_order_seq_cst); // potential read after delete
		if (l_ptr == nullptr)
		{
			l_ptr->~T();
			this->m_allocator.deallocate(l_ptr, 1);
		}

		if (this->m_ptr->_ref_count.load(std::memory_order_acquire) <= 0)
		{
			managed::deallocate_ref_block(this->m_ptr);
		}
	}

	unique_ptr& operator=(const T& value_p) noexcept
	{
		if (this->m_ptr == nullptr)
		{
			this->m_ptr = managed::allocate_ref_block<T>();
		}

		if (this->m_ptr->_reference.load(std::memory_order_relaxed) == nullptr)
		{
			this->m_ptr->_reference.store(this->m_allocator.allocate(1), std::memory_order_release);
			new( this->m_ptr->_reference.load(std::memory_order_relaxed) ) T(value_p);
		}
		*(this->m_ptr->_reference.load(std::memory_order_relaxed)) = value_p;
		return *this;
	}

	unique_ptr& operator=(const unique_ptr&) = delete;

	unique_ptr& operator=(unique_ptr&& other_p) noexcept
	{
		this->m_ptr = other_p.m_ptr;
		this->m_allocator = other_p.m_allocator;
		other_p.m_ptr = nullptr;
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T* operator->() const noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return this->m_ptr->_reference.load(std::memory_order_relaxed);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T* operator->() noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return this->m_ptr->_reference.load(std::memory_order_relaxed);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T& operator*() const noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		T* l_ptr = this->m_ptr->_reference.load(std::memory_order_relaxed);
		FE_ASSERT(l_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return *l_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T& operator*() noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		T* l_ptr = this->m_ptr->_reference.load(std::memory_order_relaxed);
		FE_ASSERT(l_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return *l_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ explicit operator bool() const noexcept
	{
		return this->m_ptr != nullptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator==(const unique_ptr& other_p) const noexcept
	{
		return this->m_ptr == other_p.m_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator!=(const unique_ptr& other_p) const noexcept
	{
		return this->m_ptr != other_p.m_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator==(std::nullptr_t) const noexcept
	{
		return this->m_ptr == nullptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator!=(std::nullptr_t) const noexcept
	{
		return this->m_ptr != nullptr;
	}
};

class super_object_base;

template<typename T>
class weak_ptr
{
	friend class super_object_base;
public:
	using element_type = T;

private:
	ref_block<T>* m_ptr;

public:
	weak_ptr() noexcept : m_ptr() {}

	weak_ptr(const weak_ptr& other_p) noexcept : m_ptr(other_p.m_ptr) 
	{
		if (other_p.m_ptr == nullptr)
		{
			return;
		}

		other_p.m_ptr->_ref_count.fetch_add(1, std::memory_order_seq_cst);
	}
	
	weak_ptr(weak_ptr&& other_p) noexcept : m_ptr(other_p.m_ptr)
	{
		other_p.m_ptr = nullptr;
	}

	template<class Allocator>
	weak_ptr(const unique_ptr<T, Allocator>& unique_ptr_p) noexcept : m_ptr(unique_ptr_p.m_ptr)
	{
		if (unique_ptr_p.m_ptr == nullptr)
		{
			return;
		}

		unique_ptr_p.m_ptr->_ref_count.fetch_add(1, std::memory_order_seq_cst);
	}

	~weak_ptr() noexcept
	{
		if (this->m_ptr == nullptr)
		{
			return;
		}

		FE::int64 l_ref_count = this->m_ptr->_ref_count.fetch_sub(1, std::memory_order_seq_cst);
		if (l_ref_count <= 0)
		{
			if (this->m_ptr->_reference.load(std::memory_order_acquire) == nullptr)
			{
				managed::deallocate_ref_block(this->m_ptr);
			}
		}
	}
	
	weak_ptr& operator=(const weak_ptr& other_p) noexcept
	{
		if (other_p.m_ptr == nullptr)
		{
			return *this;
		}

		this->m_ptr = other_p.m_ptr;
		other_p.m_ptr->_ref_count.fetch_add(1, std::memory_order_seq_cst);
		return *this;
	}

	weak_ptr& operator=(weak_ptr&& other_p) noexcept
	{
		this->m_ptr = other_p.m_ptr;
		other_p.m_ptr = nullptr;
		return *this;
	}

	template<class Allocator>
	weak_ptr& operator=(const unique_ptr<T, Allocator>& unique_ptr_p) noexcept
	{
		if (unique_ptr_p.m_ptr == nullptr)
		{
			return *this;
		}

		this->m_ptr = unique_ptr_p.m_ptr;
		unique_ptr_p.m_ptr->_ref_count.fetch_add(1, std::memory_order_seq_cst);
		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T* operator->() const noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return this->m_ptr->_reference.load(std::memory_order_acquire);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T* operator->() noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return this->m_ptr->_reference.load(std::memory_order_acquire);
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T& operator*() const noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		T* l_ptr = this->m_ptr->_reference.load(std::memory_order_acquire);
		FE_ASSERT(l_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return *l_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ T& operator*() noexcept
	{
		FE_ASSERT(this->m_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		T* l_ptr = this->m_ptr->_reference.load(std::memory_order_acquire);
		FE_ASSERT(l_ptr != nullptr, "Assertion Failed: the smart pointer is a nullptr.");
		return *l_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ explicit operator bool() const noexcept
	{
		return this->m_ptr != nullptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator==(const weak_ptr& other_p) const noexcept
	{
		return this->m_ptr == other_p.m_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator!=(const weak_ptr& other_p) const noexcept
	{
		return this->m_ptr != other_p.m_ptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator==(std::nullptr_t) const noexcept
	{
		return this->m_ptr == nullptr;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR23_ bool operator!=(std::nullptr_t) const noexcept
	{
		return this->m_ptr != nullptr;
	}
};

END_NAMESPACE
#endif