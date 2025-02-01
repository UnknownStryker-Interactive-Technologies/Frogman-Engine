#ifndef _FE_GAME_INSTANCE_BASE_HPP_
#define _FE_GAME_INSTANCE_BASE_HPP_
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

#include <FE/framework/framework.hpp>
#include <FE/framework/reflection.hpp>




BEGIN_NAMESPACE(FE)


class game_instance_base
{
	FE_CLASS(game_instance_base);

public:
	game_instance_base() noexcept = default;
	~game_instance_base() noexcept = default;

protected:
	FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p);
	FE::int32 run() noexcept;
	FE::int32 shutdown() noexcept;
};


END_NAMESPACE
#endif