#ifndef _FE_FRAMEWORK_OBJECT_BASE_HPP_
#define _FE_FRAMEWORK_OBJECT_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/framework/atom_base.hpp>

// glm
#include <glm/vec3.hpp>




BEGIN_NAMESPACE(FE::framework)

class object_base : public atom_base
{
	REGISTER_FE_CLASS(object_base);
	FE_CLASS_HAS_A_BASE(atom_base);

protected:
	REGISTER_FE_PROPERTY(m_position);
	glm::dvec3 m_position;

public:
	object_base() noexcept;
	virtual ~object_base() noexcept = 0;

protected:
	virtual void on_construction() = 0;
	virtual void on_destruction() = 0;

	virtual void on_spawn(glm::dvec3 world_coordinate_p) = 0;
	virtual void on_despawn() = 0;

	REGISTER_FE_PROPERTY(m_is_active);
	var::boolean m_is_active;
	virtual void on_activate() = 0;
	virtual void on_deactivate() = 0;

	virtual void tick(FE::float64 delta_second_p) = 0;

public:
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
};

END_NAMESPACE
#endif