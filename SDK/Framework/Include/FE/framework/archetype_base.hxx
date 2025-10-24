#ifndef _FE_FRAMEWORK_ARCHETYPE_BASE_HXX_
#define _FE_FRAMEWORK_ARCHETYPE_BASE_HXX_
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

#include <FE/framework/component_base.hxx>
#include <FE/framework/smart_ptr.hxx>
#include <FE/framework/type_info.hxx>

#include <robin_hood.h>

#include <string>




BEGIN_NAMESPACE(FE)


CLASS_FORWARD_DECLARATION(framework, ECS);
CLASS_FORWARD_DECLARATION(framework, processors);

class archetype_base;


template <class Archetype>
using entity = FE::smart_ptr<Archetype, FE::RefType::_Observer>; // pointers can be though of as integers or handles
using archetype = FE::smart_ptr<FE::archetype_base, FE::RefType::_Owner>;

template<class Component>
using component_view = FE::smart_ptr<Component, FE::RefType::_Observer>;


class archetype_base
{
	friend class framework::ECS;
	friend class framework::processors;
	using component_view_table = robin_hood::unordered_map<std::size_t, component_view<component_base>>;

private:
	component_view_table m_component_view_table;
	std::pmr::string m_name;

public:
	archetype_base() noexcept;
	virtual ~archetype_base() noexcept;
	
	const std::pmr::string& get_name() const noexcept { return m_name; }

	template <class Component>
	component_view<Component> get_component() noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: T must be derived from FE::component_base.");
		typename component_view_table::iterator l_probe_result = m_component_view_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		if (l_probe_result != m_component_view_table.end())
		{
			return FE::downcast_observer<Component>(l_probe_result->second);
		}
		return component_view<Component>();
	}
};


END_NAMESPACE
#endif