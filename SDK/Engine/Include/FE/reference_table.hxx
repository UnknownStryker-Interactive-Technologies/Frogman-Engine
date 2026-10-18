#ifndef _FROGMAN_ENGINE_REFERENCE_TABLE_HXX
#define _FROGMAN_ENGINE_REFERENCE_TABLE_HXX
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
#include <FE/thread_id.hxx>
#include <FE/framework/fiber.hxx>

#include <FE/gc_base.hpp>
#include <FE/gc_ptr.hpp>

#include <algorithm>
#include <cmath>
#include <memory_resource>
#include <new>

#include <concurrent_queue.h>




BEGIN_NAMESPACE(FE)


class reference_table
{
    template <typename T, typename... Arguments>
    friend FE::gc_ptr<T> _FE_VECTOR_CALL_ gcnew(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept;

public:
    constexpr static FE::size page_size_in_bytes = 4096;
    constexpr static FE::size references_per_page = (page_size_in_bytes - (2 * sizeof(void*)) - sizeof(var::size)) / sizeof(internal::gc_ptr::metadata*);
    static_assert(sizeof(FE::list<FE::farray<internal::gc_ptr::metadata*, references_per_page>>::allocator_type::value_type) == page_size_in_bytes, "Static assertion failed: a list node of FE::reference_table must be exactly 4 KiB.");

private:
    struct alignas(FE::CPU_L1_cache_line::size) thread_reference_table
    {
        FE::list<FE::farray<internal::gc_ptr::metadata*, references_per_page>> _references;
        var::size _gc_cursor;

        explicit thread_reference_table(std::pmr::memory_resource* const resource_p) noexcept;
    };

    FE::int32 m_max_concurrency;
    FE::unique_ptr<thread_reference_table[]> m_reference_tables;
    
    std::thread m_cyclic_reference_killer;
	concurrency::concurrent_queue<internal::gc_ptr::metadata*, std::pmr::polymorphic_allocator<internal::gc_ptr::metadata*>> m_cyclic_reference_death_note;
	
    std::atomic_bool m_should_kill_cyclic_references;
    std::mutex m_killer_sleeper;
    std::condition_variable m_condition_variable;

public:
    explicit reference_table(FE::int32 max_concurrency_p, std::pmr::memory_resource* const resource_p) noexcept;

    reference_table(const reference_table&) = delete;
    reference_table& operator=(const reference_table&) = delete;

    void collect_garbage(FE::float64 percentage_p) noexcept;
	void investigate_cyclic_references(internal::gc_ptr::metadata* const metadata_p, FE::uint32 max_steps_p) noexcept;
	void fork_cyclic_reference_killer() noexcept;
	void join_cyclic_reference_killer() noexcept;

private:
    _FE_FORCE_INLINE_ void __cyclic_reference_killer_main() noexcept
    {
        while (m_should_kill_cyclic_references.load(std::memory_order_acquire) == true)
        {
            internal::gc_ptr::metadata* l_convict = nullptr;
            if (m_cyclic_reference_death_note.try_pop(l_convict))
            {
                __collect(l_convict);
                continue;
            }
            std::unique_lock<std::mutex> l_lock(m_killer_sleeper);
            m_condition_variable.wait(l_lock);
        }
    }

    _FE_FORCE_INLINE_ void __push_back(internal::gc_ptr::metadata* const metadata_p) noexcept
    {
        FE_ASSERT(metadata_p != nullptr, "Assertion failed: cannot push back a null gc object.");
        FE::uint16 l_thread_id = FE::get_current_thread_id();

        auto& l_references = m_reference_tables[l_thread_id]._references;
        if ((l_references.is_empty() == false) && (l_references.back().size() < references_per_page))
        {
            l_references.back().push_back(metadata_p);
            return;
        }
        l_references.emplace_back().push_back(metadata_p);
    }

    _FE_FORCE_INLINE_ static void __collect(internal::gc_ptr::metadata* const metadata_p) noexcept
    {
        FE_ASSERT(metadata_p != nullptr, "Assertion failed: cannot collect a null gc object.");

        FE::size l_metadata_offset_in_bytes = internal::gc_ptr::align_up(static_cast<var::size>(metadata_p->_sizeofT) * metadata_p->_length, alignof(internal::gc_ptr::metadata));
        for (var::size i = metadata_p->_length; i > 0; --i)
        {
            std::launder( reinterpret_cast<FE::gc_base*>(reinterpret_cast<var::byte*>(metadata_p->_data) + ((i - 1) * metadata_p->_sizeofT)) )->~gc_base();
        }
        metadata_p->_resource->deallocate(reinterpret_cast<var::byte*>(metadata_p) - l_metadata_offset_in_bytes, l_metadata_offset_in_bytes + sizeof(internal::gc_ptr::metadata), std::max<var::size>(metadata_p->_alignofT, alignof(internal::gc_ptr::metadata)));
    }
};


END_NAMESPACE

#pragma warning(pop)
#endif
