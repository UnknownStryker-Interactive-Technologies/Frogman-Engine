#ifndef _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HXX_
#define _FE_FRAMEWORK_REFLECTION_TYPE_INFO_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/definitions.hxx>
#include <FE/do_once.hxx>
#include <FE/hash.hxx>
#include <FE/memory.hxx>
#include <FE/types.hxx>
#include <FE/type_traits.hxx>

// std
#include <memory>
#include <memory_resource>
#include <string>
#include <string_view>
#include <shared_mutex>
#include <typeinfo>

// boost
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <absl/container/node_hash_map.h>

// windows
#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <DbgHelp.h>
#endif




CLASS_FORWARD_DECLARATION(FE, component_base);


BEGIN_NAMESPACE(FE::framework::reflection)


namespace internal::type_info
{
    class metadata
    {
    public:
        using string_type = std::pmr::string;

        thread_local static std::shared_ptr<std::pmr::unsynchronized_pool_resource> tl_s_resource;
		static std::atomic_uint64_t s_type_id_counter;

        string_type _typename;
        string_type _base_typename;
        std::size_t _hashed_name = 0;
		std::size_t _hashed_base_name = 0;
		var::uint64 _component_type_id = 0;
    };
}


class type_info
{
    using table_type = absl::node_hash_map<typename internal::type_info::metadata::string_type, internal::type_info::metadata,
                                            FE::hash<typename internal::type_info::metadata::string_type>,
                                            std::equal_to<typename internal::type_info::metadata::string_type>,
		                                    FE::polymorphic_allocator< std::pair<const typename internal::type_info::metadata::string_type, internal::type_info::metadata> >
    >;
    using lock_type = std::shared_mutex;

	std::shared_ptr<std::pmr::unsynchronized_pool_resource> m_resource;
    internal::type_info::metadata m_info;
   
    thread_local static table_type tl_s_type_information;

public:
    type_info() noexcept
        : m_info()
    {
        if (internal::type_info::metadata::tl_s_resource == nullptr)
        {
			internal::type_info::metadata::tl_s_resource = std::make_shared<std::pmr::unsynchronized_pool_resource>();
        }
		m_resource = internal::type_info::metadata::tl_s_resource;
    }
	~type_info() noexcept = default;
   
private:
    void __demangle_type_name(std::pmr::string& out_ret_p, const char* mangled_name_p) noexcept
    {
        // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolname
        var::ASCII l_buffer[1024] = { 0 };
        _FE_MAYBE_UNUSED_ DWORD l_result = UnDecorateSymbolName(mangled_name_p, l_buffer, sizeof(l_buffer), UNDNAME_COMPLETE);
        FE_NEGATIVE_ASSERT((l_result == 0), "UnDecorateSymbolName() operation unsuccessful.");
        out_ret_p = typename internal::type_info::metadata::string_type( static_cast<var::ASCII*>(l_buffer), m_resource.get() );
    }

    template <typename T>
    void set() noexcept
    {
        __demangle_type_name( m_info._typename, typeid(T).name() );
		m_info._hashed_name = robin_hood::hash_bytes(m_info._typename.data(), m_info._typename.length());

        if constexpr (FE::has_base_type<T>::value == true)
        { 
            __demangle_type_name( m_info._base_typename, typeid(typename T::base_type).name() );
			m_info._hashed_base_name = robin_hood::hash_bytes(m_info._base_typename.data(), m_info._base_typename.length());
        }

        if constexpr (std::is_base_of_v<FE::component_base, T> == true)
        {
            ++internal::type_info::metadata::s_type_id_counter;
            m_info._component_type_id = internal::type_info::metadata::s_type_id_counter;
        }

        type_info::tl_s_type_information.emplace(m_info._typename, m_info);
    }

public:
    _FE_FORCE_INLINE_ FE::ASCII* name() const noexcept
    {
        return m_info._typename.c_str();
    }

    _FE_FORCE_INLINE_ std::size_t hash_code() const noexcept
    {
		return m_info._hashed_name;
    }

    _FE_FORCE_INLINE_ FE::ASCII* base_name() const noexcept
    {
		return m_info._base_typename.c_str();
    }

    _FE_FORCE_INLINE_ std::size_t base_hash_code() const noexcept
    {
		return m_info._hashed_base_name;
    }

    _FE_FORCE_INLINE_ var::uint64 component_typeid() const noexcept
    {
        FE_ASSERT(m_info._component_type_id > 0, "Assertion failed: the current type is not a valid component type.");
		return m_info._component_type_id;
    }

    static FE::ASCII* get_base_name_of(const std::string_view& this_type_name_p) noexcept
    {
        thread_local static typename internal::type_info::metadata::string_type tl_s_buff;
        tl_s_buff = this_type_name_p;
        auto l_result = type_info::tl_s_type_information.find(tl_s_buff);
        if (l_result != type_info::tl_s_type_information.end()) _FE_LIKELY_
        {
            return l_result->second._base_typename.c_str();
        }
        FE_LOG(FE::log::Severity::_Warning, "Warning: Frogman Engine RTTI get_base_name_of() method_registry is returning nullptr. Please check if the type is registered to this RTTI system.");
        return "\0";
    }

    template<typename T>
    _FE_FORCE_INLINE_ friend type_info& type_id() noexcept;
};

template<typename T>
_FE_FORCE_INLINE_ type_info& type_id() noexcept
{
    thread_local static type_info tl_s_type_info;
    FE_DO_ONCE(_DO_ONCE_PER_THREAD_, tl_s_type_info.set<T>(););
    return tl_s_type_info;
}

END_NAMESPACE
#endif