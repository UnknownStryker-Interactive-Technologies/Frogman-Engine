#ifndef _FE_CORE_BITMASK_HXX_
#define _FE_CORE_BITMASK_HXX_
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

#include <boost/hash2/xxhash.hpp>




BEGIN_NAMESPACE(FE)


class bitmask // this is a dynamic bit set designed to be used as archetype ids; expects faster performance than std::vector<bool>
{
public:
	using allocator_type = std::pmr::polymorphic_allocator<var::byte>;

private:
	using alloc_traits = std::allocator_traits<allocator_type>;

	allocator_type m_allocator;
	var::byte* m_bitmask;
	var::size m_capacity_in_bits;
	var::uint64 m_64bit_buffer;

public:
	bitmask() noexcept
		:	m_allocator(),
			m_bitmask(nullptr),
			m_capacity_in_bits(64),
			m_64bit_buffer(0)
	{}

	bitmask(FE::uint64 value_p) noexcept
		:	m_allocator(),
			m_bitmask(nullptr),
			m_capacity_in_bits(64),
			m_64bit_buffer(value_p)
	{}

	constexpr ~bitmask() noexcept
	{
		if (m_bitmask == nullptr)
		{
			return;
		}

		m_allocator.deallocate( m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits) );
	}


	constexpr bitmask(const bitmask& other_p) noexcept
		:	m_allocator(other_p.m_allocator),
			m_bitmask(),
			m_capacity_in_bits(other_p.m_capacity_in_bits),
			m_64bit_buffer(other_p.m_64bit_buffer)
	{
		if ( other_p.__large_bitset_engaged() )
		{
			FE_ASSERT(other_p.m_bitmask != nullptr, "Assertion failed: large mode engaged, but the bitmask pointer is null.");
			FE::size l_incoming_size_in_bytes = __calculate_size_of_bits_in_bytes(other_p.m_capacity_in_bits);
			m_bitmask = m_allocator.allocate( l_incoming_size_in_bytes );
			FE::memcpy(m_bitmask, other_p.m_bitmask, l_incoming_size_in_bytes);
		}
	}
	
	constexpr bitmask& operator=(const bitmask& other_p) noexcept
	{
		if (&other_p == this)
		{
			return *this;
		}

		if (m_bitmask != nullptr)
		{
			m_allocator.deallocate(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits));
		}

		if (other_p.__large_bitset_engaged() )
		{
			FE_ASSERT(other_p.m_bitmask != nullptr, "Assertion failed: large mode engaged, but the bitmask pointer is null.");
			FE::size l_incoming_size_in_bytes = __calculate_size_of_bits_in_bytes(other_p.m_capacity_in_bits);
			m_bitmask = m_allocator.allocate( l_incoming_size_in_bytes );
			FE::memcpy(m_bitmask, other_p.m_bitmask, l_incoming_size_in_bytes);
		}
		else // small mode
		{
			m_64bit_buffer = other_p.m_64bit_buffer;
		}
		m_capacity_in_bits = other_p.m_capacity_in_bits;	
		return *this;
	}


	constexpr bitmask(bitmask&& other_p) noexcept
		:	m_allocator(other_p.m_allocator),
			m_bitmask(other_p.m_bitmask),
			m_capacity_in_bits(other_p.m_capacity_in_bits),
			m_64bit_buffer(other_p.m_64bit_buffer)
	{
		other_p.m_bitmask = nullptr;
		other_p.m_capacity_in_bits = 64;
		other_p.m_64bit_buffer = 0;
	}

	constexpr bitmask& operator=(bitmask&& other_p) noexcept
	{
		if (&other_p == this)
		{
			return *this;
		}

		if (m_bitmask != nullptr)
		{
			m_allocator.deallocate(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits));
		}

		//if constexpr (alloc_traits::propagate_on_container_copy_assignment::value == true)
		//{
		//	m_allocator = other_p.m_allocator;
		//}

		m_bitmask = other_p.m_bitmask;
		other_p.m_bitmask = nullptr;

		m_capacity_in_bits = other_p.m_capacity_in_bits;
		other_p.m_capacity_in_bits = 64;

		m_64bit_buffer = other_p.m_64bit_buffer;
		other_p.m_64bit_buffer = 0;

		return *this;
	}


	_FE_FORCE_INLINE_ constexpr const allocator_type& get_allocator() const noexcept { return m_allocator; }
	
	constexpr bool read_at(FE::size index_p) const noexcept
	{
		static_assert( sizeof(m_64bit_buffer) == 8 );
		FE_ASSERT(index_p < m_capacity_in_bits, "Assertion failed: index is out of bounds of the bitmask capacity.");

		if (__large_bitset_engaged())
		{
			FE::size l_slot_index = index_p / 8;
			FE::size l_bit_index = index_p % 8;

			var::byte l_8bit_slot = m_bitmask[l_slot_index] << (7 - l_bit_index); 
			l_8bit_slot = l_8bit_slot >> 7;
			return static_cast<bool>(l_8bit_slot);
		}
		else
		{
			var::uint64 l_64bit_slot = m_64bit_buffer << (63 - index_p);
			l_64bit_slot = l_64bit_slot >> 63; 
			return static_cast<bool>(l_64bit_slot);
		}
	}

	constexpr void write_at(FE::size index_p, bool value_p) noexcept
	{
		static_assert(sizeof(m_64bit_buffer) == 8);
		FE_ASSERT(index_p < m_capacity_in_bits, "Assertion failed: index is out of bounds of the bitmask capacity.");

		if (__large_bitset_engaged())
		{
			FE::size l_slot_index = index_p / 8;
			FE::byte l_bit_index = static_cast<FE::byte>(index_p % 8);

			var::byte l_8bit_slot = 0b00000001;
			l_8bit_slot = l_8bit_slot << l_bit_index; 
			if (value_p == true)
			{
				m_bitmask[l_slot_index] = m_bitmask[l_slot_index] | l_8bit_slot; 
			}
			else
			{
				m_bitmask[l_slot_index] = m_bitmask[l_slot_index] ^ l_8bit_slot;
			}
		}
		else
		{
			var::uint64 l_64bit_slot = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
			l_64bit_slot = l_64bit_slot << index_p;
			if (value_p == true)
			{
				m_64bit_buffer = m_64bit_buffer | l_64bit_slot;
			}
			else
			{
				m_64bit_buffer = m_64bit_buffer ^ l_64bit_slot;
			}
		}
	}


	constexpr void reserve(FE::size new_capacity_in_bits_p) noexcept
	{
		FE::size l_new_size_in_bytes = __calculate_size_of_bits_in_bytes(new_capacity_in_bits_p);
		FE::size l_current_size_in_bytes = __calculate_size_of_bits_in_bytes(m_capacity_in_bits);

		if (l_current_size_in_bytes >= l_new_size_in_bytes)
		{
			return;
		}

		var::byte* l_new_bitmask = m_allocator.allocate(l_new_size_in_bytes);
		std::memset(l_new_bitmask, 0, l_new_size_in_bytes);

		if (m_bitmask != nullptr)
		{		
			FE::memcpy(l_new_bitmask, l_new_size_in_bytes,
						m_bitmask, l_current_size_in_bytes);
			m_allocator.deallocate(m_bitmask, l_current_size_in_bytes);
		}
		else
		{
			*((var::uint64*)l_new_bitmask) = m_64bit_buffer;
		}
		m_bitmask = l_new_bitmask;
		m_capacity_in_bits = new_capacity_in_bits_p;
	}

	constexpr void resize(FE::size new_capacity_in_bits_p) noexcept
	{
		if ((new_capacity_in_bits_p == m_capacity_in_bits) ||
			(new_capacity_in_bits_p <= 64))
		{
			return;
		}

		FE::size l_new_size_in_bytes = __calculate_size_of_bits_in_bytes(new_capacity_in_bits_p);
		var::byte* l_new_bitmask = m_allocator.allocate( l_new_size_in_bytes );
		std::memset(l_new_bitmask, 0, l_new_size_in_bytes);

		if (m_bitmask != nullptr)
		{

			FE::size l_current_size_in_bytes = __calculate_size_of_bits_in_bytes(m_capacity_in_bits);
			FE::memcpy(l_new_bitmask, l_new_size_in_bytes,
						m_bitmask, l_current_size_in_bytes);
			m_allocator.deallocate(m_bitmask, l_current_size_in_bytes);
		}
		else
		{
			*((var::uint64*)l_new_bitmask) = m_64bit_buffer;
		}
		m_bitmask = l_new_bitmask;
		m_capacity_in_bits = new_capacity_in_bits_p;
	}

	constexpr void clear() noexcept
	{
		if (m_bitmask != nullptr)
		{
			m_allocator.deallocate(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits));
			m_bitmask = nullptr;
		}

		m_capacity_in_bits = 64;
		m_64bit_buffer = 0;
	}


	constexpr FE::size bitwidth_capacity() const noexcept
	{
		return m_capacity_in_bits;
	}


	constexpr bitmask& operator&=(const bitmask& other_p) noexcept
	{
		if (__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			FE_BITWISE_AND(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				other_p.m_bitmask, __calculate_size_of_bits_in_bytes(other_p.m_capacity_in_bits)
			);
			return *this;
		}

		if (__large_bitset_engaged())
		{
			*reinterpret_cast<var::uint64*>(m_bitmask) = *reinterpret_cast<var::uint64*>(m_bitmask) & other_p.m_64bit_buffer;
			return *this;
		}

		if (other_p.__large_bitset_engaged())
		{
			m_64bit_buffer = m_64bit_buffer & *reinterpret_cast<var::uint64*>(other_p.m_bitmask);
			return *this;
		}
		m_64bit_buffer = m_64bit_buffer & other_p.m_64bit_buffer;
		return *this;
	}

	constexpr bitmask& operator|=(const bitmask& other_p) noexcept
	{
		if (__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			FE_BITWISE_OR(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				other_p.m_bitmask, __calculate_size_of_bits_in_bytes(other_p.m_capacity_in_bits)
			);
			return *this;
		}

		if (__large_bitset_engaged())
		{
			*reinterpret_cast<var::uint64*>(m_bitmask) = *reinterpret_cast<var::uint64*>(m_bitmask) | other_p.m_64bit_buffer;
			return *this;
		}

		if (other_p.__large_bitset_engaged())
		{
			m_64bit_buffer = m_64bit_buffer | *reinterpret_cast<var::uint64*>(other_p.m_bitmask);
			return *this;
		}
		m_64bit_buffer = m_64bit_buffer | other_p.m_64bit_buffer;
		return *this;
	}

	constexpr bitmask& operator^=(const bitmask& other_p) noexcept
	{
		if (__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			FE_BITWISE_XOR(m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				m_bitmask, __calculate_size_of_bits_in_bytes(m_capacity_in_bits),
				other_p.m_bitmask, __calculate_size_of_bits_in_bytes(other_p.m_capacity_in_bits)
			);
			return *this;
		}

		if (__large_bitset_engaged())
		{
			*reinterpret_cast<var::uint64*>(m_bitmask) = *reinterpret_cast<var::uint64*>(m_bitmask) xor other_p.m_64bit_buffer;
			return *this;
		}

		if (other_p.__large_bitset_engaged())
		{
			m_64bit_buffer = m_64bit_buffer xor *reinterpret_cast<var::uint64*>(other_p.m_bitmask);
			return *this;
		}
		m_64bit_buffer = m_64bit_buffer xor other_p.m_64bit_buffer;
		return *this;
	}

	bitmask operator~() noexcept
	{
		bitmask l_bitmask = *this;
		if (l_bitmask.__large_bitset_engaged())
		{
			FE_BITWISE_NOT( l_bitmask.m_bitmask, l_bitmask.__calculate_size_of_bits_in_bytes( l_bitmask.m_capacity_in_bits ) );
			return *this;
		}
		l_bitmask.m_64bit_buffer = ~(l_bitmask.m_64bit_buffer);
		return l_bitmask;
	}

	constexpr bool operator==(const bitmask& other_p) const noexcept
	{
		if (&other_p == this)
		{
			return true;
		}

		if (__large_bitset_engaged() && !other_p.__large_bitset_engaged())
		{
			return false;
		}

		if (!__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			return false;
		}

		if (__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			return std::memcmp(m_bitmask, other_p.m_bitmask, __calculate_size_of_bits_in_bytes(std::min(m_capacity_in_bits, other_p.m_capacity_in_bits))) == 0;
		}
		return m_64bit_buffer == other_p.m_64bit_buffer;
	}

	constexpr bool operator!=(const bitmask& other_p) const noexcept
	{
		if (&other_p == this)
		{
			return false;
		}

		if (__large_bitset_engaged() && !other_p.__large_bitset_engaged())
		{
			return true;
		}

		if (!__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			return true;
		}

		if (__large_bitset_engaged() && other_p.__large_bitset_engaged())
		{
			return std::memcmp(m_bitmask, other_p.m_bitmask, __calculate_size_of_bits_in_bytes( std::min(m_capacity_in_bits, other_p.m_capacity_in_bits) )) != 0;
		}
		return m_64bit_buffer != other_p.m_64bit_buffer;
	}

	_FE_FORCE_INLINE_ FE::uint64 hash_code() const noexcept
	{
		boost::hash2::xxhash_64 l_xxhash_64;
		if (__large_bitset_engaged())
		{
			l_xxhash_64.update(reinterpret_cast<const unsigned char*>(m_bitmask), __calculate_size_of_bits_in_bytes(m_capacity_in_bits));
			return l_xxhash_64.result();
		}
		l_xxhash_64.update(reinterpret_cast<const unsigned char*>(&m_64bit_buffer), sizeof(m_64bit_buffer));
		return l_xxhash_64.result();
	}

	_FE_FORCE_INLINE_ FE::byte* data() const noexcept
	{
		if (__large_bitset_engaged())
		{
			return m_bitmask;
		}
		return reinterpret_cast<FE::byte*>(&m_64bit_buffer);
	}

	_FE_FORCE_INLINE_ FE::size capacity_in_bytes() const noexcept
	{
		return __calculate_size_of_bits_in_bytes(m_capacity_in_bits);
	}

private:
	_FE_FORCE_INLINE_ constexpr FE::size __calculate_size_of_bits_in_bytes(FE::size capacity_in_bits_p) const noexcept
	{
		return (capacity_in_bits_p + 7) / 8;
	}

	_FE_FORCE_INLINE_ constexpr FE::boolean __large_bitset_engaged() const noexcept
	{
		return m_capacity_in_bits > 64;
	}
};


END_NAMESPACE
#endif