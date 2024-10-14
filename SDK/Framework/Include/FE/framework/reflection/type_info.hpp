#ifndef _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HPP_
#define _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/allocator.hxx>
#include <FE/definitions.h>
#include <FE/do_once.hxx>
#include <FE/types.hxx>
#include <FE/type_traits.hxx>

// std
#include <memory>
#include <memory_resource>
#include <string>
#include <shared_mutex>
#include <typeinfo>

// boost
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>

// windows
#ifdef _WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>
#endif




BEGIN_NAMESPACE(FE::framework::reflection)


namespace internal::type_info
{
    class name
    {
    public:
        using string_type = std::pmr::basic_string<char, std::char_traits<char>>;

		static std::pmr::monotonic_buffer_resource s_resource;

        string_type _typename{ std::pmr::polymorphic_allocator<char>{&s_resource} };
        string_type _base_typename{ std::pmr::polymorphic_allocator<char>{&s_resource} };
    };

    struct c_style_deleter
    {
        void operator()(void* p) const noexcept
        {
            free(p);
        }
    };
}


class type_info
{
    using table_type = robin_hood::unordered_map<typename internal::type_info::name::string_type, internal::type_info::name>;
    using lock_type = std::shared_mutex;

    typename internal::type_info::name::string_type m_name;

    static table_type s_type_information;
    static lock_type s_lock;
   

    template<class String>
    _FE_FORCE_INLINE_ static String __demangle_type_name(const char* mangled_name_p) noexcept
    {
        FE_STATIC_ASSERT(std::is_class<String>::value == false, "Static Assertion failed: the template argument String must not be a string class type.");

#ifdef _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_
        // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
	    int l_result = 0;
		var::size_t l_length = 0;
	    std::unique_ptr<char[], internal::type_info::c_style_deleter> l_buffer = abi::__cxa_demangle(mangled_name_p, nullptr, &l_length, &l_result);
        FE_ASSERT((l_result != 0), "abi::__cxa_demangle() operation unsuccessful: abi::__cxa_demangle() returned the error code '${%d@0}'.\nNOTE:\n0: The demangling operation succeeded.\n-1: A memory allocation failiure occurred.\n-2: mangled_name is not a valid name under the C++ ABI mangling rules.\n-3: One of the arguments is invalid.", &l_result);
        FE_ASSERT((l_length >= 1024), "abi::__cxa_demangle() operation unsuccessful: the string length exceeds the limitation. The demangled name must be shorter than 1024 characters.");
        return l_buffer.get();
#else
        // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolname
        var::ASCII l_buffer[1024] = { 0 };
        _FE_MAYBE_UNUSED_ DWORD l_result = UnDecorateSymbolName(mangled_name_p, l_buffer, sizeof(l_buffer), UNDNAME_COMPLETE);
        FE_ASSERT((l_result == 0), "UnDecorateSymbolName() operation unsuccessful.");
        return static_cast<var::ASCII*>(l_buffer);
#endif
    }

public:
    template<typename T> // Do not manually call this function!
    _FE_FORCE_INLINE_ void set() noexcept
    {
        internal::type_info::name l_info; 
        l_info._typename = __demangle_type_name<typename internal::type_info::name::string_type>( typeid(T).name() ); 
        if constexpr (FE::has_base_type<T>::value == true)
        { 
            l_info._base_typename = __demangle_type_name<typename internal::type_info::name::string_type>( typeid(typename T::base_type).name() );
        }

        std::lock_guard<lock_type> l_lock(s_lock);
        this->m_name = l_info._typename;
        type_info::s_type_information.emplace(this->m_name, std::move(l_info));
    }

    _FE_FORCE_INLINE_ FE::ASCII* name() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(this->m_name)->second._typename.c_str();
    }

    _FE_FORCE_INLINE_ FE::uint64 hash_code() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        typename internal::type_info::name::string_type& l_typename = type_info::s_type_information.find(this->m_name)->second._typename;
        return robin_hood::hash_bytes(l_typename.data(), l_typename.length());
    }

    _FE_FORCE_INLINE_ FE::ASCII* base_name() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(this->m_name)->second._base_typename.c_str();
    }

    _FE_FORCE_INLINE_ FE::uint64 base_hash_code() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        typename internal::type_info::name::string_type& l_typename = type_info::s_type_information.find(this->m_name)->second._base_typename;
        return robin_hood::hash_bytes(l_typename.data(), l_typename.length());
    }

    _FE_FORCE_INLINE_ static FE::ASCII* get_base_name_of(FE::ASCII* this_type_name_p) noexcept
    {
        thread_local static typename internal::type_info::name::string_type l_typename;
        l_typename = this_type_name_p;
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(l_typename)->second._base_typename.c_str();
    }
};

template<typename T>
_FE_FORCE_INLINE_ type_info& type_id() noexcept
{
    static type_info l_type_info;
    FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, l_type_info.set<T>(););
    return l_type_info;
}

END_NAMESPACE
#endif