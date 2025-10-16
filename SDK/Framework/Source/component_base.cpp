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
#include <FE/framework/component_base.hpp>
#include <FE/framework.h>




BEGIN_NAMESPACE(FE)


component_base::component_base() noexcept
	:	m_metadata( framework::make_unique<internal::ECS::component_metadata>() )
{
}

FE::ASCII* component_base::get_typename() const noexcept { return m_metadata->_typename; }
FE::ASCII* component_base::get_memory_layout_version() const noexcept { return m_metadata->_memory_layout_version; }


internal::ECS::gc_metadata::gc_metadata() noexcept
	:	_member_components(framework::framework_base::get_framework().get_memory_resource()),
		_member_entities(framework::framework_base::get_framework().get_memory_resource()),
		_is_circular_reference(false)
{
}


internal::ECS::component_metadata::component_metadata() noexcept
	:	m_gc_metadata( framework::make_unique<gc_metadata>() ),
		_group(),
		_index(0),
		_typename(nullptr),
		_memory_layout_version("default")
{
}


END_NAMESPACE

