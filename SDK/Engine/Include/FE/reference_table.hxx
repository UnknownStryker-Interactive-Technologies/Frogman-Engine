#ifndef _FROGMAN_ENGINE_REFERENCE_TABLE_HXX_
#define _FROGMAN_ENGINE_REFERENCE_TABLE_HXX_
#pragma warning(push)
#pragma warning(disable: 4324)
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
#include <FE/farray.hxx>
#include <FE/list.hxx>
#include <FE/memory.hxx>
#include <FE/processors.hxx>
#include <FE/thread_id.hxx>
#include <FE/framework/fiber.hxx>

#include <FE/gc_base.hpp>
#include <FE/gc_ptr.hpp>

#include <algorithm>
#include <cmath>
#include <memory_resource>
#include <new>
#include <shared_mutex>
#include <span>

#include <absl/container/flat_hash_map.h>

#include <concurrent_queue.h>




BEGIN_NAMESPACE(FE)


class reference_table
{
    template <typename T, typename... Arguments>
    friend FE::gc_ptr<T> _FE_VECTOR_CALL_ gcnew(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept;

public:
    constexpr static FE::size page_size_in_bytes = 4096;
    constexpr static FE::size references_per_page = (page_size_in_bytes - (2 * sizeof(void*)) - sizeof(var::size)) / sizeof(internal::gc_ptr::metadata*);
    static_assert(sizeof(typename FE::list< FE::farray<internal::gc_ptr::metadata*, references_per_page> >::allocator_type::value_type) == page_size_in_bytes, "Static assertion failed: a list node of FE::reference_table must be exactly 4 KiB.");

private:
    struct alignas(FE::CPU_L1_cache_line::size) thread_reference_table
    {
        FE::list<FE::farray<internal::gc_ptr::metadata*, references_per_page>> _references;

        explicit thread_reference_table(std::pmr::memory_resource* const resource_p) noexcept;
    };

    struct suspect
    {
        internal::gc_ptr::metadata* _metadata = nullptr;
        var::size _type_hash = 0;
    };

    using gc_ptr_offset_vector = std::pmr::vector<var::int64>;
    using gc_ptr_offset_registry_type =
        absl::flat_hash_map<var::size, gc_ptr_offset_vector,
        absl::DefaultHashContainerHash<var::size>, absl::DefaultHashContainerEq<var::size>,
        std::pmr::polymorphic_allocator< std::pair<const var::size, gc_ptr_offset_vector> >
        >;

    FE::int32 m_max_concurrency;
    FE::unique_ptr<thread_reference_table[]> m_reference_tables;
    std::shared_mutex m_gc_ptr_offset_registry_lock;
    gc_ptr_offset_registry_type m_gc_ptr_offset_registry;
	std::jthread m_cyclic_reference_inspector;
	FE::anesthesia m_cyclic_reference_inspector_anesthesia;
	concurrency::concurrent_queue< suspect, std::pmr::polymorphic_allocator<suspect> > m_suspected_cyclic_reference_queue;

public:
    explicit reference_table(FE::int32 max_concurrency_p, std::pmr::memory_resource* const resource_p) noexcept;
    ~reference_table() noexcept;

    reference_table(const reference_table&) = delete;
    reference_table& operator=(const reference_table&) = delete;

    void collect_garbages(FE::float64 percentage_p) noexcept;

private:
    void __investigate_cyclic_references() noexcept;

    static void __collect(internal::gc_ptr::metadata* const metadata_p) noexcept;
    void __collect_cyclic_garbage(internal::gc_ptr::metadata* const metadata_p) noexcept;
    void __push_back(internal::gc_ptr::metadata* const metadata_p) noexcept;

    template <class T>
    void __register_gc_ptr_offsets(FE::size type_hash_p) noexcept
    {
        gc_ptr_offset_vector l_offsets(m_gc_ptr_offset_registry.get_allocator().resource());
        __append_gc_ptr_offsets<T>(l_offsets);

        std::unique_lock<std::shared_mutex> l_lock(m_gc_ptr_offset_registry_lock);
        m_gc_ptr_offset_registry.try_emplace(type_hash_p, std::move(l_offsets));
    }

    template <class T>
    static void __append_gc_ptr_offsets(gc_ptr_offset_vector& out_offsets_p) noexcept
    {
        const gc_ptr_offset_vector& l_own_offsets = internal::gc_ptr::own_gc_ptr_offsets<T>();
        out_offsets_p.insert(out_offsets_p.end(), l_own_offsets.begin(), l_own_offsets.end());

        if constexpr (FE::has_base_type<T>::value == true)
        {
            if constexpr ((std::is_base_of_v<FE::gc_base, typename T::base_type> == true) && (std::is_same_v<T, typename T::base_type> == false))
            {
                __append_gc_ptr_offsets<typename T::base_type>(out_offsets_p);
            }
        }
    }

    std::span<const var::int64> __find_gc_ptr_offsets(FE::size type_hash_p) noexcept
    {
        std::shared_lock<std::shared_mutex> l_lock(m_gc_ptr_offset_registry_lock);
        auto l_search_result = m_gc_ptr_offset_registry.find(type_hash_p);
        if (l_search_result == m_gc_ptr_offset_registry.end())
        {
            return {};
        }
        return { l_search_result->second.data(), l_search_result->second.size() };
    }
};


END_NAMESPACE

#pragma warning(pop)
#endif
