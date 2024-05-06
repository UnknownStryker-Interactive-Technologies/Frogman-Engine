#ifndef _FE_CORE_STRING_VIEW_HXX_
#define _FE_CORE_STRING_VIEW_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/smart_pointers/ptr.hxx>
#include <FE/core/algorithm/string.hxx>
#include <FE/core/algorithm/utility.hxx>
#include <FE/core/iterator.hxx>
#include <optional>




BEGIN_NAMESPACE(FE)


template<typename CharT, class Allocator, class Traits>
class basic_string;


template<typename CharT>
class basic_string_view final
{
	FE_STATIC_ASSERT(FE::is_char<CharT>::value == false, "CharT is not a valid character type");

	FE::ptr<CharT[]> m_watcher;
	var::index_t m_begin;
	var::index_t m_end;

public:
	using value_type = CharT;
	using pointer = CharT*;
	using const_pointer = const CharT*;
	using reference = CharT&;
	using const_reference = const CharT&;
	using const_iterator = FE::const_iterator<FE::contiguous_iterator<CharT>>;
	using iterator = FE::iterator<FE::contiguous_iterator<CharT>>;
	using const_reverse_iterator = FE::const_reverse_iterator<FE::contiguous_iterator<CharT>>;
	using reverse_iterator = FE::reverse_iterator<FE::contiguous_iterator<CharT>>;
	using size_type = var::index_t;
	using length_type = var::index_t;
	using difference_type = ptrdiff_t;


	_CONSTEXPR17_ basic_string_view() noexcept : m_watcher(), m_begin(), m_end() {}
	_CONSTEXPR17_ basic_string_view(basic_string_view& other_p) noexcept : m_watcher(other_p.m_watcher), m_begin(other_p.m_begin), m_end(other_p.m_end) {}
	_CONSTEXPR17_ basic_string_view(basic_string_view&& rvalue_p) noexcept : m_watcher(rvalue_p.m_watcher), m_begin(rvalue_p.m_begin), m_end(rvalue_p.m_end) 
	{
		rvalue_p.m_watcher.reset();
		rvalue_p.m_begin = 0;
		rvalue_p.m_end = 0;
	}

	_CONSTEXPR20_ ~basic_string_view() noexcept {}


	template<class Allocator>
	_CONSTEXPR20_ basic_string_view(const FE::exclusive_ptr<CharT[], Allocator>& source_pointer_p) noexcept : m_watcher(source_pointer_p), m_begin(), m_end()
	{
		if (this->m_watcher.is_expired() == true)
		{
			return;
		}

		this->m_end = algorithm::string::length(this->m_watcher.get_unchecked());
	}


	_CONSTEXPR20_ basic_string_view& operator=(basic_string_view& other_p) noexcept
	{
		if (other_p.m_watcher.is_expired() == true)
		{
			return *this;
		}

		this->m_watcher = other_p.m_watcher;
		this->m_begin = other_p.m_begin;
		this->m_end = other_p.m_end;

		return *this;
	}

	_CONSTEXPR20_ basic_string_view& operator=(basic_string_view&& rvalue_p) noexcept
	{
		if (rvalue_p.m_watcher.is_expired() == true)
		{
			return *this;
		}

		this->m_watcher = rvalue_p.m_watcher;
		this->m_begin = rvalue_p.m_begin;
		this->m_end = rvalue_p.m_end;

		rvalue_p.m_watcher.reset();
		rvalue_p.m_begin = 0;
		rvalue_p.m_end = 0;

		return *this;
	}

	template<class Allocator>
	_CONSTEXPR20_ basic_string_view& operator=(const FE::exclusive_ptr<CharT[], Allocator>& source_pointer_p) noexcept
	{
		if (source_pointer_p == nullptr)
		{
			return *this;
		}

		this->m_begin = 0;
		this->m_end = algorithm::string::length(source_pointer_p.get());
		this->m_watcher = source_pointer_p;
	}


	_NODISCARD_ _FORCE_INLINE_ iterator begin() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return iterator{ l_pointer_to_string + this->m_begin };
	}
	_NODISCARD_ _FORCE_INLINE_ iterator end() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return iterator{ l_pointer_to_string + this->m_end };
	}

	_NODISCARD_ _FORCE_INLINE_ const_iterator cbegin() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return const_iterator{ l_pointer_to_string + this->m_begin };
	}
	_NODISCARD_ _FORCE_INLINE_ const_iterator cend() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return const_iterator{ l_pointer_to_string + this->m_end };
	}

	_NODISCARD_ _FORCE_INLINE_ reverse_iterator rbegin() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return reverse_iterator{ (l_pointer_to_string + this->m_end) - 1 };
	}

	_NODISCARD_ _FORCE_INLINE_ reverse_iterator rend() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return reverse_iterator{ (l_pointer_to_string + this->m_begin) - 1 };
	}

	_NODISCARD_ _FORCE_INLINE_ const_reverse_iterator crbegin() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return const_reverse_iterator{ (l_pointer_to_string + this->m_end) - 1 };
	}

	_NODISCARD_ _FORCE_INLINE_ const_reverse_iterator crend() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return const_reverse_iterator{ (l_pointer_to_string + this->m_begin) - 1 };
	}
	

	_NODISCARD_ _FORCE_INLINE_ CharT& operator[](index_t index_p) const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		FE_ASSERT(index_p >= this->m_end, "${%s@0}: ${%s@1} was ${%lu@2}, and the string capacity was ${%lu@3}", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(index_p), &index_p, &m_end);
		return l_pointer_to_string[index_p];
	}

	_NODISCARD_ _FORCE_INLINE_ CharT front() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return *(l_pointer_to_string + this->m_begin);
	}
	_NODISCARD_ _FORCE_INLINE_ CharT back() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return *((l_pointer_to_string + this->m_end) - 1);
	}
	_NODISCARD_ _FORCE_INLINE_ CharT* data() const noexcept
	{
		pointer l_pointer_to_string = this->m_watcher.operator->();
		FE_ASSERT(l_pointer_to_string == nullptr, "${%s@0}: ${%s@1} was nullptr.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(l_pointer_to_string));
		return l_pointer_to_string + this->m_begin;
	}


	_NODISCARD_ _FORCE_INLINE_ length_type length() const noexcept
	{
		FE_ASSERT(this->m_end < this->m_begin, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}. ${%s@1} was ${%lu@3}, and ${%s@2} was ${%lu@4}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_ILLEGAL_POSITION), TO_STRING(this->m_begin), TO_STRING(this->m_end), &m_begin, &m_end);
		return this->m_end - this->m_begin; 
	}

	_NODISCARD_ _FORCE_INLINE_ length_type max_length() const noexcept { return FE::max_value<length_type> / sizeof(CharT); }
	_NODISCARD_ _FORCE_INLINE_ boolean is_empty() const noexcept { return this->m_begin == this->m_end; }
	_NODISCARD_ _FORCE_INLINE_ boolean is_valid() const noexcept { return !(this->m_watcher.is_expired()); }


	_CONSTEXPR17_ void remove_prefix(const size_type count_p) noexcept
	{
		FE_ASSERT(this->m_begin >= this->m_end, "${%s@0}: Unable to shrink the front of an empty string view", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		this->m_begin += count_p;
	}

	_CONSTEXPR17_ void remove_suffix(const size_type count_p) noexcept
	{
		FE_ASSERT(this->m_begin >= this->m_end, "${%s@0}: Unable to shrink the back of an empty string view", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE));
		this->m_end -= count_p;
	}


	_CONSTEXPR20_ void reset_range() noexcept
	{
		FE_ASSERT(this->m_watcher.is_expired() == true, "Assertion Failed: Cannot reset an empty string view.");

		this->m_begin = 0;
		this->m_end = algorithm::string::length(this->m_watcher.get_unchecked());
	}
	
	_CONSTEXPR20_ void swap(basic_string_view& in_out_other_p) noexcept
	{
		algorithm::utility::swap(*this, in_out_other_p);
	}


	_CONSTEXPR20_ void copy(CharT* const out_dest_buffer_pointer_p, const size_type buffer_capacity_p, const size_type count_p, const size_type position_p = 0) const noexcept
	{
		FE_ASSERT(out_dest_buffer_pointer_p == nullptr, "${%s@0}: out_dest_string_buffer_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(count_p > this->length(), "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(count_p), TO_STRING(this->length()));

		algorithm::string::copy(out_dest_buffer_pointer_p, buffer_capacity_p, this->begin().operator->() + position_p, count_p);
	}

	template<typename CharT, class Allocator, class Traits>
	_CONSTEXPR20_ void copy(basic_string<CharT, Allocator, Traits>& out_dest_string_buffer_p, _MAYBE_UNUSED_ const size_type buffer_capacity_p, const size_type count_p, const size_type position_p = 0) const noexcept
	{
		pointer l_out_dest_string_buffer = out_dest_string_buffer_p.data();
		FE_ASSERT(l_out_dest_string_buffer == nullptr, "${%s@0}: out_dest_string_buffer_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(count_p > this->length(), "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(count_p), TO_STRING(this->length()));
		FE_ASSERT(count_p > buffer_capacity_p, "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY), TO_STRING(count_p), TO_STRING(buffer_capacity_p));

		algorithm::string::copy(l_out_dest_string_buffer, this->begin().operator->() + position_p, count_p);
	}
	
	_NODISCARD_ _CONSTEXPR20_ basic_string_view substr(const size_type position_p, const size_type count_p) noexcept
	{
		FE_ASSERT(this->m_watcher.is_expired() == true, "Assertion Failed: Cannot return a substring of an empty string view.");
		
		basic_string_view l_tmp_view;
		l_tmp_view = *this;
		l_tmp_view.m_begin = this->m_begin + position_p;
		l_tmp_view.m_end = this->m_begin + position_p + count_p;

		return l_tmp_view;
	}

	_NODISCARD_ _CONSTEXPR20_ boolean starts_with(const basic_string_view& other_p) const noexcept
	{
		if (other_p.is_empty() == true)
		{
			return false;
		}

		length_t l_other_string_length = other_p.length();

		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_begin, l_other_string_length },
			other_p.begin().operator->(), algorithm::string::range{other_p.m_begin, l_other_string_length}
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean starts_with(const CharT value_p) const noexcept
	{
		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_begin, this->m_begin + 1},
			&value_p, algorithm::string::range{ 0, 1 }
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean starts_with(const CharT* const string_p) const noexcept
	{
		FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@0} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));

		length_t l_input_string_length = algorithm::string::length(string_p);

		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_begin, l_input_string_length },
			string_p, algorithm::string::range{ 0, l_input_string_length }
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean ends_with(const basic_string_view& other_p) const noexcept
	{
		if (other_p.m_string_length == 0)
		{
			return false;
		}

		length_t l_other_string_length = other_p.length();

		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_end - this->length(), this->m_end },
			other_p.begin().operator->(), algorithm::string::range{ other_p.m_end - other_p.length(), other_p.m_end }
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean ends_with(const CharT value_p) const noexcept
	{
		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_end - 1, this->m_end },
			&value_p, algorithm::string::range{ 0, 1 }
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean ends_with(const CharT* const string_p) const noexcept
	{
		FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
		
		length_t l_input_string_length = algorithm::string::length(string_p);
		return algorithm::string::compare_ranged(this->begin().operator->(), algorithm::string::range{ this->m_end - l_input_string_length, this->m_end },
			string_p, algorithm::string::range{ 0, l_input_string_length }
		);
	}

	_NODISCARD_ _CONSTEXPR20_ boolean contains(const basic_string_view& other_p) const noexcept
	{
		if (other_p.is_empty() == true)
		{
			return false;
		}

		return (algorithm::string::find_the_first(this->begin().operator->(), other_p.begin().operator->())).has_value();
	}

	_NODISCARD_ _CONSTEXPR20_ boolean contains(const CharT value_p) const noexcept
	{
		return algorithm::string::find_the_first(this->begin().operator->(), value_p).has_value();
	}

	_NODISCARD_ _CONSTEXPR20_ boolean contains(const CharT* const string_p) const noexcept
	{
		FE_ASSERT(string_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(string_p));
		return (algorithm::string::find_the_first(this->begin().operator->(), string_p)).has_value();
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const basic_string_view& other_p, size_type position_p = 0) const noexcept
	{
		if (other_p.is_empty() == true)
		{
			return std::nullopt;
		}

		FE_ASSERT(position_p > this->length(), "${%s@0}: ${%s@1} cannot be greater than ${%s@2}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(position_p), TO_STRING(this->length()));

		return std::move(this->find(other_p.begin().operator->(), position_p));
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const CharT* const string_p, const size_type position_p = 0) const noexcept
	{
		FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->begin().operator->() + position_p, string_p);

		if (l_result == std::nullopt)
		{
			return std::nullopt;
		}

		l_result->_begin += position_p;
		l_result->_end += position_p;
		return std::move(l_result);
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> find(const CharT value_p, const size_type position_p = 0) const noexcept
	{
		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		std::optional<algorithm::string::range> l_result = algorithm::string::find_the_first(this->begin().operator->() + position_p, value_p);
		
		if (l_result == std::nullopt)
		{
			return std::nullopt;
		}

		l_result->_begin += position_p;
		l_result->_end += position_p;
		return std::move(l_result);
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const basic_string_view& other_p, const size_type position_p = 0) const noexcept
	{
		if (other_p.is_empty() == true)
		{
			return std::nullopt;
		}

		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		return std::move(this->rfind(other_p.begin().operator->(), position_p));
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const CharT* const string_p, const size_type position_p = 0) const noexcept
	{
		FE_ASSERT(string_p == nullptr, "${%s@0}: string_p is nullptr", TO_STRING(FE::MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR));
		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		return std::move(algorithm::string::find_the_last_within_range(this->begin().operator->(), algorithm::string::range{this->m_begin, position_p}, string_p));
	}

	_NODISCARD_ _CONSTEXPR20_ std::optional<algorithm::string::range> rfind(const CharT value_p, size_type position_p = 0) const noexcept
	{
		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		return std::move(algorithm::string::find_the_last_within_range(this->begin().operator->(), algorithm::string::range{this->m_begin, position_p}, value_p));
	}

	_NODISCARD_ _CONSTEXPR20_ algorithm::string::count<CharT> count_chars(const CharT value_p, const size_type position_p = 0) const noexcept
	{
		FE_ASSERT(position_p > this->length(), "${%s@0}: position_p cannot be greater than ${%s@1}.", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_RANGE), TO_STRING(this->length()));

		return std::move(algorithm::string::count_chars(this->begin().operator->() + position_p, value_p));
	}

	template<typename CharT, class Allocator, class Traits>
	_CONSTEXPR20_ boolean operator==(const basic_string<CharT, Allocator, Traits>& string_p) const noexcept
	{
		return algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, string_p.c_str(), algorithm::string::range{ 0, string_p.length() });
	}
	_CONSTEXPR20_ boolean operator==(basic_string_view& other_p) const noexcept
	{
		return algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, other_p.m_watcher.operator->(), algorithm::string::range{ other_p.m_begin, other_p.m_end });
	}
	_CONSTEXPR20_ boolean operator==(const CharT* const string_p) const noexcept
	{
		return algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, string_p, algorithm::string::range{ 0, algorithm::string::length(string_p) });
	}
	_CONSTEXPR20_ friend boolean operator==(const CharT* const string_p, basic_string_view& other_p) noexcept
	{
		return algorithm::string::compare_ranged(other_p.m_watcher.operator->(), algorithm::string::range{ other_p.m_begin(), other_p.m_end}, string_p, algorithm::string::range{0, algorithm::string::length(string_p)});
	}

	template<typename CharT, class Allocator, class Traits>
	_CONSTEXPR20_ boolean operator!=(const basic_string<CharT, Allocator, Traits>& string_p) const noexcept
	{
		return !algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, string_p.c_str(), algorithm::string::range{ 0, string_p.length() });
	}
	_CONSTEXPR20_ boolean operator!=(basic_string_view& other_p) const noexcept
	{
		return !algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, other_p.m_watcher.operator->(), algorithm::string::range{ other_p.m_begin, other_p.m_end });
	}
	_CONSTEXPR20_ boolean operator!=(const CharT* const string_p) const noexcept
	{
		return !algorithm::string::compare_ranged(this->m_watcher.operator->(), algorithm::string::range{ this->m_begin, this->m_end }, string_p, algorithm::string::range{ 0, algorithm::string::length(string_p) });
	}
	_CONSTEXPR20_ friend boolean operator!=(const CharT* const string_p, basic_string_view& other_p) noexcept
	{
		return !algorithm::string::compare_ranged(other_p.m_watcher.operator->(), algorithm::string::range{ other_p.m_begin(), other_p.m_end }, string_p, algorithm::string::range{ 0, algorithm::string::length(string_p) });
	}
};


using string_view = basic_string_view<var::character>;
using ustring_view = basic_string_view<var::uchar>;
using sstring_view = basic_string_view<var::schar>;
using wstring_view = basic_string_view<var::wchar>;

#ifdef _HAS_CXX20_
using string_view8 = basic_string_view<var::UTF8>;
#endif

using string_view16 = basic_string_view<var::UTF16>;
using string_view32 = basic_string_view<var::UTF32>;


END_NAMESPACE
#endif
