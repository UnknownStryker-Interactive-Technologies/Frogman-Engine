#ifndef _FROGMAN_ENGINE_MODE_HPP_
#define _FROGMAN_ENGINE_MODE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
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
#include <FE/controller.hpp>




BEGIN_NAMESPACE(FE)


class mode : FE::archetype_base
{
	using base_type = FE::archetype_base;

	FE::controller m_controller;

public:
	mode(framework::ECS& host_p, ControllerType type_p = FE::ControllerType::_KeyboardAndMouse) noexcept;
	virtual ~mode() noexcept override;

public:
	_FE_FORCE_INLINE_ const FE::controller& get_controller() const noexcept { return m_controller; }
	_FE_FORCE_INLINE_ FE::controller& get_controller() noexcept { return m_controller; }
};


END_NAMESPACE
#endif