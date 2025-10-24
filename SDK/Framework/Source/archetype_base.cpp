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
#include <FE/framework/archetype_base.hxx>
#include <FE/framework/component_base.hxx>




BEGIN_NAMESPACE(FE)


archetype_base::archetype_base() noexcept
    :   m_name()
{
}

archetype_base::~archetype_base() noexcept
{
    for (auto& pair : m_component_view_table)
    {
		pair.second->m_metadata->_group->remove_component(pair.second->m_metadata->_index);
    }
}


END_NAMESPACE

