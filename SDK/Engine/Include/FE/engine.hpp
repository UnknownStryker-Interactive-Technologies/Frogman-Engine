#ifndef _FROGMAN_ENGINE_HPP_
#define _FROGMAN_ENGINE_HPP_
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
#include <FE/pair.hxx>

#include <functional> // std

#ifdef FROGMAN_ENGINE
#error Frogman Engine Prohibits macroizing the keyword "FROGMAN_ENGINE()".
#else                                                                                                              // The name below does not follow the naming convention since it is considered hidden from users.
#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (FE::int32, FE::ASCII**)> FrogmanEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::ASCII** argv_p) { return new ::FE::framework::game_engine(argc_p, argv_p); } );
#endif

#include <FE/framework/framework.hpp>




BEGIN_NAMESPACE(FE)


class game_engine : public framework::framework_base
{
	std::shared_ptr<class ECS> m_entity_component_system;
	std::shared_ptr<class game_instance_base> m_game_instance;

public:
	game_engine(FE::int32 argc_p, FE::ASCII** argv_p);
	~game_engine();

	static game_engine& get_game_engine() noexcept;

	FE::ECS& get_entity_component_system() noexcept;

private:
	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;
};


END_NAMESPACE
#endif