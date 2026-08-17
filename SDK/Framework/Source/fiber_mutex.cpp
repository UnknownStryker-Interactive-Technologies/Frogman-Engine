#include "FE/framework/fiber_mutex.hpp"
#include <FE/framework/fiber.hxx>




FE::fiber_mutex::fiber_mutex() noexcept
    :   m_front(0),
        m_back(0),
        m_owner_id(0),
	    m_owner_fiber_scheduler(nullptr)
{
}

void FE::fiber_mutex::lock() noexcept
{
	FE::int16 l_ticket = m_back.fetch_add(1, std::memory_order_acq_rel);

	while (m_front.load(std::memory_order_acquire) != l_ticket)
	{
        if (m_owner_id.load(std::memory_order_acquire) + 1 == l_ticket)
        {
			FE::fiber_scheduler* const l_current_fiber_scheduler = FE::fiber_scheduler::get_current_fiber_scheduler();
            l_current_fiber_scheduler->siglock();
            m_owner_fiber_scheduler.store(l_current_fiber_scheduler, std::memory_order_release);
        }
		FE::fiber_scheduler::yield();
	}

	m_owner_id.store(l_ticket, std::memory_order_release);
}

void FE::fiber_mutex::unlock() noexcept
{
	FE_ASSERT(m_front != m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");

    m_front.fetch_add(1, std::memory_order_acq_rel);
    FE::fiber_scheduler* const l_target_fiber_scheduler = m_owner_fiber_scheduler.load(std::memory_order_acquire);
    if (l_target_fiber_scheduler != nullptr)
    {
        l_target_fiber_scheduler->sigunlock();
    }
}


//class shared_mutex
//{
//    std::atomic_int16_t m_front;
//    std::atomic_int16_t m_back;
//    std::atomic_int16_t m_shared_count;
//
//public:
//	shared_mutex() noexcept;
//	~shared_mutex() noexcept = default;
//
//
//    shared_mutex(const shared_mutex&) = delete;
//    shared_mutex(shared_mutex&&) = delete;
//    shared_mutex& operator=(const shared_mutex&) = delete;
//    shared_mutex& operator=(shared_mutex&&) = delete;
//
//
//    void lock() noexcept;
//    // Attempting to unlock a shared_mutex that is not locked by the current thread will result in an undefined behavior.
//    void unlock() noexcept;
//    void lock_shared() noexcept;
//    // Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.
//	void unlock_shared() noexcept;
//};

//FE::shared_mutex::shared_mutex() noexcept
//    :   m_front(0),
//        m_back(0),
//        m_shared_count(0)
//{
//}
//
//void FE::shared_mutex::lock() noexcept
//{
//    FE::int16 l_ticket = m_back.fetch_add(1, std::memory_order_acq_rel);
//
//    while (m_front.load(std::memory_order_acquire) != l_ticket)
//    {
//		FE::fiber_scheduler::yield();
//    }
//
//    while (m_shared_count.load(std::memory_order_acquire) != 0)
//    {
//        FE::fiber_scheduler::yield();
//    }
//}
//
//void FE::shared_mutex::unlock() noexcept
//{
//    FE_ASSERT(m_front != m_back, "Assertion Failed: attempting to unlock a mutex that is not locked by the current thread will result in an undefined behavior.");
//    m_front.fetch_add(1, std::memory_order_acq_rel);
//}
//
//
//void FE::shared_mutex::lock_shared() noexcept
//{
//    while ((m_front.load(std::memory_order_acquire) xor m_back.load(std::memory_order_acquire)) != 0)
//    {
//        FE::fiber_scheduler::yield();
//    }
//    m_shared_count.fetch_add(1, std::memory_order_acq_rel);
//}
//
//void FE::shared_mutex::unlock_shared() noexcept
//{
//    FE_ASSERT(m_shared_count > 0, "Assertion Failed: Attempting to unlock_shared a shared_mutex that is not locked by the current thread will result in an undefined behavior.");
//    m_shared_count.fetch_sub(1, std::memory_order_acq_rel);
//}