#ifndef _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HPP_
#define _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/macros/definitions.h>
#include <FE/core/allocator.hxx>
#include <FE/core/do_once.hxx>
#include <FE/core/hash.hpp>
#include <FE/core/log/log.h>
#include <FE/core/types.hxx>
#include <FE/core/type_traits.hxx>
#include <FE/core/managed/unique_ptr.hxx>

// std
#include <string>
#include <shared_mutex>
#include <typeinfo>

// boost
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>




BEGIN_NAMESPACE(FE::framework::reflection)


namespace internal::type_info
{
    struct name
    {
        using string_type = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
        string_type _typename;
        string_type _base_typename;
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
    _FORCE_INLINE_ static String __demangle_type_name(const char* mangled_name_p) noexcept
    {
#ifdef _FROGMAN_ENGINE_CORE_HAS_CXXABI_H_
	    FE_STATIC_ASSERT(std::is_class<String>::value == false, "Static Assertion failed: the template argument String must not be a string class type.");
	
	    int l_result = 0;
	    char* const l_demangled_typename = abi::__cxa_demangle(mangled_name_p, nullptr, nullptr, &l_result);
	    FE_ASSERT((l_result != 0), "abi::__cxa_demangle() operation unsuccessful: abi::__cxa_demangle() returned the error code '${%d@0}'.\nNOTE:\n0: The demangling operation succeeded.\n-1: A memory allocation failiure occurred.\n-2: mangled_name is not a valid name under the C++ ABI mangling rules.\n-3: One of the arguments is invalid.", &l_result);
	    String l_buffer = l_demangled_typename; // This line of code never throws any exceptions. In fact, Frogman Engine does not like throwing exceptions, and it always adheres to fail-fast strategy.
	    free(l_demangled_typename);
	    return l_buffer;
#else
    // For MSVC:
    // https://stackoverflow.com/questions/13777681/demangling-in-msvc
    // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolname
    // https://github.com/wine-mirror/wine/blob/master/dlls/msvcrt/undname.c 
        return mangled_name_p;
#endif
    }

public:
    template<typename T> // Do not manually call this function!
    _FORCE_INLINE_ void set() noexcept
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

    _FORCE_INLINE_ FE::ASCII* name() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(this->m_name)->second._typename.c_str();
    }

    _FORCE_INLINE_ FE::uint64 hash_code() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        typename internal::type_info::name::string_type& l_typename = type_info::s_type_information.find(this->m_name)->second._typename;
        return robin_hood::hash_bytes(l_typename.data(), l_typename.length());
    }

    _FORCE_INLINE_ FE::ASCII* base_name() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(this->m_name)->second._base_typename.c_str();
    }

    _FORCE_INLINE_ FE::uint64 base_hash_code() const noexcept
    {
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        typename internal::type_info::name::string_type& l_typename = type_info::s_type_information.find(this->m_name)->second._base_typename;
        return robin_hood::hash_bytes(l_typename.data(), l_typename.length());
    }

    _FORCE_INLINE_ static FE::ASCII* get_base_name_of(FE::ASCII* this_type_name_p) noexcept
    {
        thread_local static typename internal::type_info::name::string_type l_typename;
        l_typename = this_type_name_p;
        boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
        return type_info::s_type_information.find(l_typename)->second._base_typename.c_str();
    }
};

template<typename T>
_FORCE_INLINE_ type_info& type_id() noexcept
{
    static type_info l_type_info;
    FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, l_type_info.set<T>(););
    return l_type_info;
}

END_NAMESPACE
#endif
