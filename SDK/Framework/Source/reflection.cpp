// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
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