#ifndef _FE_FRAMEWORK_ATOM_BASE_HPP_
#define _FE_FRAMEWORK_ATOM_BASE_HPP_
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

#include <FE/framework/reflection.hpp>




BEGIN_NAMESPACE(FE::framework)

/*
The top class for all game objects in Frogman Engine.
This class is not meant to be instantiated.

The class name is irrelevant to the atomic memory operation.
*/
class atom_base
{
	FE_CLASS(atom_base);

public:
	atom_base() noexcept;
	virtual ~atom_base() noexcept = default;

protected:
	virtual void on_construction() {};
	virtual void on_destruction() {};

	FE_PROPERTY(m_is_tick_enabled);
	var::boolean m_is_tick_enabled;
	virtual void tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p) {};

public:
	virtual void serialize() {};
	virtual void deserialize() {};
};

END_NAMESPACE
#endif