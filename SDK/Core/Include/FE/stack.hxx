#ifndef _FE_CORE_STACK_HXX_
#define _FE_CORE_STACK_HXX_
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
#include <FE/fstack.hxx>
#include <FE/list.hxx>

#include <memory>
#include <memory_resource>




BEGIN_NAMESPACE(FE)

// eXceptionless assertive stack
template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
class stack
{
	static_assert(std::is_same_v<T, typename std::allocator_traits<Allocator>::value_type>, "Allocator type must match the stack value type.");
	static constexpr FE::size mini_stack_capacity = ((4 * FE::one_KiB) / sizeof(T)) - (16 / sizeof(T));
	static_assert(mini_stack_capacity > 0, "Mini stack capacity must be greater than zero.");
public:
	using value_type = typename std::allocator_traits<Allocator>::value_type;
	using size_type = typename std::allocator_traits<Allocator>::size_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc< FE::fstack<T, mini_stack_capacity> >;

private:
	using container_type = FE::list< FE::fstack<T, mini_stack_capacity>, allocator_type >;
	container_type m_mini_stacks;
	typename container_type::iterator m_top_slab;
	// global size of the stack
	size_type m_size;

public:
	stack() noexcept
		:	m_mini_stacks(),
			m_top_slab(),
			m_size()
	{
		m_mini_stacks.emplace_back();
		m_top_slab = m_mini_stacks.begin();
	}

	explicit stack(const allocator_type& allocator_p) noexcept
		:	m_mini_stacks(allocator_p),
			m_top_slab(),
			m_size()
	{
		m_mini_stacks.emplace_back();
		m_top_slab = m_mini_stacks.begin();
	}

	stack(const stack& other_p) noexcept
		:	m_mini_stacks(other_p.m_mini_stacks),
			m_top_slab(),
			m_size(other_p.m_size)
	{
		auto l_off = std::distance<typename container_type::const_iterator>(other_p.m_mini_stacks.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_top_slab));
		m_top_slab = std::next(m_mini_stacks.begin(), l_off);
	}

	stack(stack&& other_p) noexcept
		:	m_mini_stacks(std::move(other_p.m_mini_stacks)),
			m_top_slab(other_p.m_top_slab),
			m_size(other_p.m_size)
	{
		other_p.m_mini_stacks.emplace_back();
		other_p.m_top_slab = other_p.m_mini_stacks.begin();
		other_p.m_size = 0;
	}


	~stack() noexcept
	{
	}


	stack& operator=(const stack& other_p) noexcept
	{
		if (this == &other_p)
		{
			return *this;
		}

		m_mini_stacks = other_p.m_mini_stacks;

		auto l_off = std::distance<typename container_type::const_iterator>(other_p.m_mini_stacks.cbegin(), FE::iterator_cast<typename container_type::const_iterator>(other_p.m_top_slab));
		m_top_slab = std::next(m_mini_stacks.begin(), l_off);

		m_size = other_p.m_size;
		return *this;
	}

	stack& operator=(stack&& other_p) noexcept
	{
		if (this == &other_p)
		{
			return *this;
		}

		m_mini_stacks = std::move(other_p.m_mini_stacks);
		m_top_slab = other_p.m_top_slab;
		m_size = other_p.m_size;

		other_p.m_mini_stacks.emplace_back();
		other_p.m_top_slab = other_p.m_mini_stacks.begin();
		other_p.m_size = 0;
		return *this;
	}


	reference top() noexcept
	{
		return m_top_slab->top();
	}

	const_reference top() const noexcept
	{
		return m_top_slab->top();
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
		FE_ASSERT(m_size < mini_stack_capacity, "Stack overflow: Cannot push to a full stack");

		++m_size;
		m_top_slab->emplace( std::forward<Arguments&&>(arguments_p)... );

		if (m_top_slab->size() == mini_stack_capacity)
		{
			m_mini_stacks.emplace_back();
			++m_top_slab;
		}
	}


	void push(const value_type& value_p) noexcept
	{
		FE_ASSERT(m_size < mini_stack_capacity, "Stack overflow: Cannot push to a full stack");

		++m_size;
		m_top_slab->push(value_p);

		if (m_top_slab->size() == mini_stack_capacity)
		{
			m_mini_stacks.emplace_back();
			++m_top_slab;
		}
	}


	void pop() noexcept
	{
		FE_ASSERT(m_size > 0, "Stack underflow: Cannot pop from an empty stack");

		--m_size;
		m_top_slab->pop();

		if ((m_top_slab->is_empty() == true) && (m_top_slab != m_mini_stacks.begin()))
		{
			--m_top_slab;
		}
	}
};


END_NAMESPACE

namespace xtl
{
	template <typename T, class Allocator = std::pmr::polymorphic_allocator<T>>
	using stack = ::FE::stack<T, Allocator>;
}

#endif