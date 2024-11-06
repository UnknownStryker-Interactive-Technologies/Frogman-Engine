#ifndef _FE_FRAMEWORK_OBJECT_COMPONENT_BASE_HPP_
#define _FE_FRAMEWORK_OBJECT_COMPONENT_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/framework/atom_base.hpp>
	



BEGIN_NAMESPACE(FE::framework)


class object_component_base : public atom_base
{
	REGISTER_FE_CLASS(object_component_base);
	FE_CLASS_HAS_A_BASE(atom_base);

public:
	object_component_base() noexcept;
	virtual ~object_component_base() noexcept = 0;

protected:
	virtual void on_construction() = 0;
	virtual void on_destruction() = 0;

	virtual void tick(FE::float64 delta_second_p) = 0;

public:
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
};

END_NAMESPACE
#endif