#ifndef _FE_FRAMEWORK_SYSTEM_BASE_HPP_
#define _FE_FRAMEWORK_SYSTEM_BASE_HPP_
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
#include <FE/framework/framework.hpp>
#include <FE/framework/smart_ptr.hxx>

#include <FE/framework/type_info.hpp>

#include <string>
#include <vector>




BEGIN_NAMESPACE(FE)


class system_base;


using system = FE::smart_ptr<FE::system_base, FE::RefType::_Owner>;

template <class System>
using system_view = FE::smart_ptr<System, FE::RefType::_Observer>;


class system_base
{
	friend class ECS;
private:
	std::pmr::vector<std::pmr::string> m_target_component_types;
	std::pmr::string m_typename;

public:
	system_base() noexcept;
	virtual ~system_base() noexcept;

	virtual void operator()(class component_base* const component_p) = 0;

	_FE_FORCE_INLINE_ const std::pmr::string& get_typename() const { return m_typename; }
	
	template<class Component>
	_FE_FORCE_INLINE_ void subscribe_component() noexcept
	{
		static_assert(std::is_base_of<FE::component_base, Component>::value, "An invalid component type detected."); 
		m_target_component_types.emplace_back(FE::framework::reflection::type_id<Component>().name(), FE::framework::framework_base::get_framework().get_memory_resource());
	}
};


END_NAMESPACE
#endif