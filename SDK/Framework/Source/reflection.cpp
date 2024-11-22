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
#include <FE/framework/reflection.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)


reflection::method* system::s_method_reflection = nullptr;
reflection::property* system::s_property_reflection = nullptr;

void system::initialize(FE::size reflection_map_capacity_p) noexcept
{
	(void)reflection_map_capacity_p;
	if (s_method_reflection == nullptr)
	{
		s_method_reflection = new method(reflection_map_capacity_p);
	}

	if (s_property_reflection == nullptr)
	{
		s_property_reflection = new property(reflection_map_capacity_p);
	}
}
void system::shutdown() noexcept
{
	if (s_method_reflection != nullptr)
	{
		delete s_method_reflection;
		s_method_reflection = nullptr;
	}

	if (s_property_reflection != nullptr)
	{
		delete s_property_reflection;
		s_property_reflection = nullptr;
	}
}


END_NAMESPACE