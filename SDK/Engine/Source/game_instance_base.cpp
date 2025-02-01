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
#include <FE/game_instance_base.hpp>




BEGIN_NAMESPACE(FE)



//// read game config file!
FE::int32 game_instance_base::launch(FE::int32 argc_p, FE::ASCII** argv_p)
{
    (void)argc_p;
	(void)argv_p;
    return FE::int32();
}

FE::int32 game_instance_base::run() noexcept
{
    return FE::int32();
}

FE::int32 game_instance_base::shutdown() noexcept
{
    return FE::int32();
}


END_NAMESPACE

