/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/framework/world.hxx>

#include <FE/framework/framework.hxx>
#include <FE/framework/reflection.hxx>




FE::world::world(const area_of_operation& area_of_operation_p) noexcept
	:	base_type( FE::framework::framework_base::get_framework().get_ecs() ),
		m_area_of_operation( area_of_operation_p ),
		m_ecs(m_area_of_operation._component_type_count_hint),
		m_mode( FE::framework::framework_base::get_framework().get_ecs() )
{
	m_ecs.initialize(std::move(m_area_of_operation._initializer_list));
	// call the decryption method to decrypt the world if the functor is not null.
	// asset pipelines have to be implemented somewhere here.
}