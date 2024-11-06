#ifndef _FE_FRAMEWORK_GAME_INSTANCE_HPP_
#define _FE_FRAMEWORK_GAME_INSTANCE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/framework/atom_base.hpp>
#include <FE/framework/game_thread.hpp>
#include <FE/framework/input_manager.hpp>




BEGIN_NAMESPACE(FE::framework)


class game_instance : public atom_base
{
	REGISTER_FE_CLASS(game_instance);
	FE_CLASS_HAS_A_BASE(atom_base);

	game_thread m_game_thread;

public:
	game_instance() noexcept;
	virtual ~game_instance() noexcept override;

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