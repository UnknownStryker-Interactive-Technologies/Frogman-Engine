#ifndef _FE_FRAMEWORK_ARCHETYPE_BASE_HPP_
#define _FE_FRAMEWORK_ARCHETYPE_BASE_HPP_
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
#include <FE/framework/component_base.hpp>

#include <robin_hood.h>
#include <string>




BEGIN_NAMESPACE(FE)


template<class T>
using component_view = FE::smart_ptr<T, FE::RefType::_Observer>;


class archetype_base
{
	friend class ECS;
	robin_hood::unordered_map<std::size_t, component_view<component_base>> m_component_view_table;
	std::pmr::string m_name;

public:
	archetype_base() noexcept;
	virtual ~archetype_base() noexcept;
	
	const std::pmr::string& get_name() const noexcept { return m_name; }
};


END_NAMESPACE
#endif