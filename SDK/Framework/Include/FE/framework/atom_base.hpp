#ifndef _FE_FRAMEWORK_ATOM_BASE_HPP_
#define _FE_FRAMEWORK_ATOM_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>

#include <FE/framework/reflection/reflection.h>




BEGIN_NAMESPACE(FE::framework)

/*
The top class for all game objects in Frogman Engine.
This class is not meant to be instantiated.

The class name is irrelevant to the atomic memory operation.
*/
class atom_base
{
	REGISTER_FE_CLASS(atom_base);
public:
	atom_base() noexcept;
	virtual ~atom_base() noexcept = default;

protected:
	virtual void on_construction() = 0;
	virtual void on_destruction() = 0;

	REGISTER_FE_PROPERTY(m_is_tick_enabled);
	var::boolean m_is_tick_enabled;
	virtual void tick(FE::float64 delta_second_p) = 0;

public:
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
};

END_NAMESPACE
#endif