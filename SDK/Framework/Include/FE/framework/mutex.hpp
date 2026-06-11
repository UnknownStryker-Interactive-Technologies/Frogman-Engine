#ifndef _FE_MUTEX_HPP_
#define _FE_MUTEX_HPP_
/*
Copyright 2025 Unknown Stryker

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
#include <FE/prerequisites.hxx>

#include <atomic>




BEGIN_NAMESPACE(FE)


class mutex
{
	std::atomic_int64_t m_front;
	std::atomic_int64_t m_back;

public:
	mutex() noexcept;
	~mutex() noexcept = default;


	mutex(const mutex&) = delete;
	mutex(mutex&&) = delete;
	mutex& operator=(const mutex&) = delete;
	mutex& operator=(mutex&&) = delete;


	void lock() noexcept;
	// Attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.
	void unlock() noexcept;
};


class shared_mutex
{
    std::atomic_int64_t m_front;
    std::atomic_int64_t m_back;
    std::atomic_int64_t m_shared_count;

public:
	shared_mutex() noexcept;
	~shared_mutex() noexcept = default;


    shared_mutex(const shared_mutex&) = delete;
    shared_mutex(shared_mutex&&) = delete;
    shared_mutex& operator=(const shared_mutex&) = delete;
    shared_mutex& operator=(shared_mutex&&) = delete;


    void lock() noexcept;
    // Attempting to unlock a shared_mutex that is not locked by the current thread will result in an undefined behavior.
    void unlock() noexcept;
    void lock_shared() noexcept;
    // Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.
	void unlock_shared() noexcept;
};


template <class Lock>
class scoped_lock
{
    Lock& m_lock;

public:
    scoped_lock(Lock& lock_p) noexcept
        : m_lock(lock_p)
    {
        m_lock.lock();
    }

    ~scoped_lock() noexcept
    {
        m_lock.unlock();
    }
};


template <class SharedLock>
class scoped_shared_lock
{
    SharedLock& m_shared_lock;

public:
    scoped_shared_lock(SharedLock& shared_lock_p) noexcept
        : m_shared_lock(shared_lock_p)
    {
        m_shared_lock.lock_shared();
    }

    ~scoped_shared_lock() noexcept
    {
        m_shared_lock.unlock_shared();
    }
};


END_NAMESPACE
#endif