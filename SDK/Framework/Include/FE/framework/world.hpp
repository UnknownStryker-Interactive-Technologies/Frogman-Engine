#ifndef _FE_FRAMEWORK_WORLD_HPP_
#define _FE_FRAMEWORK_WORLD_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/framework/atom_base.hpp>




BEGIN_NAMESPACE(FE::framework)

class world final : public atom_base
{
	REGISTER_FE_CLASS(world);
	FE_CLASS_HAS_A_BASE(atom_base);

public:
	world() noexcept;
	virtual ~world() noexcept override;

protected:
	virtual void on_construction() override;
	virtual void on_destruction() override;

	virtual void tick(FE::float64 delta_second_p) override;

public:
	virtual void serialize() override;
	virtual void deserialize() override;
};

END_NAMESPACE
#endif