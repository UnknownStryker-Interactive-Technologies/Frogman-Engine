#ifndef _FROGMAN_ENGINE_CONTROLLER_HPP_
#define _FROGMAN_ENGINE_CONTROLLER_HPP_
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
#include <FE/prerequisites.hxx>
#include <FE/framework/ECS.hxx>
#include <FE/input_device.hpp>




BEGIN_NAMESPACE(FE)


enum struct ControllerType
{
	_KeyboardAndMouse,
	_SpearX,
	_AI
};


class controller : public FE::archetype_base
{
	using base_type = FE::archetype_base;

	ControllerType m_device_type;
	FE::entity<FE::archetype_base> m_player;

	FE::component_view<FE::input_device::keyboard> m_keyboard_view;
	FE::component_view<FE::input_device::mouse> m_mouse_view;

public:
    controller(framework::ECS& host_p, ControllerType device_type_p) noexcept;
	virtual ~controller() noexcept override;

	_FE_FORCE_INLINE_ ControllerType get_device_type() const noexcept { return m_device_type; }

	_FE_FORCE_INLINE_ FE::entity<FE::archetype_base> get_player() const noexcept { return m_player; }
	_FE_FORCE_INLINE_ FE::entity<FE::archetype_base> get_player() noexcept { return m_player; }
	_FE_FORCE_INLINE_ void set_player(const FE::entity<FE::archetype_base>& player_p) noexcept { m_player = player_p; }

	_FE_FORCE_INLINE_ FE::input_device::keyboard& get_keyboard() noexcept { return *m_keyboard_view; }
	_FE_FORCE_INLINE_ FE::input_device::mouse& get_mouse() noexcept { return *m_mouse_view; }
};


END_NAMESPACE
#endif