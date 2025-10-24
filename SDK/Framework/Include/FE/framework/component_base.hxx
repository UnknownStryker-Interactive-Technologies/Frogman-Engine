#ifndef _FE_FRAMEWORK_COMPONENT_BASE_HXX_
#define _FE_FRAMEWORK_COMPONENT_BASE_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/memory.hxx>
#include <FE/framework/framework.hxx>
#include <FE/framework/smart_ptr.hxx>

#include <forward_list>




BEGIN_NAMESPACE(FE)


class archetype_base;
class component_base;
CLASS_FORWARD_DECLARATION(framework, ECS);
CLASS_FORWARD_DECLARATION(framework, processors);
CLASS_FORWARD_DECLARATION(internal::ECS, gc_metadata_proxy_table);
CLASS_FORWARD_DECLARATION(internal::ECS, component_metadata);


class component_base
{
	friend class FE::archetype_base;
	friend class framework::ECS;
	friend class framework::processors;
	friend class internal::ECS::gc_metadata_proxy_table;

	FE::pmr_unique_ptr<class internal::ECS::component_metadata> m_metadata;

public:
	component_base() noexcept;
	virtual ~component_base() noexcept = default;

	FE::ASCII* get_typename() const noexcept;
	FE::ASCII* get_memory_layout_version() const noexcept;
};

using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;


namespace internal::ECS
{
	class components
	{
	public:
		_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 512;

	private:
		FE::component m_components[max_components];
		var::size m_current_size = 0;

	public:
		FE::size add_component(FE::component&& comp_p) noexcept
		{
			FE_ASSERT(m_current_size < max_components);

			m_components[m_current_size] = std::move(comp_p);
			return m_current_size++;
		}

		void remove_component(FE::size index_p) noexcept
		{
			FE_ASSERT(index_p < m_current_size);

			m_components[index_p].reset();
			last().swap(m_components[index_p]);
		}

		_FE_FORCE_INLINE_ FE::component& last() { return m_components[m_current_size - 1]; }

		_FE_FORCE_INLINE_ var::size get_size() const { return m_current_size; }

		_FE_FORCE_INLINE_ FE::component* begin() noexcept { return static_cast<FE::component*>(m_components); }
		_FE_FORCE_INLINE_ FE::component* end() noexcept { return static_cast<FE::component*>(m_components) + m_current_size; }
	};


	class gc_metadata
	{
	public:
		using member_component_list_type = std::pmr::vector< FE::smart_ptr<class FE::component_base, FE::RefType::_Observer>* >;
		using member_entity_list_type = std::pmr::vector< FE::smart_ptr<class FE::archetype_base, FE::RefType::_Observer>* >;

		member_component_list_type _member_components;
		member_entity_list_type _member_entities;
		std::atomic_bool _is_circular_reference;

		gc_metadata() noexcept;
		~gc_metadata() noexcept = default;
	};


	class component_metadata
	{
		friend class gc_metadata_proxy_table;
		friend class framework::processors;
		friend class framework::ECS;

		FE::pmr_unique_ptr<class gc_metadata> m_gc_metadata;

	public:
		std::pmr::forward_list<FE::internal::ECS::components>::iterator _group;
		var::size _index;
		FE::ASCII* _typename;
		FE::ASCII* _memory_layout_version; /* modify the string value when the memory layout of the component changes; this ensures correct auto serialization.
			the intial value is set to "default"
		*/
		component_metadata() noexcept;
		~component_metadata() noexcept = default;
	};


	class gc_metadata_proxy_table
	{
	public:
		template <typename T>
		_FE_FORCE_INLINE_ static void add_watch(FE::component_base* const host_p, T& property_p) noexcept
		{
			if constexpr (FE::is_observer_smart_ptr_v<T> == true)
			{
				if constexpr (std::is_base_of_v<FE::component_base, typename T::element_type> == true)
				{
					host_p->m_metadata->m_gc_metadata->_member_components.emplace_back( &FE::upcast_observer<class FE::component_base>(property_p) );
				}
				else if constexpr (std::is_base_of_v<FE::archetype_base, typename T::element_type> == true)
				{
					host_p->m_metadata->m_gc_metadata->_member_entities.emplace_back( &FE::upcast_observer<class FE::archetype_base>(property_p) );
				}
			}
		}
	};
}


END_NAMESPACE
#endif