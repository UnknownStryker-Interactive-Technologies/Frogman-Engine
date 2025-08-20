#ifndef _FE_CORE_MUTEX_HPP_
#define _FE_CORE_MUTEX_HPP_
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
#include <FE/prerequisites.h>
#include <atomic>
#include <thread> // std::this_thread::yield();




BEGIN_NAMESPACE(FE)


// a light-weight 4 byte mutex.
class mutex
{
    std::atomic_int16_t m_front;
    std::atomic_int16_t m_back;

public:
    constexpr mutex() noexcept
        : m_front(0), m_back(0) {}

    ~mutex() noexcept {}

    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;
    mutex& operator=(mutex&&) = delete;

    _FE_FORCE_INLINE_ void lock() noexcept
    {
        int16_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);
        while (m_front.load(std::memory_order_acquire) != l_lock_id)
        {
            std::this_thread::yield();
            //_mm_pause();
        }
    }

    _FE_FORCE_INLINE_ FE::boolean try_lock() noexcept
    {
        int16_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);
        if (m_front.load(std::memory_order_acquire) != l_lock_id)
        {
            m_back.fetch_sub(1, std::memory_order_acq_rel);
            return false;
        }
        return true;
    }

    // Attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.
    _FE_FORCE_INLINE_ void unlock() noexcept
    {
        FE_ASSERT(this->m_front != this->m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");
        m_front.fetch_add(1, std::memory_order_acq_rel);
    }
};




// a light-weight 8 byte mutex.
class shared_mutex
{
    std::atomic_int16_t m_front;
    std::atomic_int16_t m_back;
    std::atomic_int16_t m_shared_count;

public:
    constexpr shared_mutex() noexcept
        : m_front(0), m_back(0), m_shared_count(0) {}

    ~shared_mutex() noexcept {}

    _FE_FORCE_INLINE_ void lock() noexcept
    {
        while (m_shared_count.load(std::memory_order_acquire) != 0)
        {
            std::this_thread::yield();
            //_mm_pause();
        }
        int16_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);

        while (m_front.load(std::memory_order_acquire) != l_lock_id)
        {
            std::this_thread::yield();
            //_mm_pause();
        }
    }

    _FE_FORCE_INLINE_ FE::boolean try_lock() noexcept
    {
        if (m_shared_count.load(std::memory_order_acquire) != 0)
        {
            return false;
        }
        int16_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);

        if (m_front.load(std::memory_order_acquire) != l_lock_id)
        {
            m_back.fetch_sub(1, std::memory_order_acq_rel);
            return false;
        }
        return true;
    }

    // Attempting to unlock a shared_mutex that is not locked by the current thread will result in an undefined behavior.
    _FE_FORCE_INLINE_ void unlock() noexcept
    {
        FE_ASSERT(this->m_front != this->m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");
        m_front.fetch_add(1, std::memory_order_acq_rel);
    }


    _FE_FORCE_INLINE_ void lock_shared() noexcept
    {
        while ((m_front.load(std::memory_order_acquire) xor m_back.load(std::memory_order_acquire)) != 0)
        {
            std::this_thread::yield();
            //_mm_pause();
        }
        m_shared_count.fetch_add(1, std::memory_order_acq_rel);
    }

    _FE_FORCE_INLINE_ FE::boolean try_lock_shared() noexcept
    {
        if ((m_front.load(std::memory_order_acquire) xor m_back.load(std::memory_order_acquire)) != 0)
        {
            return false;
        }
        m_shared_count.fetch_add(1, std::memory_order_acq_rel);
        return true;
    }

    // Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.
    _FE_FORCE_INLINE_ void unlock_shared() noexcept
    {
        FE_ASSERT(m_shared_count > 0, "Assertion Failed: Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.");
        m_shared_count.fetch_sub(1, std::memory_order_acq_rel);
    }
};




template <class Lock>
class scoped_lock
{
    Lock& m_lock;

public:
    _FE_FORCE_INLINE_ scoped_lock(Lock& lock_p) noexcept
        : m_lock(lock_p)
    {
        this->m_lock.lock();
    }

    _FE_FORCE_INLINE_ ~scoped_lock() noexcept
    {
        this->m_lock.unlock();
    }
};




template <class SharedLock>
class scoped_shared_lock
{
    SharedLock& m_shared_lock;

public:
    _FE_FORCE_INLINE_ scoped_shared_lock(SharedLock& shared_lock_p) noexcept
        : m_shared_lock(shared_lock_p)
    {
        this->m_shared_lock.lock_shared();
    }

    _FE_FORCE_INLINE_ ~scoped_shared_lock() noexcept
    {
        this->m_shared_lock.unlock_shared();
    }
};


END_NAMESPACE
#endif