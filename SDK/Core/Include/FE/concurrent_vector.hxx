#ifndef _FE_CORE_CONCURRENT_VECTOR_HXX_
#define _FE_CORE_CONCURRENT_VECTOR_HXX_
/* https://oneapi-spec.uxlfoundation.org/specifications/oneapi/v1.3-rev-1/elements/onetbb/source/containers/concurrent_vector_cls
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
#include <limits>
#include <mutex>
#include <shared_mutex>

#include <boost/thread/shared_lock_guard.hpp>




BEGIN_NAMESPACE(FE)


template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>, class SharedMutex = std::shared_mutex>
class concurrent_vector
{
public:
    using allocator_type = Allocator;
    using value_type = typename std::allocator_traits<allocator_type>::value_type;

    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using size_type = typename std::allocator_traits<allocator_type>::size_type;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

private:
    static constexpr size_t default_size = 16;

private:
    std::atomic<pointer> m_active;
    std::atomic<pointer> m_reserved;
    std::atomic<size_type> m_size;
    std::atomic<size_type> m_capacity;
    SharedMutex m_length_modifier_lock;
    Allocator m_allocator;

public:
    // Constructors and destructors are concurrently unsafe.
    constexpr concurrent_vector() noexcept
        : m_active(), m_reserved(),
        m_size(), m_capacity(),
        m_length_modifier_lock(),
        m_allocator()
    {
        __allocate_default_sized_raw_on_construction();
    }

    constexpr concurrent_vector(const allocator_type& allocator_p) noexcept
        : m_active(), m_reserved(),
        m_size(), m_capacity(),
        m_length_modifier_lock(),
        m_allocator(allocator_p)
    {
        __allocate_default_sized_raw_on_construction();
    }

    concurrent_vector(std::initializer_list<value_type> init_p,
        const allocator_type& allocator_p = allocator_type()) noexcept
        : m_active(), m_reserved(),
        m_size(), m_capacity(),
        m_length_modifier_lock(),
        m_allocator(allocator_p)
    {
        if (init_p.size() == 0)
        {
            __allocate_default_sized_raw_on_construction();
            return;
        }
        try_reserve(init_p.size());
        __move_construct_from_initializer_list(init_p);
    }

    ~concurrent_vector()
    {
        pointer l_begin = m_active.load(std::memory_order_acquire);
        pointer l_end = l_begin + size();

        for (pointer it = l_begin; it < l_end; ++it)
        {
            it->~T();
        }

        m_allocator.deallocate(l_begin, m_capacity.load(std::memory_order_acquire));
    }

private: // Concurrently unsafe methods
    inline constexpr void __allocate_default_sized_raw_on_construction() noexcept
    {
        m_active.store(m_allocator.allocate(default_size), std::memory_order_relaxed);
        FE_ASSERT(m_active.load(std::memory_order_relaxed) != nullptr);

        m_capacity.store(default_size, std::memory_order_relaxed);
        FE_ASSERT(m_capacity.load(std::memory_order_relaxed) == default_size);
    }

    inline void __move_construct_from_initializer_list(std::initializer_list<value_type>& init_p) noexcept
    {
        FE_ASSERT(init_p.size() <= m_capacity.load(std::memory_order_relaxed));

        pointer l_list_iterator = const_cast<pointer>(init_p.begin());
        const_pointer l_list_end = init_p.end();
        pointer l_current_array_it = m_active.load(std::memory_order_relaxed);

        while (l_list_iterator < l_list_end)
        {
            new(l_current_array_it) T(std::move(*l_list_iterator));
            ++l_list_iterator;
            ++l_current_array_it;
        }
    }

public: // Concurrently safe methods
    // use cas to acquire the dictatorship over the pointer.
    /*
        concurrent_vector( concurrent_vector&& other_p ) noexcept
        {

        }
        concurrent_vector& operator=( concurrent_vector&& other_p ) noexcept
        {
            return *this;
        }

        concurrent_vector( const concurrent_vector& other_p ) noexcept
        {

        }
        concurrent_vector& operator=( const concurrent_vector& other_p ) noexcept
        {
            return *this;
        }

        concurrent_vector& operator=( std::initializer_list<value_type> init_p ) noexcept
        {
            return *this;
        }
        */

public:
    inline void read_at(size_type index_p, reference out_dest_p) noexcept
    {
        boost::shared_lock_guard<SharedMutex> l_lock(m_length_modifier_lock);
        out_dest_p = *(m_active.load(std::memory_order_acquire) + index_p);
    }

    inline void write_at(size_type index_p, const_reference value_p) noexcept
    {
        boost::shared_lock_guard<SharedMutex> l_lock(m_length_modifier_lock);
        *(m_active.load(std::memory_order_acquire) + index_p) = value_p;
    }

    inline void front(reference out_dest_p) noexcept
    {
        read_at(0, out_dest_p);
    }

    inline void front(const_reference value_p) noexcept
    {
        write_at(0, value_p);
    }

    inline void back(reference out_dest_p) noexcept
    {
        read_at(size() - 1, out_dest_p);
    }

    inline void back(const_reference value_p) noexcept
    {
        write_at(size() - 1, value_p);
    }

public:
    size_type try_push_back(const value_type& value_p) noexcept
    {
        boost::shared_lock_guard<SharedMutex> l_lock(m_length_modifier_lock);
        size_type l_idx = m_size.fetch_add(1, std::memory_order_acq_rel);
        size_type l_current_capacity = m_capacity.load(std::memory_order_acquire);
        /*
        I have spent days to figure out why the program arbitrarily crashes when try_reserve() is called concurrently.
        m_size has to be less than or equal to m_capacity. However, m_size sometimes becomes m_size+1.😭
        To explain the unpleasant situation:

        Step one: a thread comes here and fails to try_push_back() and sets m_size to the current capacity
        because the given index points outside of the array range [the array ranges from 0 to the current capacity].  Note that m_size is always equal to the given index + 1 at the moment of 'size_type l_idx = m_size.fetch_add(1, std::memory_order_acq_rel);'.

        Step two: another thread enters try_push_back() and fails too, and post-increments one to m_size.

        Step three: the first thread successfully acquries a chance to operate try_reserve(), and it reads the post-incremented value, thus m_size > m_capacity sometimes becomes true.

        Step four: segfault.
        */
        if (l_idx >= l_current_capacity)
        {
            //std::cerr << m_size.load(std::memory_order_acquire) << '\n';
            /*
            Atomic writes are expansive than the reads, because only a single thread can write to a cachline at a time.
            The performance characteristics of atmoic operations vary on the hardware architectures.
            On some hardwares, they use some kind of bus to communicate and synchronize their data.
            If mutiple threads reference the same variable on the cache, then it is true sharing.
            If multple threads reference the same address range on the cache and do not share the data, then it is false sharing.
            True sharing and false sharing are expansive; remember that avoiding resource sharing avoids synchronization costs.
            */
            m_size.store(l_current_capacity, std::memory_order_release);
            return std::numeric_limits<size_type>::max();
        }
        //assert(l_idx != m_capacity);

        // Commit the transaction.
        new (m_active.load(std::memory_order_acquire) + l_idx) T(value_p);
        return l_idx;
    }

    template <typename... Args>
    size_type try_emplace_back(Args&&... args_p) noexcept
    {
        boost::shared_lock_guard<SharedMutex> l_lock(m_length_modifier_lock);
        size_type l_idx = m_size.fetch_add(1, std::memory_order_acq_rel);
        size_type l_current_capacity = m_capacity.load(std::memory_order_acquire);
        /*
        I have spent days to figure out why the program arbitrarily crashes when try_reserve() is called concurrently.
        m_size has to be less than or equal to m_capacity. However, m_size sometimes becomes m_size+1.😭
        To explain the unpleasant situation:

        Step one: a thread comes here and fails to try_push_back() and sets m_size to the current capacity
        because the given index points outside of the array range [the array ranges from 0 to the current capacity].  Note that m_size is always equal to the given index + 1 at the moment of 'size_type l_idx = m_size.fetch_add(1, std::memory_order_acq_rel);'.

        Step two: another thread enters try_push_back() and fails too, and post-increments one to m_size.

        Step three: the first thread successfully acquries a chance to operate try_reserve(), and it reads the post-incremented value, thus m_size > m_capacity sometimes becomes true.

        Step four: segfault.
        */
        if (l_idx >= l_current_capacity)
        {
            //std::cerr << m_size.load(std::memory_order_acquire) << '\n';
            /*
            Atomic writes are expansive than the reads, because only a single thread can write to a cachline at a time.
            The performance characteristics of atmoic operations vary on the hardware architectures.
            On some hardwares, they use some kind of bus to communicate and synchronize their data.
            If mutiple threads reference the same variable on the cache, then it is true sharing.
            If multple threads reference the same address range on the cache and do not share the data, then it is false sharing.
            True sharing and false sharing are expansive; remember that avoiding resource sharing avoids synchronization costs.
            */
            m_size.store(l_current_capacity, std::memory_order_release);
            return std::numeric_limits<size_type>::max();
        }
        //assert(l_idx != m_capacity);

        // Commit the transaction.
        new (m_active.load(std::memory_order_acquire) + l_idx) T(std::forward<Args>(args_p)...);
        return l_idx;
    }

    bool try_reserve(size_type new_capacity_p) noexcept
    {
        pointer l_nullptr = nullptr;

        if (new_capacity_p <= m_capacity.load(std::memory_order_acquire))
        {
            return false;
        }

        if (m_reserved.compare_exchange_strong(l_nullptr, reinterpret_cast<pointer>(this), std::memory_order_acq_rel) == true)
        {
            m_reserved.store(m_allocator.allocate(new_capacity_p), std::memory_order_release);
            //std::cout << new_capacity_p << '\n';
            pointer l_retired = nullptr;
            size_type l_retired_size = 0;
            size_type l_retired_capacity = 0;
            {   // temporarily blocks other threads from reading and writing the array.
                std::lock_guard<SharedMutex> l_lock(m_length_modifier_lock);
                l_retired = m_active.exchange(m_reserved.load(std::memory_order_relaxed), std::memory_order_relaxed);
                l_retired_size = m_size.load(std::memory_order_relaxed);
                l_retired_capacity = m_capacity.exchange(new_capacity_p, std::memory_order_relaxed);
                __move_construct_from(l_retired, std::min(l_retired_size, l_retired_capacity) /* read the comment in the try_emplace_back() */);
            }
            __tear_down(l_retired, l_retired_size, l_retired_capacity);
            m_reserved.store(nullptr, std::memory_order_release);
            return true;
        }
        return false;
    }

    size_type push_back(const value_type& value_p) noexcept
    {
        size_type l_idx = try_push_back(value_p);
        while (l_idx == std::numeric_limits<size_type>::max())
        {
            size_type l_new_capacity = m_capacity.load(std::memory_order_acquire);
            l_new_capacity += (l_new_capacity >> 1);
            try_reserve(l_new_capacity);
            l_idx = try_push_back(value_p);
        }
        return l_idx;
    }

    template <typename... Args>
    size_type emplace_back(Args&&... args_p) noexcept
    {
        size_type l_idx = try_emplace_back(std::forward<Args>(args_p)...);
        while (l_idx == std::numeric_limits<size_type>::max())
        {
            size_type l_new_capacity = m_capacity.load(std::memory_order_acquire);
            l_new_capacity += (l_new_capacity >> 1);
            try_reserve(l_new_capacity);
            l_idx = try_emplace_back(std::forward<Args>(args_p)...);
        }
        return l_idx;
    }

private:
    inline void __move_construct_from(pointer source_p, size_type src_size_p) noexcept
    {
        //assert(source_p != nullptr);
        pointer l_current_array = m_active.load(std::memory_order_relaxed);
        //assert(l_current_array != nullptr);
        //assert(src_size_p <= m_capacity.load( std::memory_order_acquire ));

        for (size_type i = 0; i < src_size_p; ++i)
        {
            new (l_current_array + i) T(std::move(source_p[i]));
        }
    }

    inline void __tear_down(pointer target_p, size_type target_size_p, size_type target_capacity_p) noexcept
    {
        //assert(target_p != nullptr);
        //assert(target_size_p <= target_capacity_p);
        target_size_p = std::min(target_size_p, target_capacity_p);
        for (size_type i = 0; i < target_size_p; ++i)
        {
            target_p[i].~T();
        }
        m_allocator.deallocate(target_p, target_capacity_p);
    }

public: /* use cas to acquire the dictatorship over the pointer.
    bool try_resize( size_type new_size_p ) noexcept
    {
    }

    bool try_resize( size_type new_size_p, const value_type& value_p ) noexcept
    {
    }

    void shrink_to_fit() noexcept
    {

    }

    void clear() noexcept
    {

    }
    */
    // Concurrently safe operations
    inline size_type size() const noexcept
    {
        /* read the comment in the try_emplace_back() */
        return std::min(m_size.load(std::memory_order_acquire), m_capacity.load(std::memory_order_acquire));
    }

    inline bool is_empty() const noexcept
    {
        return m_size.load(std::memory_order_acquire) == 0;
    }

    inline size_type capacity() const noexcept
    {
        return m_capacity.load(std::memory_order_acquire);
    }

public: // Concurrently unsafe operations
    inline const allocator_type& get_allocator() const noexcept
    {
        return m_allocator;
    }
    inline allocator_type& get_allocator() noexcept
    {
        return m_allocator;
    }

    inline pointer begin() noexcept
    {
        return m_active.load(std::memory_order_acquire);
	}
    inline const_pointer begin() const noexcept
    {
        return m_active.load(std::memory_order_acquire);
    }

    inline pointer end() noexcept
    {
        return m_active.load(std::memory_order_acquire) + size();
    }
    inline const_pointer end() const noexcept
    {
        return m_active.load(std::memory_order_acquire) + size();
    }
};


END_NAMESPACE
#endif