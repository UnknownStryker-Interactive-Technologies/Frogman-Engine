#ifndef _FE_CORE_TYPES_HXX_
#define _FE_CORE_TYPES_HXX_
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
#include <FE/miscellaneous/suppress_warnings.hxx>
#include <FE/definitions.hxx>

// std
#include <atomic>
#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <memory_resource>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <utility>
#pragma warning(disable: 4530) // disable no-exception warnings




BEGIN_NAMESPACE(FE)


typedef const bool boolean; // primitive types are const by default

typedef const char ASCII;  // primitive types are const by default
typedef const wchar_t wchar; // primitive types are const by default


#ifdef _FE_HAS_CXX20_
typedef const char8_t UTF8; // primitive types are const by default
#endif

typedef const char16_t UTF16; // primitive types are const by default
typedef const char32_t UTF32; // primitive types are const by default


typedef const float float32; // primitive types are const by default
typedef const double float64; // primitive types are const by default


typedef const ::std::uint8_t byte; // primitive types are const by default
typedef const ::std::uint16_t word; // primitive types are const by default
typedef const ::std::uint32_t dword; // primitive types are const by default
typedef const ::std::uint64_t qword; // primitive types are const by default


typedef const ::std::int8_t int8; // primitive types are const by default
typedef const ::std::uint8_t uint8; // primitive types are const by default
typedef const ::std::int16_t int16; // primitive types are const by default
typedef const ::std::uint16_t uint16; // primitive types are const by default
typedef const ::std::int32_t int32; // primitive types are const by default
typedef const ::std::uint32_t uint32; // primitive types are const by default
typedef const ::std::int64_t int64; // primitive types are const by default
typedef const ::std::uint64_t uint64; // primitive types are const by default


typedef const ::std::size_t size; // primitive types are const by default 
typedef const ::std::ptrdiff_t ptrdiff; // primitive types are const by default
typedef const ::std::intptr_t intptr; // primitive types are const by default
typedef const ::std::uintptr_t uintptr; // primitive types are const by default


#ifdef _FE_ON_WINDOWS_X86_64_
typedef const wchar_t directory_char_t;
#elif defined(_FE_ON_LINUX_X86_64_)
typedef const char directory_char_t;
#endif
using directory_string = std::pmr::basic_string<std::remove_const_t<directory_char_t>>;


template <typename T>
constexpr inline FE::size max_value = ::std::numeric_limits<T>::max();

template <typename T>
constexpr inline FE::size min_value = ::std::numeric_limits<T>::min();


_FE_MAYBE_UNUSED_	constexpr inline FE::size int8_max = max_value<FE::int8>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int16_max = max_value<FE::int16>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int32_max = max_value<FE::int32>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int64_max = max_value<FE::int64>;

_FE_MAYBE_UNUSED_	constexpr inline FE::size uint8_max = max_value<FE::uint8>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint16_max = max_value<FE::uint16>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint32_max = max_value<FE::uint32>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint64_max = max_value<FE::uint64>;

_FE_MAYBE_UNUSED_	constexpr inline FE::size int8_min = min_value<FE::int8>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int16_min = min_value<FE::int16>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int32_min = min_value<FE::int32>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size int64_min = min_value<FE::int64>;

_FE_MAYBE_UNUSED_	constexpr inline FE::size uint8_min = min_value<FE::uint8>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint16_min = min_value<FE::uint16>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint32_min = min_value<FE::uint32>;
_FE_MAYBE_UNUSED_	constexpr inline FE::size uint64_min = min_value<FE::uint64>;


#define _FE_TRUE_ 1
#define _FE_FALSE_ 0


#define _FE_FOUND_ 1
#define _FE_NOT_FOUND_ 0


#define _FE_SUCCEEDED_ 1
#define _FE_FAILED_ 0


_FE_MAYBE_UNUSED_ constexpr FE::float64 one_klick_f64 = 1000.0;
_FE_MAYBE_UNUSED_ constexpr FE::float32 one_klick_f32 = 1000.0f;

_FE_MAYBE_UNUSED_ constexpr FE::float64 one_meter_f64 = 1.0;
_FE_MAYBE_UNUSED_ constexpr FE::float32 one_meter_f32 = 1.0f;

_FE_MAYBE_UNUSED_ constexpr FE::float64 one_centimeter_f64 = 0.01;
_FE_MAYBE_UNUSED_ constexpr FE::float32 one_centimeter_f32 = 0.01f;

_FE_MAYBE_UNUSED_ constexpr FE::float64 one_millimeter_f64 = 0.001;
_FE_MAYBE_UNUSED_ constexpr FE::float32 one_millimeter_f32 = 0.001f;

_FE_MAYBE_UNUSED_ constexpr FE::float64 one_micrometer_f64 = 0.000001;
_FE_MAYBE_UNUSED_ constexpr FE::float32 one_micrometer_f32 = 0.000001f;


using null_t = uint8;
_FE_MAYBE_UNUSED_ inline constexpr null_t null = 0;


template <typename T>
class lazy_const
{
public:
	using value_type = typename std::remove_const<T>::type;
	using reference = value_type&;
	using const_reference = const value_type&;

private:
	value_type m_data;
	bool m_is_initialized;

public:
	constexpr lazy_const() noexcept : m_data(), m_is_initialized(false) {}
	constexpr lazy_const(value_type&& data_p) noexcept : m_data(std::move(data_p)), m_is_initialized(true) {}
	constexpr ~lazy_const() noexcept {};

	constexpr lazy_const(const lazy_const& other_p) noexcept : m_data(other_p.m_data), m_is_initialized(true) {}
	constexpr lazy_const(lazy_const&& rvalue_p) noexcept : m_data(std::move(rvalue_p.m_data)), m_is_initialized(true) {}

	constexpr lazy_const& operator=(value_type&& data_p) noexcept
	{
		if (m_is_initialized == true)
		{
			assert(false && "Frogman Engine C++: lazy_const cannot be set more than once.");
			std::exit(-9);
		}

		m_data = std::move(data_p);
		m_is_initialized = true;
		
		return *this;
	}

	constexpr lazy_const& operator=(const lazy_const& other_p) noexcept
	{
		if (m_is_initialized == true)
		{
			assert(false && "Frogman Engine C++: lazy_const cannot be set more than once.");
			std::exit(-9);
		}

		m_data = other_p.m_data;
		m_is_initialized = true;
		
		return *this;
	}

	constexpr lazy_const& operator=(lazy_const&& rvalue_p) noexcept
	{
		if (m_is_initialized == true)
		{
			assert(false && "Frogman Engine C++: lazy_const cannot be set more than once.");
			std::exit(-9);
		}

		m_data = std::move(rvalue_p.m_data);
		m_is_initialized = true;
		
		return *this;
	}

	_FE_FORCE_INLINE_ const_reference load() const noexcept { return m_data; }
};


class void_ptr
{
	void* m_ptr;
	std::type_index m_info;

public:
	_FE_FORCE_INLINE_ void_ptr() noexcept : m_ptr(), m_info(typeid(void*)) {}
	_FE_FORCE_INLINE_ ~void_ptr() noexcept {}


	_FE_FORCE_INLINE_ void_ptr(const void_ptr& other_p) noexcept : m_ptr(other_p.m_ptr), m_info(other_p.m_info) {}
	_FE_FORCE_INLINE_ void_ptr(void_ptr&& other_p) noexcept : m_ptr(other_p.m_ptr), m_info(other_p.m_info) {}


	_FE_FORCE_INLINE_ void_ptr& operator=(const void_ptr& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		m_info = other_p.m_info;
		return *this;
	}

	_FE_FORCE_INLINE_ void_ptr& operator=(void_ptr&& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		m_info = other_p.m_info;
		return *this;
	}


	_FE_FORCE_INLINE_ void_ptr(_FE_MAYBE_UNUSED_ std::nullptr_t value_p) noexcept : m_ptr(nullptr), m_info(typeid(std::nullptr_t)) {}

	_FE_FORCE_INLINE_ void_ptr& operator=(_FE_MAYBE_UNUSED_ std::nullptr_t value_p) noexcept
	{
		m_ptr = nullptr;
		m_info = typeid(std::nullptr_t);
		return *this;
	}


	_FE_FORCE_INLINE_ void_ptr(auto value_p) noexcept : m_ptr(value_p), m_info(typeid(decltype(value_p)))
	{
		static_assert(std::is_pointer<decltype(value_p)>::value == true);
	}

	_FE_FORCE_INLINE_ void_ptr& operator=(auto value_p) noexcept
	{
		m_ptr = value_p;
		m_info = typeid(decltype(value_p));
		return *this;
	}


	template<typename T>
	_FE_FORCE_INLINE_ T get() noexcept
	{
		static_assert(std::is_pointer<T>::value == true);
		assert(m_info == typeid(T));
		return static_cast<T>(m_ptr);
	}


	_FE_FORCE_INLINE_ bool operator==(void* ptr_p) const noexcept
	{
		return m_ptr == ptr_p;
	}

	_FE_FORCE_INLINE_ bool operator!=(void* ptr_p) const noexcept
	{
		return m_ptr != ptr_p;
	}

	_FE_FORCE_INLINE_ bool operator>(void* ptr_p) const noexcept
	{
		return m_ptr > ptr_p;
	}

	_FE_FORCE_INLINE_ bool operator>=(void* ptr_p) const noexcept
	{
		return m_ptr >= ptr_p;
	}

	_FE_FORCE_INLINE_ bool operator<(void* ptr_p) const noexcept
	{
		return m_ptr < ptr_p;
	}

	_FE_FORCE_INLINE_ bool operator<=(void* ptr_p) const noexcept
	{
		return m_ptr <= ptr_p;
	}
};


template <typename T>
class ref
{
	T* m_ptr = nullptr;

public:
	ref() noexcept : m_ptr(nullptr) {}
	~ref() noexcept {}
	ref(const ref& other_p) noexcept : m_ptr(other_p.m_ptr) {}
	ref(ref&& other_p) noexcept : m_ptr(other_p.m_ptr) { other_p.m_ptr = nullptr; }

	_FE_FORCE_INLINE_ ref& operator=(const ref& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		return *this;
	}
	_FE_FORCE_INLINE_ ref& operator=(ref&& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		other_p.m_ptr = nullptr;
		return *this;
	}

	ref(T& value_p) noexcept : m_ptr(&value_p) {}
	
	template<typename Child>
	ref(Child& value_p) noexcept : m_ptr(&value_p)
	{
		static_assert(std::is_base_of<T, Child>::value == true, "Child must be derived from T.");
	}

	_FE_FORCE_INLINE_ ref& operator=(T& value_p) noexcept
	{
		m_ptr = &value_p;
		return *this;
	}
	template<typename Child>
	_FE_FORCE_INLINE_ ref& operator=(Child& value_p) noexcept
	{
		m_ptr = &value_p;
		return *this;
	}

	_FE_FORCE_INLINE_ operator T& () noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be copied.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ operator const T& () const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be copied.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ T* operator&() noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return m_ptr;
	}
	_FE_FORCE_INLINE_ const T* operator&() const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return m_ptr;
	}
	_FE_FORCE_INLINE_ T& operator*() noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ const T& operator*() const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ bool operator==(null_t) const noexcept
	{
		return m_ptr == nullptr;
	}
	_FE_FORCE_INLINE_ bool operator!=(null_t) const noexcept
	{
		return m_ptr != nullptr;
	}
	_FE_FORCE_INLINE_ bool operator==(const ref& other_p) const noexcept
	{
		return m_ptr == other_p.m_ptr;
	}
	_FE_FORCE_INLINE_ bool operator!=(const ref& other_p) const noexcept
	{
		return m_ptr != other_p.m_ptr;
	}
};

// const_ref
template <typename T>
class const_ref
{
	const T* m_ptr = nullptr;

public:
	 const_ref() noexcept : m_ptr(nullptr) {}
	 ~const_ref() noexcept {}
	 const_ref(const const_ref& other_p) noexcept : m_ptr(other_p.m_ptr) {}
	 const_ref(const_ref&& other_p) noexcept : m_ptr(other_p.m_ptr) { other_p.m_ptr = nullptr; }

	_FE_FORCE_INLINE_ const_ref& operator=(const const_ref& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		return *this;
	}
	_FE_FORCE_INLINE_ const_ref& operator=(const_ref&& other_p) noexcept
	{
		m_ptr = other_p.m_ptr;
		other_p.m_ptr = nullptr;
		return *this;
	}

	const_ref(const T& value_p) noexcept : m_ptr(&value_p) {}

	template<typename Child>
	const_ref(Child& value_p) noexcept : m_ptr(&value_p)
	{
		static_assert(std::is_base_of<T, Child>::value == true, "Child must be derived from T.");
	}

	_FE_FORCE_INLINE_ const_ref& operator=(const T& value_p) noexcept
	{
		m_ptr = &value_p;
		return *this;
	}
	template<typename Child>
	_FE_FORCE_INLINE_ const_ref& operator=(Child& value_p) noexcept
	{
		m_ptr = &value_p;
		return *this;
	}

	_FE_FORCE_INLINE_ operator const T& () const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be copied.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ const T* operator&() const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return m_ptr;
	}
	_FE_FORCE_INLINE_ const T& operator*() const noexcept
	{
		assert(m_ptr != nullptr && "Frogman Engine C++: a null reference cannot be dereferenced.");
		return *m_ptr;
	}
	_FE_FORCE_INLINE_ bool operator==(null_t) const noexcept
	{
		return m_ptr == nullptr;
	}
	_FE_FORCE_INLINE_ bool operator!=(null_t) const noexcept
	{
		return m_ptr != nullptr;
	}
};


END_NAMESPACE


// variable types
namespace var 
{
	typedef bool boolean;
	static_assert(::std::atomic<boolean>::is_always_lock_free == true, "std::atomic is not compatible with boolean.");
	static_assert(sizeof(boolean) == 1, "The size of boolean must be one byte.");

	typedef char ASCII;
	static_assert(::std::atomic<ASCII>::is_always_lock_free == true, "std::atomic is not compatible with ASCII.");
	static_assert(sizeof(ASCII) == 1, "The size of ASCII must be one byte.");

	typedef wchar_t wchar;
	static_assert(::std::atomic<wchar>::is_always_lock_free == true, "std::atomic is not compatible with wchar.");

#ifdef _FE_HAS_CXX20_
	typedef char8_t UTF8;
	static_assert(::std::atomic<UTF8>::is_always_lock_free == true, "std::atomic is not compatible with UTF8.");
	static_assert(sizeof(UTF8) <= 1, "The size of UTF8 must be one byte.");
#endif

	typedef char16_t UTF16;
	static_assert(::std::atomic<UTF16>::is_always_lock_free == true, "std::atomic is not compatible with UTF16.");
	static_assert(sizeof(UTF16) == 2, "The size of UTF16 must be two bytes.");

	typedef char32_t UTF32;
	static_assert(::std::atomic<UTF32>::is_always_lock_free == true, "std::atomic is not compatible with UTF32.");
	static_assert(sizeof(UTF32) == 4, "The size of UTF32 must be four bytes.");

	typedef float float32;
	static_assert(::std::atomic<float32>::is_always_lock_free == true, "std::atomic is not compatible with float32.");
	static_assert(sizeof(float32) == 4, "The size of float32 must be four bytes.");

	typedef double float64;
	static_assert(::std::atomic<float64>::is_always_lock_free == true, "std::atomic is not compatible with float64.");
	static_assert(sizeof(float64) == 8, "The size of float64 must be eight bytes.");

	typedef ::std::uint8_t byte;
	typedef ::std::uint16_t word; // primitive types are const by default
	typedef ::std::uint32_t dword; // primitive types are const by default
	typedef ::std::uint64_t qword; // primitive types are const by default

	typedef ::std::int8_t int8;
	static_assert(::std::atomic<int8>::is_always_lock_free == true, "std::atomic is not compatible with int8.");
	static_assert(sizeof(int8) == 1, "The size of int8 must be one byte.");

	typedef ::std::uint8_t uint8;
	static_assert(::std::atomic<uint8>::is_always_lock_free == true, "std::atomic is not compatible with uint8.");
	static_assert(sizeof(uint8) == 1, "The size of uint8 must be one byte.");

	typedef ::std::int16_t int16;
	static_assert(::std::atomic<int16>::is_always_lock_free == true, "std::atomic is not compatible with int16.");
	static_assert(sizeof(int16) == 2, "The size of int16 must be two bytes.");

	typedef ::std::uint16_t uint16;
	static_assert(::std::atomic<uint16>::is_always_lock_free == true, "std::atomic is not compatible with uint16.");
	static_assert(sizeof(uint16) == 2, "The size of uint16 must be two bytes.");

	typedef ::std::int32_t int32;
	static_assert(::std::atomic<int32>::is_always_lock_free == true, "std::atomic is not compatible with int32.");
	static_assert(sizeof(int32) == 4, "The size of int32 must be four bytes.");

	typedef ::std::uint32_t uint32;
	static_assert(::std::atomic<uint32>::is_always_lock_free == true, "std::atomic is not compatible with uint32.");
	static_assert(sizeof(uint32) == 4, "The size of uint32 must be four bytes.");

	typedef ::std::int64_t int64;
	static_assert(::std::atomic<int64>::is_always_lock_free == true, "std::atomic is not compatible with int64.");
	static_assert(sizeof(int64) == 8, "The size of int64 must be eight bytes.");

	typedef ::std::uint64_t uint64;
	static_assert(::std::atomic<uint64>::is_always_lock_free == true, "std::atomic is not compatible with uint64.");
	static_assert(sizeof(uint64) == 8, "The size of uint64 must be eight bytes.");

	typedef ::std::size_t size;
	typedef ::std::ptrdiff_t ptrdiff;
	typedef ::std::intptr_t intptr;
	typedef ::std::uintptr_t uintptr;

	#ifdef _FE_ON_WINDOWS_X86_64_
		typedef wchar_t directory_char_t;
	#elif defined(_FE_ON_LINUX_X86_64_)
		typedef char directory_char_t;
	#endif
}

#endif