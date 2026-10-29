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
#include <FE/engine.hpp>
#include <FE/memory.hpp>
#include <FE/stack.hxx>
#include <FE/processors.hxx>

#include <absl/container/flat_hash_set.h>




FE::reference_table::thread_reference_table::thread_reference_table(std::pmr::memory_resource* const resource_p) noexcept
    :   _references(resource_p)
{
}

FE::reference_table::reference_table(FE::int32 max_concurrency_p, std::pmr::memory_resource* const resource_p) noexcept
    :   m_max_concurrency(max_concurrency_p),
        m_reference_tables(),
        m_gc_ptr_offset_registry_lock(),
        m_gc_ptr_offset_registry(resource_p),
        m_cyclic_reference_inspector(),
        m_cyclic_reference_inspector_anesthesia(),
        m_suspected_cyclic_reference_queue(FE::get_concurrent_memory_resource())
{
    FE_ASSERT(max_concurrency_p > 0, "Assertion failed: max_concurrency must be greater than zero.");
    FE_ASSERT(resource_p != nullptr, "Assertion failed: cannot create a reference table with a null memory_resource.");

    m_reference_tables = FE::make_unique<thread_reference_table[]>(resource_p, static_cast<var::size>(max_concurrency_p), resource_p);
	m_cyclic_reference_inspector = std::jthread(&reference_table::__investigate_cyclic_references, this);
}

FE::reference_table::~reference_table() noexcept
{
    {
        std::lock_guard<std::mutex> l_lock(m_cyclic_reference_inspector_anesthesia._lock); // the inspector between its check and its wait sees the flag
    }
    m_cyclic_reference_inspector_anesthesia._cv.notify_all(); // resuscitate the inspector from anesthesia
    m_cyclic_reference_inspector.join();
}


void FE::reference_table::collect_garbages(FE::float64 percentage_p) noexcept
{
    FE_ASSERT((percentage_p >= 0.0) && (percentage_p <= 100.0), "Assertion failed: percentage_p must be in the range of 0.0 to 100.0.");
    FE_ASSERT(FE::fiber_scheduler::is_fiber() == true, "Assertion failed: collect_garbages must be called from a fiber context.");
    
    FE::uint16 l_thread_id = FE::get_current_thread_id();
    FE_ASSERT(l_thread_id < m_max_concurrency, "Assertion failed: the current thread id is out of the max_concurrency range.");

    thread_reference_table& l_table = m_reference_tables[l_thread_id];
    if (l_table._references.is_empty() == true)
    {
        return;
    }

	FE::size l_total_reference_entries = l_table._references.size() * references_per_page;
    FE::size l_entries_to_inspect = (FE::size)((FE::float64)l_total_reference_entries / (percentage_p / 100.0));
	var::size l_inspected_entries = 0;

    for (auto& page : l_table._references)
    {
        for (var::size i = 0; i < page.size(); ++i)
        {
			if (page[i]->_is_under_concurrent_inspection.load(std::memory_order_acquire) == true)
			{
				continue;
			}

            if ((page[i]->_reference_count != 0) && (page[i]->_is_cyclic_garbage.load(std::memory_order_acquire) == true))
            {
                __collect_cyclic_garbage(page[i]);
                continue;
            }

            FE::size l_type_hash = page[i]->_data->get_type_hash();
            if ((page[i]->_reference_count == 0) || (__find_gc_ptr_offsets(l_type_hash).empty() == false))
            {
                page[i]->_is_under_concurrent_inspection.store(true, std::memory_order_release);
                m_suspected_cyclic_reference_queue.push({ page[i], l_type_hash });
                m_cyclic_reference_inspector_anesthesia._enqueue_count.fetch_add(1, std::memory_order_acq_rel);
                m_cyclic_reference_inspector_anesthesia._cv.notify_one();
            }

            ++l_inspected_entries;
            if (l_inspected_entries == l_entries_to_inspect)
            {
                FE::fiber_scheduler::yield();
				l_inspected_entries = 0;
            }
        }
    }

    for (auto& page : l_table._references)
    {
        for (var::size i = 0; i < page.size(); ++i)
        {
            if (page[i]->_is_garbage.load(std::memory_order_acquire) == true)
            {
                __collect(page[i]);
                page[i] = page.back();
				page.pop_back();
            }
        }
    }
}


void FE::reference_table::__investigate_cyclic_references() noexcept
{
    using circular_visit_vertex_map_type =
        absl::flat_hash_map<internal::gc_ptr::metadata*, var::int32,
        absl::DefaultHashContainerHash<internal::gc_ptr::metadata*>, absl::DefaultHashContainerEq<internal::gc_ptr::metadata*>,
        std::pmr::polymorphic_allocator< std::pair<internal::gc_ptr::metadata* const, var::int32> >
        >;

    using suspect_set_type =
        absl::flat_hash_set<internal::gc_ptr::metadata*,
        absl::DefaultHashContainerHash<internal::gc_ptr::metadata*>, absl::DefaultHashContainerEq<internal::gc_ptr::metadata*>,
        std::pmr::polymorphic_allocator<internal::gc_ptr::metadata*>
        >;

    struct stackframe
    {
        internal::gc_ptr::metadata* _metadata = nullptr;
        var::uint64 _child_reference_edge_index = 0;
        std::span<const var::int64> _gc_ptr_offsets;

        auto children_begin() noexcept { return _gc_ptr_offsets.begin(); }
        auto children_end() noexcept { return _gc_ptr_offsets.end(); }
    };


    std::pmr::vector<stackframe> l_inspection_stack;
    circular_visit_vertex_map_type l_visited_vertex_map;
    suspect_set_type l_prob_garbages;


    while (FE::engine::get_engine().get_processors().is_active())
    {
        FE::uint64 l_enqueue_count = m_cyclic_reference_inspector_anesthesia._enqueue_count.load(std::memory_order_acquire);
        suspect l_suspect;
        if (m_suspected_cyclic_reference_queue.try_pop(l_suspect) == false) // no tasks to available, put the thread to sleep for a while to avoid busy-waiting.
        {
            for (auto l_prob_garbage = l_prob_garbages.begin(); l_prob_garbage != l_prob_garbages.end();)
            {
                if (l_prob_garbage == l_prob_garbages.begin())
                {
                    l_visited_vertex_map.clear();
                    for (internal::gc_ptr::metadata* const vertex : l_prob_garbages)
                    {
                        for (var::int64 l_offset : __find_gc_ptr_offsets(vertex->_data->get_type_hash()))
                        {
                            l_visited_vertex_map[reinterpret_cast<gc_ptr_base*>(reinterpret_cast<var::byte*>(vertex->_data) + l_offset)->__get_control_block()] += 1;
                        }
                    }
                }

                var::boolean l_is_certain_garbage = true;
                for (var::int64 l_offset : __find_gc_ptr_offsets((*l_prob_garbage)->_data->get_type_hash()))
                {
                    internal::gc_ptr::metadata* const l_child_vertex = reinterpret_cast<gc_ptr_base*>(reinterpret_cast<var::byte*>((*l_prob_garbage)->_data) + l_offset)->__get_control_block();
                    if ((l_child_vertex != nullptr) && (l_prob_garbages.contains(l_child_vertex) == false))
                    {
                        l_is_certain_garbage = false;
                        break;
                    }
                }

                if ((l_is_certain_garbage == false) || ((*l_prob_garbage)->_reference_count.load(std::memory_order_acquire) - l_visited_vertex_map[*l_prob_garbage] != 0))
                {
                    l_prob_garbages.erase(l_prob_garbage);
                    l_prob_garbage = l_prob_garbages.begin();
                    continue;
                }
                ++l_prob_garbage;
            }

            for (internal::gc_ptr::metadata* const vertex : l_prob_garbages)
            {
                vertex->_is_cyclic_garbage.store(true, std::memory_order_release);
            }
            l_prob_garbages.clear();
            l_visited_vertex_map.clear();

            std::unique_lock<std::mutex> l_lock(m_cyclic_reference_inspector_anesthesia._lock);
            m_cyclic_reference_inspector_anesthesia._cv.wait(l_lock, 
                [this, l_enqueue_count]() noexcept
                {
                    return (m_cyclic_reference_inspector_anesthesia._enqueue_count.load(std::memory_order_acquire) != l_enqueue_count) || FE::engine::get_engine().get_processors().is_active() != true;
                }
            );
            continue;
        }

        internal::gc_ptr::metadata* const l_metadata = l_suspect._metadata;
        FE_ASSERT(l_metadata != nullptr, "Assertion failed: cannot investigate a null pointer for cyclic references.");
        FE_ASSERT(l_metadata->_data != nullptr, "Assertion failed: cannot investigate a null pointer for cyclic references.");
        
        if (l_metadata->_reference_count.load(std::memory_order_acquire) == 0)
        {
            l_prob_garbages.erase(l_metadata);
            l_metadata->_is_garbage.store(true, std::memory_order_release);
            continue;
        }

        if (l_metadata->_is_cyclic_garbage.load(std::memory_order_acquire) == true)
        {
            l_metadata->_is_under_concurrent_inspection.store(false, std::memory_order_release);
            continue;
        }

        // DFS
        l_visited_vertex_map[l_metadata] = 0;
		l_inspection_stack.push_back({ l_metadata, 0, __find_gc_ptr_offsets(l_suspect._type_hash) });
        
        auto l_edge_iterator = l_inspection_stack.back().children_begin() + l_inspection_stack.back()._child_reference_edge_index;
        for (;;)
        {
            if (l_edge_iterator == l_inspection_stack.back().children_end())
            {
                l_inspection_stack.pop_back();
				if (l_inspection_stack.empty())
				{
					break;
				}
                l_edge_iterator = l_inspection_stack.back().children_begin() + l_inspection_stack.back()._child_reference_edge_index;
                continue;
            }

            gc_ptr_base* const l_child = reinterpret_cast<gc_ptr_base*>(reinterpret_cast<var::byte*>(l_inspection_stack.back()._metadata->_data) + *l_edge_iterator);
            internal::gc_ptr::metadata* const l_child_vertex = l_child->__get_control_block();
            if (l_child_vertex == nullptr)
            {
                ++l_edge_iterator;
                ++l_inspection_stack.back()._child_reference_edge_index;
                continue;
            }
            auto l_search_result = l_visited_vertex_map.find(l_child_vertex);

            ++l_inspection_stack.back()._child_reference_edge_index;

            if (l_search_result == l_visited_vertex_map.end())
            {
                // this must be the first visit.
                l_visited_vertex_map[l_child_vertex] = 0;
                l_inspection_stack.push_back({ l_child_vertex, 0 });
                l_inspection_stack.back()._gc_ptr_offsets = __find_gc_ptr_offsets(l_inspection_stack.back()._metadata->_data->get_type_hash());
                l_edge_iterator = l_inspection_stack.back().children_begin() + l_inspection_stack.back()._child_reference_edge_index;
                continue;
            }

            // this vertex has been visited before, which means a cycle is detected.
            l_visited_vertex_map[l_child_vertex] += 1;
            ++l_edge_iterator;
        }

		for (const auto& [vertex, cyclic_visit_count] : l_visited_vertex_map)
		{
			if (vertex->_reference_count.load(std::memory_order_acquire) - cyclic_visit_count == 0)
			{
                l_prob_garbages.insert(vertex);
			}
		}

		l_metadata->_is_under_concurrent_inspection.store(false, std::memory_order_release);
		l_inspection_stack.clear();
        l_visited_vertex_map.clear();
    }
}

void FE::reference_table::__collect(internal::gc_ptr::metadata* const metadata_p) noexcept
{
    FE_ASSERT(metadata_p != nullptr, "Assertion failed: cannot collect a null object.");
    FE_ASSERT(metadata_p->_data != nullptr, "Assertion failed: cannot collect a null object.");

    for (var::size i = 0; i < metadata_p->_length; ++i)
    {
        var::byte* l_target = reinterpret_cast<var::byte*>(metadata_p->_data) + (i * metadata_p->_sizeofT);
        std::launder((FE::gc_base*)l_target)->~gc_base();
    }

    FE::size l_metadata_offset_in_bytes = internal::gc_ptr::align_up(static_cast<var::size>(metadata_p->_sizeofT) * metadata_p->_length, alignof(internal::gc_ptr::metadata));
    var::byte* const l_ptr_to_delete = reinterpret_cast<var::byte*>(metadata_p) - l_metadata_offset_in_bytes;
    FE::size l_total_size_in_bytes = l_metadata_offset_in_bytes + sizeof(internal::gc_ptr::metadata);
    FE::size l_alignment_in_bytes = std::max<var::size>(metadata_p->_alignofT, alignof(internal::gc_ptr::metadata));
    metadata_p->_resource->deallocate(l_ptr_to_delete, l_total_size_in_bytes, l_alignment_in_bytes);
}

void FE::reference_table::__collect_cyclic_garbage(internal::gc_ptr::metadata* const metadata_p) noexcept
{
    std::span<const var::int64> l_gc_ptr_offsets = __find_gc_ptr_offsets(metadata_p->_data->get_type_hash());
    for (var::size i = 0; i < metadata_p->_length; ++i)
    {
        var::byte* const l_this = reinterpret_cast<var::byte*>(metadata_p->_data) + (i * metadata_p->_sizeofT);
        for (var::int64 l_offset : l_gc_ptr_offsets)
        {
            reinterpret_cast<gc_ptr_base*>(l_this + l_offset)->__release();
        }
    }
}

void FE::reference_table::__push_back(internal::gc_ptr::metadata* const metadata_p) noexcept
{
    FE_ASSERT(metadata_p != nullptr, "Assertion failed: cannot push back a null gc object.");
    FE::uint16 l_thread_id = FE::get_current_thread_id();

    auto& l_references = m_reference_tables[l_thread_id]._references;

    if (l_references.is_empty() || l_references.back().size() == l_references.back().capacity())
    {
        l_references.emplace_back();
    }
 
    l_references.back().push_back(metadata_p);
}
