#ifndef _FE_FRAMEWORK_CHARACTER_BASE_HPP_
#define _FE_FRAMEWORK_CHARACTER_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/framework/pawn_base.hpp>




BEGIN_NAMESPACE(FE::framework)

class character_base : public pawn_base
{
	REGISTER_FE_CLASS(character_base);
	FE_CLASS_HAS_A_BASE(pawn_base);

public:
	character_base() noexcept;
	virtual ~character_base() noexcept = 0;

protected:
	virtual void on_construction() = 0;
	virtual void on_destruction() = 0;

	virtual void on_spawn(glm::dvec3 world_coordinate_p) = 0;
	virtual void on_despawn() = 0;

	virtual void on_activate() = 0;
	virtual void on_deactivate() = 0;

	virtual void tick(FE::float64 delta_second_p) = 0;

public:
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
};

END_NAMESPACE
#endif