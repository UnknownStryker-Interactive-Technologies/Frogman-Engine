#include <FE/reference_table.hxx>
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




FE::reference_table::thread_reference_table::thread_reference_table(std::pmr::memory_resource* const resource_p) noexcept
    :   _references(resource_p),
        _gc_cursor(0)
{
}


FE::reference_table::reference_table(FE::int32 max_concurrency_p, std::pmr::memory_resource* const resource_p) noexcept
    :   m_max_concurrency(max_concurrency_p),
        m_reference_tables(),
        m_cyclic_reference_death_note(resource_p)
{
    FE_ASSERT(max_concurrency_p > 0, "Assertion failed: max_concurrency must be greater than zero.");
    FE_ASSERT(resource_p != nullptr, "Assertion failed: cannot create a reference table with a null memory_resource.");

    m_reference_tables = FE::make_unique<thread_reference_table[]>(resource_p, static_cast<var::size>(max_concurrency_p), resource_p);
}


void FE::reference_table::investigate_cyclic_references(internal::gc_ptr::metadata* const metadata_p, FE::uint32 max_steps_p) noexcept
{
}

void FE::reference_table::schedule_cyclic_reference_deletion() noexcept
{

}


void FE::reference_table::collect_garbage(FE::float64 percentage_p) noexcept
{
    FE_ASSERT((percentage_p >= 0.0) && (percentage_p <= 100.0), "Assertion failed: percentage_p must be in the range of 0.0 to 100.0.");

    FE::uint16 l_thread_id = FE::get_current_thread_id();
    FE_ASSERT(l_thread_id < m_max_concurrency, "Assertion failed: the current thread id is out of the max_concurrency range.");

    thread_reference_table& l_table = m_reference_tables[l_thread_id];
    if (l_table._references.is_empty() == true)
    {
        return;
    }

    FE::size l_table_size = ((l_table._references.size() - 1) * references_per_page) + l_table._references.back().size();
    FE::size l_inspection_count = std::min(static_cast<var::size>(std::ceil(static_cast<var::float64>(l_table_size) * percentage_p / 100.0)), l_table_size);

    var::size l_position = (l_table._gc_cursor < l_table_size) ? l_table._gc_cursor : 0;
    var::size l_slot = l_position % references_per_page;
    auto l_page = l_table._references.begin() + static_cast<var::ptrdiff>(l_position / references_per_page);

    for (var::size i = 0; i < l_inspection_count; ++i)
    {
        internal::gc_ptr::metadata*& l_reference = (*l_page)[l_slot];
        if ((l_reference != nullptr) && (l_reference->_reference_count.load(std::memory_order_acquire) == 0))
        {
            __collect(l_reference);
            l_reference = nullptr;
        }

        ++l_position;
        ++l_slot;
        if ((l_slot < references_per_page) && (l_position < l_table_size))
        {
            continue;
        }

        l_slot = 0;
        ++l_page;
        if (l_position == l_table_size)
        {
            l_position = 0;
            l_page = l_table._references.begin();
        }
    }
    l_table._gc_cursor = l_position;
}

