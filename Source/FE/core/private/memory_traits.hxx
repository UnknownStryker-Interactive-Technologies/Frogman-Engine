#ifndef _FE_CORE_PRIVATE_MEMORY_TRAITS_HXX_
#define _FE_CORE_PRIVATE_MEMORY_TRAITS_HXX_
// Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/iterator.hxx>
#include <FE/core/memory.hxx>




BEGIN_NAMESPACE(FE::internal)


template<typename T, ADDRESS DestAddressAlignment = ADDRESS::_NOT_ALIGNED, ADDRESS SourceAddressAlignment = ADDRESS::_NOT_ALIGNED, TYPE_TRIVIALITY IsTrivial = FE::is_trivial<T>::value>
class memory_traits;

template<typename T, ADDRESS DestAddressAlignment, ADDRESS SourceAddressAlignment>
class memory_traits<T, DestAddressAlignment, SourceAddressAlignment, TYPE_TRIVIALITY::_TRIVIAL> final
{
public:
	using value_type = T;

	_MAYBE_UNUSED_ static constexpr inline TYPE_TRIVIALITY is_trivial = TYPE_TRIVIALITY::_TRIVIAL;


	_FORCE_INLINE_ static void construct(_MAYBE_UNUSED_ T& out_dest_p) noexcept
	{
	}

	_FORCE_INLINE_ static void construct(T& out_dest_p, T value_p) noexcept
	{
		out_dest_p = std::move(value_p);
	}

	template<class Iterator>
	_FORCE_INLINE_ static void construct(Iterator in_out_dest_first_p, Iterator in_out_dest_last_p, const T& value_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		FE_ASSERT(in_out_dest_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(in_out_dest_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));
		
		if constexpr (std::is_class<Iterator>::value == true)
		{
			if constexpr (std::is_same<typename Iterator::iterator_category, typename FE::contiguous_iterator<typename Iterator::value_type>::category>::value == true && sizeof(T) == sizeof(var::byte))
			{
				FE_UNALIGNED_MEMSET(iterator_cast<T*>(in_out_dest_first_p), (int8)value_p, (in_out_dest_last_p - in_out_dest_first_p) * sizeof(T));
			}
			else
			{
				while (in_out_dest_first_p != in_out_dest_last_p)
				{
					*in_out_dest_first_p = value_p;
					++in_out_dest_first_p;
				}
			}
		}
		else if constexpr ((std::is_pointer<Iterator>::value == true) && (sizeof(T) == sizeof(var::byte)))
		{
			FE_UNALIGNED_MEMSET(iterator_cast<T*>(in_out_dest_first_p), (int8)value_p, (in_out_dest_last_p - in_out_dest_first_p) * sizeof(T));
		}
		else
		{
			while (in_out_dest_first_p != in_out_dest_last_p)
			{
				*in_out_dest_first_p = value_p;
				++in_out_dest_first_p;
			}
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_construct(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		if constexpr (std::is_pointer<Iterator>::value == true)
		{
			FE::memcpy<DestAddressAlignment, SourceAddressAlignment>(	iterator_cast<T*>(out_dest_p), dest_capacity_p * sizeof(T), 
																		iterator_cast<T*>(source_p), source_count_p * sizeof(T));
		}
		else if constexpr (std::is_class<Iterator>::value == true)
		{
			if constexpr (std::is_same<typename Iterator::iterator_category, typename FE::contiguous_iterator<typename Iterator::value_type>::category>::value == true)
			{
				FE::memcpy<DestAddressAlignment, SourceAddressAlignment>(	iterator_cast<T*>(out_dest_p), dest_capacity_p * sizeof(T), 
																			iterator_cast<T*>(source_p), source_count_p * sizeof(T));
			}
			else
			{
				count_t l_max_count = FE_MIN(dest_capacity_p, source_count_p);
				for (var::count_t i = 0; i < l_max_count; ++i)
				{
					*out_dest_p = *source_p;
					++out_dest_p;
					++source_p;
				}
			}
		}
	}
	
	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_construct(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		if constexpr (std::is_pointer<Iterator>::value == true)
		{
			FE::memcpy<DestAddressAlignment, SourceAddressAlignment>(iterator_cast<T*>(out_dest_p), iterator_cast<T*>(source_p), count_to_copy_p * sizeof(T));
		}
		else if constexpr (std::is_class<Iterator>::value == true)
		{
			if constexpr (std::is_same<typename Iterator::iterator_category, typename FE::contiguous_iterator<typename Iterator::value_type>::category>::value == true)
			{
				FE::memcpy<DestAddressAlignment, SourceAddressAlignment>(iterator_cast<T*>(out_dest_p), iterator_cast<T*>(source_p), count_to_copy_p * sizeof(T));
			}
			else
			{
				for (var::count_t i = 0; i < count_to_copy_p; ++i)
				{
					*out_dest_p = *source_p;
					++out_dest_p;
					++source_p;
				}
			}
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_construct(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, dest_capacity_p, source_p, source_count_p);
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_construct(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, source_p, count_to_copy_p);
	}


	_FORCE_INLINE_ static void destruct(_MAYBE_UNUSED_ T& out_dest_p) noexcept
	{
		out_dest_p = T();
	}

	template<class Iterator>
	_FORCE_INLINE_ static void destruct(_MAYBE_UNUSED_ Iterator in_out_dest_first_p, _MAYBE_UNUSED_ Iterator in_out_dest_last_p) noexcept
	{
		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));

	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_assign(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, dest_capacity_p, source_p, source_count_p);
	}
	
	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_assign(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, source_p, count_to_copy_p);
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_assign(Iterator out_dest_p, count_t dest_count_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(dest_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, dest_count_p, source_p, source_count_p);
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_assign(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		memory_traits<T>::template copy_construct<Iterator>(out_dest_p, source_p, count_to_copy_p);
	}


	template<class Iterator>
	_FORCE_INLINE_ static void assign(Iterator in_out_dest_first_p, Iterator in_out_dest_last_p, const T& value_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		FE_ASSERT(in_out_dest_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(in_out_dest_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));

		memory_traits<T>::template construct<Iterator>(in_out_dest_first_p, in_out_dest_last_p, value_p);
	}
};


template<typename T, ADDRESS DestAddressAlignment, ADDRESS SourceAddressAlignment>
class memory_traits<T, DestAddressAlignment, SourceAddressAlignment, TYPE_TRIVIALITY::_NOT_TRIVIAL> final
{
public:
	_MAYBE_UNUSED_ static constexpr inline TYPE_TRIVIALITY is_trivial = TYPE_TRIVIALITY::_NOT_TRIVIAL;
	using value_type = T;

	_FORCE_INLINE_ static void construct(_MAYBE_UNUSED_ T& out_dest_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		new(&out_dest_p) T();
	}

	_FORCE_INLINE_ static void construct(T& out_dest_p, T value_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		new(&out_dest_p) T(std::move(value_p));
	}

	template<class Iterator>
	_FORCE_INLINE_ static void construct(Iterator in_out_dest_first_p, Iterator in_out_dest_last_p, const T& value_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		FE_ASSERT(in_out_dest_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(in_out_dest_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));

		while (in_out_dest_first_p != in_out_dest_last_p)
		{
			new(iterator_cast<T*>(in_out_dest_first_p)) T(value_p);
			++in_out_dest_first_p;
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_construct(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		if (dest_capacity_p <= source_count_p)
		{
			for (var::count_t i = 0; i < dest_capacity_p; ++i)
			{
				new(iterator_cast<T*>(out_dest_p)) T(*source_p);
				++out_dest_p;
				++source_p;
			}
			return;
		}
		else
		{
			const T* const source_end = iterator_cast<const T* const>(source_p + source_count_p);
			while (source_p != source_end)
			{
				new(iterator_cast<T*>(out_dest_p)) T(*source_p);
				++out_dest_p;
				++source_p;
			}
			return;
		}
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_construct(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		for (var::count_t i = 0; i < count_to_copy_p; ++i)
		{
			new(iterator_cast<T*>(out_dest_p)) T(*source_p);
			++out_dest_p;
			++source_p;
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_construct(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		if (dest_capacity_p <= source_count_p)
		{
			const T* const dest_end = iterator_cast<const T* const>(out_dest_p + dest_capacity_p);
			while (out_dest_p != dest_end)
			{
				new(iterator_cast<T*>(out_dest_p)) T(std::move(*source_p));
				++out_dest_p;
				++source_p;
			}
			return;
		}
		else
		{
			for (var::count_t i = 0; i < source_count_p; ++i)
			{
				new(iterator_cast<T*>(out_dest_p)) T(std::move(*source_p));
				++out_dest_p;
				++source_p;
			}
			return;
		}
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_construct(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		for (var::count_t i = 0; i < count_to_copy_p; ++i)
		{
			new(iterator_cast<T*>(out_dest_p)) T(std::move(*source_p));
			++out_dest_p;
			++source_p;
		}
	}


	_FORCE_INLINE_ static void destruct(_MAYBE_UNUSED_ T& out_dest_p) noexcept
	{
		static_assert(std::is_destructible<T>::value == true, "static assertion failed: The typename T must be destructible.");
		out_dest_p.~T();
	}

	template<class Iterator>
	_FORCE_INLINE_ static void destruct(_MAYBE_UNUSED_ Iterator in_out_dest_first_p, Iterator in_out_dest_last_p) noexcept
	{
		static_assert(std::is_destructible<T>::value == true, "static assertion failed: The typename T must be destructible.");
		FE_ASSERT(in_out_dest_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(in_out_dest_first_p));
		FE_ASSERT(in_out_dest_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(in_out_dest_last_p));
		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));

		while (in_out_dest_first_p != in_out_dest_last_p)
		{
			in_out_dest_first_p->~T();
			++in_out_dest_first_p;
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_assign(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		if (dest_capacity_p <= source_count_p)
		{
			for (var::count_t i = 0; i < dest_capacity_p; ++i)
			{
				*out_dest_p = *source_p;
				++out_dest_p;
				++source_p;
			}
			return;
		}
		else
		{
			for (var::count_t i = 0; i < source_count_p; ++i)
			{
				*out_dest_p = *source_p;
				++out_dest_p;
				++source_p;
			}
			return;
		}
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void copy_assign(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		for (var::count_t i = 0; i < count_to_copy_p; ++i)
		{
			*out_dest_p = *source_p;
			++out_dest_p;
			++source_p;
		}
	}


	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_assign(Iterator out_dest_p, count_t dest_capacity_p, InputIterator source_p, count_t source_count_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(dest_capacity_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(dest_capacity_p));
		FE_ASSERT(source_count_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_count_p));

		if (dest_capacity_p <= source_count_p)
		{
			for (var::count_t i = 0; i < dest_capacity_p; ++i)
			{
				*out_dest_p = std::move(*source_p);
				++out_dest_p;
				++source_p;
			}
			return;
		}
		else
		{
			for (var::count_t i = 0; i < source_count_p; ++i)
			{
				*out_dest_p = std::move(*source_p);
				++out_dest_p;
				++source_p;
			}
			return;
		}
	}

	template<class Iterator, class InputIterator>
	_FORCE_INLINE_ static void move_assign(Iterator out_dest_p, InputIterator source_p, count_t count_to_copy_p) noexcept
	{
		FE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));

		FE_ASSERT(count_to_copy_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(count_to_copy_p));

		for (var::count_t i = 0; i < count_to_copy_p; ++i)
		{
			*out_dest_p = std::move(*source_p);
			++out_dest_p;
			++source_p;
		}
	}


	template<class Iterator>
	_FORCE_INLINE_ static void assign(Iterator in_out_dest_first_p, Iterator in_out_dest_last_p, const T& value_p) noexcept
	{
		static_assert(std::is_constructible<T>::value == true, "static assertion failed: The typename T must be copy constructible.");

		FE_ASSERT(in_out_dest_first_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(out_dest_p));
		FE_ASSERT(in_out_dest_last_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(source_p));
		FE_ASSERT(in_out_dest_first_p > in_out_dest_last_p, "${%s@0}: The begin iterator ${%s@1} must be pointing at the first element of a container.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_POSITIONING), TO_STRING(in_out_dest_first_p));

		while (in_out_dest_first_p != in_out_dest_last_p)
		{
			*in_out_dest_first_p = value_p;
			++in_out_dest_first_p;
		}
	}
};
END_NAMESPACE
#endif