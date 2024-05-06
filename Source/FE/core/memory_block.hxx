#ifndef _FE_CORE_MEMORY_BLOCK_HXX_
#define _FE_CORE_MEMORY_BLOCK_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/memory.hxx>
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier




BEGIN_NAMESPACE(FE)


template<typename T, class Alignment = FE::align_8bytes>
class alignas(Alignment::size) memory_block final
{
private:
	var::boolean m_is_constructed;
	var::byte m_memory[sizeof(T)];

public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const reference;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using alignment_type = Alignment;
	
	_FORCE_INLINE_ memory_block() noexcept : m_is_constructed(), m_memory() {}

	_FORCE_INLINE_ ~memory_block()
	{
		if (this->m_is_constructed == false)
		{
			return;
		}

		reinterpret_cast<pointer>(this->m_memory)->~T();
		this->m_is_constructed = false;
	}

	_FORCE_INLINE_ memory_block(const memory_block& other_p) noexcept : m_is_constructed(other_p.m_is_constructed), m_memory()
	{
		if (other_p.m_is_constructed == false)
		{
			return;
		}

		pointer l_this_memory = reinterpret_cast<pointer>(this->m_memory);
		new(l_this_memory) T();
		*l_this_memory = *reinterpret_cast<pointer>(other_p.m_memory);
	}

	_FORCE_INLINE_ memory_block(memory_block&& rvalue_p) noexcept : m_is_constructed(rvalue_p.m_is_constructed), m_memory()
	{
		if (rvalue_p.m_is_constructed == false)
		{
			return;
		}

		pointer l_this_memory = reinterpret_cast<pointer>(this->m_memory);
		new(l_this_memory) T();
		*l_this_memory = std::move(*reinterpret_cast<pointer>(rvalue_p.m_memory));
	}

	_FORCE_INLINE_ memory_block& operator=(const memory_block& other_p) noexcept
	{
		if (other_p.m_is_constructed == false)
		{
			return *this;
		}

		pointer l_this_memory = reinterpret_cast<pointer>(this->m_memory);
		if (this->m_is_constructed == false)
		{
			new(l_this_memory) T();
			this->m_is_constructed = true;
		}

		*l_this_memory = *reinterpret_cast<pointer>(other_p.m_memory);

		return *this;
	}

	_FORCE_INLINE_ memory_block& operator=(memory_block&& rvalue_p) noexcept
	{
		if (rvalue_p.m_is_constructed == false)
		{
			return *this;
		}

		pointer l_this_memory = reinterpret_cast<pointer>(this->m_memory);
		if (this->m_is_constructed == false)
		{
			new(l_this_memory) T();
			this->m_is_constructed = true;
		}

		*l_this_memory = std::move(*reinterpret_cast<pointer>(rvalue_p.m_memory));

		return *this;
	}

	_FORCE_INLINE_ T& operator*() noexcept
	{
		FE_ASSERT(this->m_is_constructed == false, "ERROR: Attempted to dereference an uninitialized memory block.");

		return *reinterpret_cast<pointer>(this->m_memory);
	}

	_FORCE_INLINE_ T* operator->() noexcept
	{
		FE_ASSERT(this->m_is_constructed == false, "ERROR: Attempted to access an uninitialized memory block.");

		return reinterpret_cast<pointer>(this->m_memory);
	}

	_FORCE_INLINE_ boolean is_constructed() noexcept { return this->m_is_constructed; }

	_FORCE_INLINE_ boolean construct() noexcept
	{
		if (this->m_is_constructed == false)
		{
			this->m_is_constructed = true;
			new(reinterpret_cast<pointer>(this->m_memory)) T();
		}
		return this->m_is_constructed;
	}

	_FORCE_INLINE_ boolean construct(T value_p) noexcept
	{
		if (this->m_is_constructed == false)
		{
			this->m_is_constructed = true;
			new(reinterpret_cast<pointer>(this->m_memory)) T(std::move(value_p));
		}
		return this->m_is_constructed;
	}

	template<typename ... Arguments>
	_FORCE_INLINE_ boolean construct(Arguments&& ...arguments_p) noexcept
	{
		if (this->m_is_constructed == false)
		{
			this->m_is_constructed = true;
			new(reinterpret_cast<pointer>(this->m_memory)) T(arguments_p...);
		}
		return this->m_is_constructed;
	}

	_FORCE_INLINE_ boolean destruct() noexcept
	{
		if (this->m_is_constructed == true)
		{
			this->m_is_constructed = false;
			reinterpret_cast<pointer>(this->m_memory)->~T();
		}
		return this->m_is_constructed;
	}

};


END_NAMESPACE
#pragma warning(pop)
#endif
