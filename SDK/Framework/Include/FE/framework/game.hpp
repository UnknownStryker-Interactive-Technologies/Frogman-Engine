#ifndef _FE_FRAMEWORK_GAME_HPP_
#define _FE_FRAMEWORK_GAME_HPP_
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


class game
{
	FE_CLASS(game);

public:
	game() noexcept = default;
	~game() noexcept = default;

protected:
	int32 launch(int32 argc_p, ASCII** argv_p);
	int32 run() noexcept;
	int32 shutdown() noexcept;
};


END_NAMESPACE
#endif