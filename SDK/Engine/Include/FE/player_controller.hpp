#ifndef _FROGMAN_ENGINE_PLAYER_CONTROLLER_HPP_
#define _FROGMAN_ENGINE_PLAYER_CONTROLLER_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/framework/component_base.hxx>
#include <FE/input_device.hpp>




BEGIN_NAMESPACE(FE)


class player_controller : public FE::component_base
{
public:
	input_device::keyboard _keyboard;
	input_device::mouse _mouse;
	FE::entity<FE::archetype_base> _player;

public:
    player_controller() noexcept = default;
	virtual ~player_controller() noexcept override = default;
};


END_NAMESPACE
#endif