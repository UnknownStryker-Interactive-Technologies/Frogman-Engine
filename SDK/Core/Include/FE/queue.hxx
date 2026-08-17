#ifndef _FE_CORE_QUEUE_HXX_
#define _FE_CORE_QUEUE_HXX_
/*
Copyright 2025 by UNKNOWN STRYKER (Hojin Lee / Joey)

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
#include <FE/fqueue.hxx>
#include <FE/list.hxx>

#include <memory>
#include <memory_resource>
#include <utility>




BEGIN_NAMESPACE(FE)

// eXceptionless assertive queue
template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
class queue
{
	static_assert(std::is_same_v<T, typename std::allocator_traits<Allocator>::value_type>, "Allocator type must match the queue value type.");
	static constexpr FE::size mini_stack_capacity = ((4 * FE::one_KiB) / sizeof(T)) - (16 / sizeof(T));
	static_assert(mini_stack_capacity > 0, "Mini queue capacity must be greater than zero.");
public:
	using value_type = typename std::allocator_traits<Allocator>::value_type;
	using size_type = typename std::allocator_traits<Allocator>::size_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc< FE::fqueue<T, mini_stack_capacity> >;
	
private:
	using container_type = FE::list< FE::fqueue<T, mini_stack_capacity>, allocator_type >;
	container_type m_mini_queues;
	typename container_type::iterator m_front_slab;
	typename container_type::iterator m_back_slab;
	// global size of the queue
	size_type m_size;

public:
	queue() noexcept
		:	m_mini_queues(),
			m_front_slab(),
			m_back_slab(),
			m_size()
	{
		m_mini_queues.emplace_back();
		m_front_slab = m_mini_queues.begin();
		m_back_slab = m_mini_queues.begin();
	}

	explicit queue(const allocator_type& allocator_p) noexcept
		:	m_mini_queues(allocator_p),
			m_front_slab(),
			m_back_slab(),
			m_size()
	{
		m_mini_queues.emplace_back();
		m_front_slab = m_mini_queues.begin();
		m_back_slab = m_mini_queues.begin();
	}

	queue(const queue& other_p) noexcept
		:	m_mini_queues(other_p.m_mini_queues),
			m_front_slab(),
			m_back_slab(),
			m_size(other_p.m_size)
	{
		auto l_off = std::distance<typename container_type::const_iterator>(other_p.m_mini_queues.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_front_slab));
		m_front_slab = std::next(m_mini_queues.begin(), l_off);

		l_off = std::distance<typename container_type::const_iterator>(other_p.m_mini_queues.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_back_slab));
		m_back_slab = std::next(m_mini_queues.begin(), l_off);
	}

	queue(queue&& other_p) noexcept
		:	m_mini_queues(std::move(other_p.m_mini_queues)),
			m_front_slab(other_p.m_front_slab),
			m_back_slab(other_p.m_back_slab),
			m_size(other_p.m_size)
	{
		other_p.m_mini_queues.emplace_back();
		other_p.m_front_slab = other_p.m_mini_queues.begin();
		other_p.m_back_slab = other_p.m_mini_queues.begin();
		other_p.m_size = 0;
	}


	~queue() noexcept
	{}


	queue& operator=(const queue& other_p) noexcept
	{
		if (this == &other_p)
		{
			return *this;
		}

		m_mini_queues = other_p.m_mini_queues;

		auto l_off = std::distance(other_p.m_mini_queues.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_front_slab));
		m_front_slab = std::next(m_mini_queues.begin(), l_off);

		l_off = std::distance(other_p.m_mini_queues.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_back_slab));
		m_back_slab = std::next(m_mini_queues.begin(), l_off);

		m_size = other_p.m_size;
		return *this;
	}
	
	queue& operator=(queue&& other_p) noexcept
	{
		if (this == &other_p)
		{
			return *this;
		}

		m_mini_queues = std::move(other_p.m_mini_queues);
		m_front_slab = other_p.m_front_slab;
		m_back_slab = other_p.m_back_slab;
		m_size = other_p.m_size;

		other_p.m_mini_queues.emplace_back();
		other_p.m_front_slab = other_p.m_mini_queues.begin();
		other_p.m_back_slab = other_p.m_mini_queues.begin();
		other_p.m_size = 0;
		return *this;
	}


	reference front() noexcept
	{
		return m_front_slab->front();
	}

	const_reference front() const noexcept
	{
		return m_front_slab->front();
	}


	reference back() noexcept
	{
		return m_back_slab->back();
	}

	const_reference back() const noexcept
	{
		return m_back_slab->back();
	}


	FE::boolean is_empty() const noexcept
	{
		return m_size == 0;
	}


	size_type size() const noexcept
	{
		return m_size;
	}


	template<typename... Arguments >
	void emplace(Arguments&&... arguments_p) noexcept
	{
		if (m_back_slab->size() == mini_stack_capacity)
		{
			typename container_type::iterator l_next_slab = std::next(m_back_slab);
			if (l_next_slab == m_mini_queues.end())
			{
				l_next_slab = m_mini_queues.begin();
			}

			if ((l_next_slab == m_front_slab) 
				&& (m_front_slab->is_empty() == false))
			{
				m_back_slab = m_mini_queues.emplace(FE::iterator_cast<typename container_type::const_iterator>(m_front_slab));
			}
			else
			{
				m_back_slab = l_next_slab;
			}
		}

		++m_size;
		m_back_slab->emplace( std::forward<Arguments>(arguments_p)... );
	}


	void push(const value_type& value_p) noexcept
	{
		if (m_back_slab->size() == mini_stack_capacity)
		{
			typename container_type::iterator l_next_slab = std::next(m_back_slab);
			if (l_next_slab == m_mini_queues.end())
			{
				l_next_slab = m_mini_queues.begin();
			}

			if ((l_next_slab == m_front_slab)
				&& (m_front_slab->is_empty() == false))
			{
				m_back_slab = m_mini_queues.emplace(FE::iterator_cast<typename container_type::const_iterator>(m_front_slab));
			}
			else
			{
				m_back_slab = l_next_slab;
			}
		}

		++m_size;
		m_back_slab->push(value_p);
	}


	void pop() noexcept
	{
		FE_ASSERT(m_size != 0, "Queue underflow: Attempted to pop from an empty queue.");

		--m_size;
		m_front_slab->pop();

		if (m_front_slab->is_empty() == true && m_front_slab != m_back_slab)
		{
			++m_front_slab;

			if (m_front_slab == m_mini_queues.end())
			{
				m_front_slab = m_mini_queues.begin();
			}
		}
	}
};


END_NAMESPACE

namespace xtl
{
	// eXceptionless assertive doubly linked list
	template<class T, class Allocator = std::pmr::polymorphic_allocator<T>>
	using queue = FE::queue<T, Allocator>;
}

#endif