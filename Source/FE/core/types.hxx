#ifndef _FE_CORE_TYPES_HXX_
#define _FE_CORE_TYPES_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/miscellaneous/configuration.h>
#include <FE/core/macros/attributes.h>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <limits>
#include <typeinfo>
#include <type_traits>
#include <utility>
#pragma warning(disable: 4530) // disable no-exception warnings
#ifdef _MSVC_
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#endif




BEGIN_NAMESPACE(FE)


typedef const bool boolean; // primitive types are const by default

typedef const char character;  // primitive types are const by default
typedef const signed char schar; // primitive types are const by default
typedef const unsigned char uchar; // primitive types are const by default
typedef const wchar_t wchar; // primitive types are const by default


#ifdef _HAS_CXX20_
typedef const char8_t UTF8; // primitive types are const by default
#endif

typedef const char16_t UTF16; // primitive types are const by default
typedef const char32_t UTF32; // primitive types are const by default


typedef const float float32; // primitive types are const by default
typedef const double float64; // primitive types are const by default


typedef const bool binary; // primitive types are const by default
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


typedef const ::std::size_t size_t; // primitive types are const by default 
typedef const ::std::size_t length_t; // primitive types are const by default 
typedef const ::std::size_t index_t; // primitive types are const by default
typedef const ::std::size_t count_t; // primitive types are const by default
typedef const ::std::size_t capacity_t; // primitive types are const by default
typedef const ::std::ptrdiff_t ptrdiff_t; // primitive types are const by default
typedef const ::std::intptr_t intptr_t; // primitive types are const by default
typedef const ::std::uintptr_t uintptr_t; // primitive types are const by default


	// variable types
	namespace var 
	{
		typedef bool boolean;
		static_assert(::std::atomic<boolean>::is_always_lock_free == true, "std::atomic is not compatible with boolean.");
		static_assert(sizeof(boolean) == 1, "The size of boolean must be one byte.");

		typedef char character;
		static_assert(::std::atomic<character>::is_always_lock_free == true, "std::atomic is not compatible with character.");
		static_assert(sizeof(character) == 1, "The size of character must be one byte.");

		typedef signed char schar;
		static_assert(::std::atomic<schar>::is_always_lock_free == true, "std::atomic is not compatible with schar.");
		static_assert(sizeof(schar) == 1, "The size of schar must be one byte.");

		typedef unsigned char uchar;
		static_assert(::std::atomic<uchar>::is_always_lock_free == true, "std::atomic is not compatible with uchar.");
		static_assert(sizeof(uchar) == 1, "The size of uchar must be one byte.");

		typedef wchar_t wchar;
		static_assert(::std::atomic<wchar>::is_always_lock_free == true, "std::atomic is not compatible with wchar.");
		static_assert(sizeof(wchar) <= 4, "The size of wchar must be less than or equal to four bytes.");

#ifdef _HAS_CXX20_
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

		typedef bool binary;
		static_assert(sizeof(binary) == 1, "The size of binary must be one byte.");

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

		typedef ::std::size_t size_t;
		typedef ::std::size_t length_t;
		typedef ::std::size_t index_t;
		typedef ::std::size_t count_t;
		typedef ::std::size_t capacity_t;
		typedef ::std::ptrdiff_t ptrdiff_t;
		typedef ::std::intptr_t intptr_t;
		typedef ::std::uintptr_t uintptr_t;
	}


	template <typename T>
	constexpr inline auto max_value = ::std::numeric_limits<T>::max();

	template <typename T>
	constexpr inline auto min_value = ::std::numeric_limits<T>::min();

_MAYBE_UNUSED_	constexpr inline FE::int8 int8_max = max_value<FE::int8>;
_MAYBE_UNUSED_	constexpr inline FE::int16 int16_max = max_value<FE::int16>;
_MAYBE_UNUSED_	constexpr inline FE::int32 int32_max = max_value<FE::int32>;
_MAYBE_UNUSED_	constexpr inline FE::int64 int64_max = max_value<FE::int64>;
	
_MAYBE_UNUSED_	constexpr inline FE::uint8 uint8_max = max_value<FE::uint8>;
_MAYBE_UNUSED_	constexpr inline FE::uint16 uint16_max = max_value<FE::uint16>;
_MAYBE_UNUSED_	constexpr inline FE::uint32 uint32_max = max_value<FE::uint32>;
_MAYBE_UNUSED_	constexpr inline FE::uint64 uint64_max = max_value<FE::uint64>;

_MAYBE_UNUSED_	constexpr inline FE::int8 int8_min = min_value<FE::int8>;
_MAYBE_UNUSED_	constexpr inline FE::int16 int16_min = min_value<FE::int16>;
_MAYBE_UNUSED_	constexpr inline FE::int32 int32_min = min_value<FE::int32>;
_MAYBE_UNUSED_	constexpr inline FE::int64 int64_min = min_value<FE::int64>;

_MAYBE_UNUSED_	constexpr inline FE::uint8 uint8_min = min_value<FE::uint8>;
_MAYBE_UNUSED_	constexpr inline FE::uint16 uint16_min = min_value<FE::uint16>;
_MAYBE_UNUSED_	constexpr inline FE::uint32 uint32_min = min_value<FE::uint32>;
_MAYBE_UNUSED_	constexpr inline FE::uint64 uint64_min = min_value<FE::uint64>;


#define _NULL_ 0
#define _NULL_f_ 0.0f


#define _TRUE_ 1
#define _FALSE_ 0


#define _FOUND_ true
#define _NOT_FOUND_ false


#define _FE_SUCCESS_ true
#define _FE_FAILED_ false


struct null {};


template <typename T>
class lazy_const
{
public:
	using value_type = typename std::remove_const<T>::type;
	using reference = value_type&;
	using const_reference = const value_type&;

private:
	value_type m_data;
	var::boolean m_is_initialized;

public:
	_CONSTEXPR17_ lazy_const() noexcept : m_data(), m_is_initialized(false) {}
	_CONSTEXPR17_ lazy_const(value_type&& data_p) noexcept : m_data(std::move(data_p)), m_is_initialized(true) {}
	_CONSTEXPR20_ ~lazy_const() noexcept {};

	_CONSTEXPR20_ _FORCE_INLINE_ lazy_const(const lazy_const& other_p) noexcept : m_data(other_p.m_data), m_is_initialized(true) {}
	_CONSTEXPR20_ _FORCE_INLINE_ lazy_const(lazy_const&& rvalue_p) noexcept : m_data(std::move(rvalue_p.m_data)), m_is_initialized(true) {}

	_FORCE_INLINE_ lazy_const& operator=(value_type&& data_p) noexcept
	{
		assert(this->m_is_initialized == false);

		this->m_data = std::move(data_p);
		this->m_is_initialized = true;
		
		return *this;
	}

	_FORCE_INLINE_ lazy_const& operator=(const lazy_const& other_p) noexcept
	{
		assert(this->m_is_initialized == false);

		this->m_data = other_p.m_data;
		this->m_is_initialized = true;
		
		return *this;
	}

	_FORCE_INLINE_ lazy_const& operator=(lazy_const&& rvalue_p) noexcept
	{
		assert(this->m_is_initialized == false);

		this->m_data = std::move(rvalue_p.m_data);
		this->m_is_initialized = true;
		
		return *this;
	}

	_FORCE_INLINE_ const_reference load() const noexcept { return this->m_data; }
};


namespace container
{
	// unsafe_ref is a non-deletable pointer class template.
	template<typename T>
	class unsafe_ref
	{
		T* m_ref_ptr;

	public:

		_CONSTEXPR17_ _FORCE_INLINE_ unsafe_ref() noexcept : m_ref_ptr() {}
		_CONSTEXPR17_ _FORCE_INLINE_ ~unsafe_ref() noexcept {}

		_CONSTEXPR17_ _FORCE_INLINE_ unsafe_ref(T& ref_p) noexcept : m_ref_ptr(&ref_p) {}

		_CONSTEXPR17_ _FORCE_INLINE_ unsafe_ref(const unsafe_ref& ref_p) noexcept : m_ref_ptr(ref_p.m_ref_ptr) {}
		_CONSTEXPR17_ _FORCE_INLINE_ unsafe_ref(unsafe_ref&& ref_p) noexcept : m_ref_ptr(ref_p.m_ref_ptr) { ref_p.m_ref_ptr = nullptr; }

		_FORCE_INLINE_ unsafe_ref& operator=(T& ref_p) noexcept
		{
			this->m_ref_ptr = &ref_p;
			return *this;
		}

		_FORCE_INLINE_ unsafe_ref& operator=(unsafe_ref&& ref_p) noexcept
		{
			this->m_ref_ptr = ref_p.m_ref_ptr;
			ref_p.m_ref_ptr = nullptr;
			return *this;
		}

		_FORCE_INLINE_ explicit operator bool() const noexcept
		{
			return this->m_ref_ptr != nullptr;
		}

		_FORCE_INLINE_ bool operator!() const noexcept
		{
			return this->m_ref_ptr == nullptr;
		}

		_FORCE_INLINE_ operator T& () const noexcept
		{
			return *this->m_ref_ptr;
		}

		_FORCE_INLINE_ boolean is_null() const noexcept
		{
			return this->m_ref_ptr != nullptr;
		}

		_FORCE_INLINE_ T& get() const noexcept
		{
			return *(this->m_ref_ptr);
		}
	};

	template <class Container>
	struct range
	{
		unsafe_ref<Container> _container;
		typename Container::size_type _begin;
		typename Container::size_type _end;
	};
}


template<typename T>
class buffer final
{
	thread_local static T tl_s_rvalue_buffer;

public:
	using value_type = T;
	using reference = T&;
	using rvalue_reference = T&&;

	_CONSTEXPR20_ _FORCE_INLINE_ static void set(rvalue_reference rvalue_p) noexcept
	{
		tl_s_rvalue_buffer = std::move(rvalue_p);
	}

	_CONSTEXPR20_ _FORCE_INLINE_ static reference set_and_get(rvalue_reference rvalue_p) noexcept
	{
		tl_s_rvalue_buffer = std::move(rvalue_p);
		return tl_s_rvalue_buffer;
	}

	_CONSTEXPR20_ _FORCE_INLINE_ static reference get() noexcept
	{
		return tl_s_rvalue_buffer;
	}
};

template<typename T>
thread_local T buffer<T>::tl_s_rvalue_buffer;




namespace internal
{
	struct do_once
	{
		template<typename ExecutableCode>
		_FORCE_INLINE_ do_once(ExecutableCode code_p) noexcept
		{
			code_p();
		}
	};
}
#ifdef _DO_ONCE_AT_APP_PROCESS_
#error _DO_ONCE_AT_APP_PROCESS_ is a reserved Frogman Engine macro keyword.
#endif 
// A directive for executing a given code once per an application launch.
#define _DO_ONCE_AT_APP_PROCESS_ static


#ifdef _DO_ONCE_AT_THREAD_PROCESS_
#error _DO_ONCE_AT_THREAD_PROCESS_ is a reserved Frogman Engine macro keyword.
#endif 
// A directive for executing a given code once per thread.
#define _DO_ONCE_AT_THREAD_PROCESS_ thread_local static 


#ifdef FE_DO_ONCE
#error FE_DO_ONCE is a reserved Frogman Engine macro keyword.
#endif
#define FE_DO_ONCE(frequency, code) { frequency ::FE::internal::do_once __FE_DO_ONCE_INSTANCE__(code); }


END_NAMESPACE


#ifndef let
// let is a reserved experimental Frogman Engine keyword and can be removed without notifications.
#define let ::FE::
#else
#error let is a reserved experimental Frogman Engine keyword.
#endif


//#ifdef _MSVC_
//#ifndef max
//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif
//#ifndef min
//#define min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif
//#endif
#endif