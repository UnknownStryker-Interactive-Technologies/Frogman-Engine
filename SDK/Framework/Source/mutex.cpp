#include "FE/framework/mutex.hpp"
#include <FE/framework/game_processor.hxx>




FE::mutex::mutex() noexcept
    :   m_front(0),
        m_back(0)
{
}

void FE::mutex::lock() noexcept
{
	int64_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);
	while (m_front.load(std::memory_order_acquire) != l_lock_id)
	{
		FE::fiber_scheduler::yield();
	}
}

void FE::mutex::unlock() noexcept
{
	FE_ASSERT(m_front != m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");
	m_front.fetch_add(1, std::memory_order_acq_rel);
}




FE::shared_mutex::shared_mutex() noexcept
    :   m_front(0),
        m_back(0),
        m_shared_count(0)
{
}

void FE::shared_mutex::lock() noexcept
{
    int64_t l_lock_id = m_back.fetch_add(1, std::memory_order_acq_rel);
    while (m_front.load(std::memory_order_acquire) != l_lock_id)
    {
		FE::fiber_scheduler::yield();
    }

    while (m_shared_count.load(std::memory_order_acquire) != 0)
    {
        FE::fiber_scheduler::yield();
    }
}

void FE::shared_mutex::unlock() noexcept
{
    FE_ASSERT(m_front != m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");
    m_front.fetch_add(1, std::memory_order_acq_rel);
}

void FE::shared_mutex::lock_shared() noexcept
{
    while ((m_front.load(std::memory_order_acquire) xor m_back.load(std::memory_order_acquire)) != 0)
    {
        FE::fiber_scheduler::yield();
    }
    m_shared_count.fetch_add(1, std::memory_order_acq_rel);
}

void FE::shared_mutex::unlock_shared() noexcept
{
    FE_ASSERT(m_shared_count > 0, "Assertion Failed: Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.");
    m_shared_count.fetch_sub(1, std::memory_order_acq_rel);
}