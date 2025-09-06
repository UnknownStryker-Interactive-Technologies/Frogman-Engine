#ifndef _FE_FRAMEWORK_COMPONENT_BASE_HPP_
#define _FE_FRAMEWORK_COMPONENT_BASE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>

#include <FE/framework/smart_ptr.hxx>

#include <forward_list>




BEGIN_NAMESPACE(FE)


class component_base;


using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;


class components
{
public:
	_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 512;

private:
	component m_components[max_components];
	var::size m_current_size = 0;

public:
	FE::size add_component(component&& comp_p) noexcept
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

	_FE_FORCE_INLINE_ component& last() { return m_components[m_current_size - 1]; }

	_FE_FORCE_INLINE_ var::size get_size() { return m_current_size; }
};


namespace internal::ECS
{
	struct component_identifier
	{
		std::pmr::forward_list<components>::iterator _group;
		var::size _index;
		std::pmr::string _typename;
		std::pmr::string _memory_layout_version; /* modify the string value when the memory layout of the component changes; this ensures correct auto serialization. 
			the intial value is set to "default"
		*/
	};
}


class component_base
{
	friend class archetype_base;
	friend class ECS;

private:
	internal::ECS::component_identifier m_identifier;

public:
	component_base() noexcept;
	virtual ~component_base() noexcept;

	_FE_FORCE_INLINE_ const std::pmr::string& get_typename() const { return m_identifier._typename; }
	_FE_FORCE_INLINE_ const std::pmr::string& get_memory_layout_version() const { return m_identifier._memory_layout_version; }
};


END_NAMESPACE
#endif