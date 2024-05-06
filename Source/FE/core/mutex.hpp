#ifndef _FE_CORE_MUTEX_HPP_
#define _FE_CORE_MUTEX_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include "prerequisite_symbols.h"
#include <FE/core/memory.hxx>

#ifdef _WINDOWS_64BIT_OS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define _MUTEX_TYPE_ HANDLE
#undef WIN32_LEAN_AND_MEAN

#elif defined(_LINUX_64BIT_OS_)
#include <pthread.h>
#define _MUTEX_TYPE_ pthread_mutex_t
#endif




BEGIN_NAMESPACE(FE)


/*
A Mutual Exclusion Object Class
On Windows 64bit Platform: size of a mutex object is sixteen bytes.
On Linux(Ubuntu) 64bit Platform: size of a mutex instance is forty-eight bytes.
*/
#pragma pack(push, _DWORD_SIZE_)
class mutex final
{
private:
    ::std::atomic_bool m_is_locked;
    _MUTEX_TYPE_ m_mutex;

public:
    mutex() noexcept;

    ~mutex() noexcept;


    mutex(const mutex& other_ref_p) noexcept = delete;


    mutex& operator=(const mutex& other_ref_p) noexcept = delete;


    void lock() noexcept;
    void unlock() noexcept;
    var::boolean try_lock() noexcept;
    var::boolean is_locked() noexcept;
};


/*
A Timed Mutual Exclusion Object Class
On Windows 64bit Platform: size of a mutex object is sixteen bytes.
On Linux(Ubuntu) 64bit Platform: size of a mutex instance is forty-eight bytes.
*/
class timed_mutex final
{
private:
    ::std::atomic_bool m_is_locked;
    _MUTEX_TYPE_ m_timed_mutex;
    
public:
    timed_mutex() noexcept;

    ~timed_mutex() noexcept;


    timed_mutex(const timed_mutex& other_ref_p) noexcept = delete;


    timed_mutex & operator=(const timed_mutex& other_ref_p) noexcept = delete;


    void lock(uint32 milliseconds_p) noexcept;
    void unlock() noexcept;
    var::boolean try_lock(uint32 milliseconds_p) noexcept;
    var::boolean is_locked() noexcept;
};
#pragma pack(pop, _DWORD_SIZE_)


template <class lock>
class lock_guard
{
private:
    lock& m_lock_ref;

public:
    typedef lock lock_type;

    lock_guard(lock& lock_ref_p) noexcept : m_lock_ref(lock_ref_p) { this->m_lock_ref.lock(); }
    lock_guard(lock& lock_ref_p, uint32 milliseconds_p) noexcept : m_lock_ref(lock_ref_p) { this->m_lock_ref.lock(milliseconds_p); }

    lock_guard(const lock_guard& other_ref_p) noexcept = delete;


    lock_guard(lock_guard&& move_p) noexcept = delete;


    lock_guard& operator=(const lock_guard& other_ref_p) noexcept = delete;


    lock_guard& operator=(lock_guard&& move_p) noexcept = delete;


    ~lock_guard() noexcept
    {
        if (this->m_lock_ref.is_locked() == true)
        {
            this->m_lock_ref.unlock();
        }
    }
};


END_NAMESPACE
#endif 
