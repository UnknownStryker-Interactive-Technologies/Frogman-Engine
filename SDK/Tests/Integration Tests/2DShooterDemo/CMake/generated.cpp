// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 

class component_derived : public FE::component_base
{
public:
	component_derived() noexcept = default;
	virtual ~component_derived() noexcept = default;

};
void f(FE::component_base* const component_p)
{
	(component_p);
}

void load_reflection_data()
{
	FE::framework::framework_base::get_framework().get_method_reflection().associate_system<component_derived>(FE::SystemCallPhase::_EndPhysics, &f);
}
