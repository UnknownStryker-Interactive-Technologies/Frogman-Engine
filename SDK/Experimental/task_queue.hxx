#ifndef _FE_CORE_TASK_QUEUE_HXX_
#define _FE_CORE_TASK_QUEUE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool.hxx>
#include <FE/core/function.hxx>
#include <FE/core/queue.hxx>
#include <FE/core/private/debug.h>

//std 
#include <any>




BEGIN_NAMESPACE(FE::experimental)


template<size Capacity>
class task_queue
{   
    // It assumes that the sizes of task types are less than or equal to 16 bytes.
    FE::block_pool<16, Capacity> m_task_pool;
    FE::fqueue<FE::task_base*, Capacity> m_task_queue;
    FE::fqueue<std::any, Capacity> m_argument_queue;

public:
    // These are unavailable since memory pools are unmovable and uncopyable.
    task_queue() noexcept = default;
    ~task_queue() noexcept = default;

    task_queue(const task_queue&) noexcept = delete;
    task_queue(task_queue&&) noexcept = delete;

    task_queue& operator=(const task_queue&) noexcept = delete;
    task_queue& operator=(task_queue&&) noexcept = delete;

    template<class Task>
    _FORCE_INLINE_ void push(const Task& task_p, const typename Task::arguments_buffer_type& arguments_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of<FE::task_base, Task>::value == false), "Static type checking failed: the template parameter Task is not the one of the children of FE::task_base.");
        FE_STATIC_ASSERT((std::is_base_of<FE::argument_base, typename Task::arguments_buffer_type>::value == false), "Static type checking failed: the template parameter Arguments is not the one of the children of FE::argument_base.");
        Task* l_task = this->m_task_pool.template allocate<Task>();
        *l_task = task_p;
        this->m_task_queue.push(l_task);
        this->m_argument_queue.push(arguments_p);
    }

    _FORCE_INLINE_ void pop() noexcept
    {
        this->m_task_pool.template deallocate<>(this->m_task_queue.pop());
        this->m_argument_queue.pop();
    }

    _FORCE_INLINE_ void pop_all() noexcept
    {
        this->m_task_queue.pop_all();
        this->m_argument_queue.pop_all();
    }

    template<typename Return = void>
    _FORCE_INLINE_ Return execute_front() noexcept
    {
        if constexpr (std::is_void<Return>::value == false)
        {
            return std::any_cast<Return>(
                this->m_task_queue.front()->operator()(
                    this->m_argument_queue.front()
                    )
                );
        }
        else if constexpr (std::is_void<Return>::value == true)
        {
            this->m_task_queue.front()->operator()(
                    this->m_argument_queue.front()
                    );
        }
    }

    template<typename Return = void>
    _FORCE_INLINE_ Return execute_back() noexcept
    {
        if constexpr (std::is_void<Return>::value == false)
        {
            return std::any_cast<Return>(
                this->m_task_queue.back()->operator()(
                    this->m_argument_queue.back()
                    )
                );
        }
        else if constexpr (std::is_void<Return>::value == true)
        {
            this->m_task_queue.back()->operator()(
                    this->m_argument_queue.back()
                    );
        }
    }

    _FORCE_INLINE_ boolean is_empty() const noexcept
	{
		return (this->m_task_queue.is_empty() == true) && (this->m_argument_queue.is_empty() == true);
	}

    typename FE::fqueue<std::any, Capacity>::const_iterator argument_queue_cbegin() noexcept { return this->m_argument_queue.cbegin(); }
    typename FE::fqueue<std::any, Capacity>::const_iterator argument_queue_cend() noexcept { return this->m_argument_queue.cend(); }

    typename FE::fqueue<FE::task_base*, Capacity>::const_iterator task_queue_cbegin() noexcept { return this->m_task_queue.cbegin(); }
    typename FE::fqueue<FE::task_base*, Capacity>::const_iterator task_queue_cend() noexcept { return this->m_task_queue.cend(); }
};

END_NAMESPACE
#endif