#ifndef _FE_CORE_SYNCHRONIZED_ACCESS_HXX_
#define _FE_CORE_SYNCHRONIZED_ACCESS_HXX_
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




BEGIN_NAMESPACE(FE)


template<class Lock, typename T>
class unique_access
{
public:
	using lock_type = Lock;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

private:
	lock_type& m_lock;
	reference m_data;

public:
	_FE_FORCE_INLINE_ unique_access(lock_type& lock_p, T& data_p) noexcept : m_lock(lock_p), m_data(data_p)
	{
		this->m_lock.lock();
	}
	_FE_FORCE_INLINE_ ~unique_access() noexcept
	{
		m_lock.unlock();
	}


	unique_access(const unique_access&) = delete;
	unique_access(unique_access&&) = delete;

	unique_access& operator=(const unique_access&) = delete;
	unique_access& operator=(unique_access&&) = delete;


	_FE_FORCE_INLINE_ reference operator*() noexcept { return this->m_data; }
	_FE_FORCE_INLINE_ const_reference operator*() const noexcept { return this->m_data; }

	_FE_FORCE_INLINE_ pointer operator->() noexcept { return &(this->m_data); }
	_FE_FORCE_INLINE_ const_pointer operator->() const noexcept { return &(this->m_data); }
};


template<class SharedLock, typename T>
class shared_access
{
public:
	using lock_type = SharedLock;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

private:
	lock_type& m_lock;
	reference m_data;

public:
	_FE_FORCE_INLINE_ shared_access(lock_type& lock_p, T& data_p) noexcept : m_lock(lock_p), m_data(data_p)
	{
		this->m_lock.lock_shared();
	}
	_FE_FORCE_INLINE_ ~shared_access() noexcept
	{
		m_lock.unlock_shared();
	}


	shared_access(const shared_access&) = delete;
	shared_access(shared_access&&) = delete;

	shared_access& operator=(const shared_access&) = delete;
	shared_access& operator=(shared_access&&) = delete;


	_FE_FORCE_INLINE_ reference operator*() noexcept { return this->m_data; }
	_FE_FORCE_INLINE_ const_reference operator*() const noexcept { return this->m_data; }

	_FE_FORCE_INLINE_ pointer operator->() noexcept { return &(this->m_data); }
	_FE_FORCE_INLINE_ const_pointer operator->() const noexcept { return &(this->m_data); }
};


END_NAMESPACE
#endif