#ifndef _FE_CORE_DEQUE_HXX
#define _FE_CORE_DEQUE_HXX
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
#include <FE/memory.hxx>
#include <memory>




BEGIN_NAMESPACE(FE)


namespace internal::deque
{
	template <typename T>
	consteval FE::size calculate_node_size() noexcept
	{
		if constexpr ( sizeof(T) >= ( (4 * FE::one_KiB) - 32 ) )
		{
			return 2;
		}

		constinit var::size l_size = (FE::CPU_L1_cache_line::size / sizeof(T));
		if constexpr (l_size <= 1)
		{
			l_size = ((4 * FE::one_KiB) - 32) / sizeof(T);
		}
		return l_size;
	}
}


template <typename T, class Allocator = FE::polymorphic_allocator<T>> 
class deque
{
	class node
	{
	public:
		constexpr static FE::size array_size = FE::internal::deque::calculate_node_size<T>();
		constexpr static FE::size array_size_in_bytes = array_size * sizeof(T);

	private:
		var::byte m_data[array_size_in_bytes];
		T* m_front;
		T* m_back;

	public:
		// emplace_front, emplace_back, pop_front, pop_back
	};

public:
	using value_type = T;
	using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<node>;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = void; // to be implemented
	using const_iterator = void; // to be implemented
	using reverse_iterator = void; // to be implemented
	using const_reverse_iterator = void; // to be implemented

	class nodes
	{
		_FE_NO_UNIQUE_ADDRESS_ allocator_type m_allocator;
		node* m_array;
		var::size m_size;
	public:
		// ops
	};

private:
	
	nodes m_nodes;
	var::size m_capacity;
	var::size m_size;

public:
	// see: https://en.cppreference.com/w/cpp/container/deque.html
};


END_NAMESPACE
#endif