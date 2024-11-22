#ifndef _FE_FRAMEWORK_OBJECT_BASE_HPP_
#define _FE_FRAMEWORK_OBJECT_BASE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/framework/super_object_base.hpp>

// glm
#include <glm/vec3.hpp>




BEGIN_NAMESPACE(FE::framework)

class object_base : public super_object_base
{
	FE_CLASS(object_base);
	FE_CLASS_HAS_A_BASE(super_object_base);

protected:
	FE_PROPERTY(m_position);
	glm::dvec3 m_position;

public:
	object_base() noexcept;
	virtual ~object_base() noexcept = default;

protected:
	virtual void on_construction() override;
	virtual void on_destruction() override;

	virtual void on_spawn(_FE_MAYBE_UNUSED_ glm::dvec3 world_coordinate_p);
	virtual void on_despawn();

	FE_PROPERTY(m_is_active);
	var::boolean m_is_active;

	virtual void on_activate();
	virtual void on_deactivate();

	virtual void tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p) override;
};

END_NAMESPACE
#endif