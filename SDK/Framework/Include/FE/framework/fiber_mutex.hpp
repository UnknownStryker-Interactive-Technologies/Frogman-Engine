#ifndef _FE_FRAMEWORK_FIBER_MUTEX_HPP_
#define _FE_FRAMEWORK_FIBER_MUTEX_HPP_
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

#include <atomic>




BEGIN_NAMESPACE(FE)

class fiber_scheduler;

class fiber_mutex
{
	std::atomic_int16_t m_front;
	std::atomic_int16_t m_back;
    std::atomic_int16_t m_owner_id;
	std::atomic<class FE::fiber_scheduler*> m_owner_fiber_scheduler;

public:
    fiber_mutex() noexcept;
	~fiber_mutex() noexcept = default;


	fiber_mutex(const fiber_mutex&) = delete;
	fiber_mutex(fiber_mutex&&) = delete;
	fiber_mutex& operator=(const fiber_mutex&) = delete;
	fiber_mutex& operator=(fiber_mutex&&) = delete;


	void lock() noexcept;
	// Attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.
	void unlock() noexcept;
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