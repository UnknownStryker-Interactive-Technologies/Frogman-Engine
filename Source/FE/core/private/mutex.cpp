#include <FE/core/mutex.hpp>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.


FE::mutex::mutex() noexcept : m_is_locked(false)
#ifdef _WINDOWS_64BIT_OS_
, m_mutex( CreateMutex(NULL, FALSE, NULL) )
#endif
{
#ifdef _LINUX_64BIT_OS_
    pthread_mutex_init(&(this->m_mutex), nullptr);
#endif
}

FE::mutex::~mutex() noexcept
{
    FE_ASSERT(this->m_is_locked.load(std::memory_order_acquire) == true, "CRITICAL ERROR: mutex is still locked");

#ifdef _WINDOWS_64BIT_OS_
    CloseHandle(this->m_mutex);
#elif defined(_LINUX_64BIT_OS_)
    pthread_mutex_destroy(&(this->m_mutex));
#endif
}

void FE::mutex::lock() noexcept
{
    this->m_is_locked.store(true, std::memory_order_release);
#ifdef _WINDOWS_64BIT_OS_
    WaitForSingleObject(this->m_mutex, INFINITE);
#elif defined(_LINUX_64BIT_OS_)
    pthread_mutex_lock(&(this->m_mutex));
#endif
}

void FE::mutex::unlock() noexcept
{
    this->m_is_locked.store(false, std::memory_order_release);
#ifdef _WINDOWS_64BIT_OS_
    ReleaseMutex(this->m_mutex);
#elif defined(_LINUX_64BIT_OS_)
    pthread_mutex_unlock(&(this->m_mutex));
#endif
}

FE::var::boolean FE::mutex::try_lock() noexcept
{
#ifdef _WINDOWS_64BIT_OS_
    if (this->m_is_locked.load(std::memory_order_acquire) == true)
    {
        return _FAILED_;
    }

    this->m_is_locked.store(true, std::memory_order_release);
    WaitForSingleObject(this->m_mutex, INFINITE);

    return _SUCCESSFUL_;

#elif defined(_LINUX_64BIT_OS_)
    this->m_is_locked.store(true, std::memory_order_release);
    return pthread_mutex_trylock(&(this->m_mutex)) == 0;
#endif
}

FE::var::boolean FE::mutex::is_locked() noexcept
{
    return this->m_is_locked.load(std::memory_order_acquire);
}




FE::timed_mutex::timed_mutex() noexcept : m_is_locked(false)
#ifdef _WINDOWS_64BIT_OS_
, m_timed_mutex(CreateMutex(NULL, FALSE, NULL))
#endif
{
#ifdef _LINUX_64BIT_OS_ 
    pthread_mutex_init(&(this->m_timed_mutex), nullptr);
#endif
}

FE::timed_mutex::~timed_mutex() noexcept
{
    FE_ASSERT(this->m_is_locked.load(std::memory_order_acquire) == true, "CRITICAL ERROR: timed mutex is still locked");

#ifdef _WINDOWS_64BIT_OS_
    CloseHandle(this->m_timed_mutex);
#elif defined(_LINUX_64BIT_OS_)
    pthread_mutex_destroy(&(this->m_timed_mutex));
#endif
}

void FE::timed_mutex::lock(uint32 milliseconds_p) noexcept
{
    ABORT_IF(milliseconds_p == 0, "ERROR: expression_string_ptrc_p is 0.");
    this->m_is_locked.store(true, std::memory_order_release);
#ifdef _WINDOWS_64BIT_OS_
    WaitForSingleObject(this->m_timed_mutex, milliseconds_p);
#elif defined(_LINUX_64BIT_OS_)
    struct timespec l_timer;
    l_timer.tv_sec = milliseconds_p / 1000;
    l_timer.tv_nsec = (milliseconds_p % 1000) * 1000000;
    pthread_mutex_timedlock(&(this->m_timed_mutex), &l_timer);
#endif
}

void FE::timed_mutex::unlock() noexcept
{
    this->m_is_locked.store(false, std::memory_order_release);
#ifdef _WINDOWS_64BIT_OS_
    ReleaseMutex(this->m_timed_mutex);
#elif defined(_LINUX_64BIT_OS_)
    pthread_mutex_unlock(&(this->m_timed_mutex));
#endif
}

FE::var::boolean FE::timed_mutex::try_lock(uint32 milliseconds_p) noexcept
{
    ABORT_IF(milliseconds_p == 0, "ERROR: milliseconds_p is 0.");
    if (this->m_is_locked.load(std::memory_order_acquire) == true)
    {
        return _FAILED_;
    }
#ifdef _WINDOWS_64BIT_OS_
    this->m_is_locked.store(true, std::memory_order_release);
    WaitForSingleObject(this->m_timed_mutex, milliseconds_p);

    return _SUCCESSFUL_;

#elif defined(_LINUX_64BIT_OS_)
    struct timespec l_timer;
    l_timer.tv_sec = milliseconds_p / 1000;
    l_timer.tv_nsec = (milliseconds_p % 1000) * 1000000;
    return pthread_mutex_timedlock(&(this->m_timed_mutex), &l_timer) == 0;
#endif
}

FE::var::boolean FE::timed_mutex::is_locked() noexcept
{
    return this->m_is_locked.load(std::memory_order_acquire);
}
