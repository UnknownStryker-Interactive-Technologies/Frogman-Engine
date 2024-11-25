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
#include <FE/framework/world_context.hpp>




BEGIN_NAMESPACE(FE::framework)


world_context::world_context() noexcept
{
}

world_context::~world_context() noexcept
{
}

void world_context::on_construction()
{
}

void world_context::on_destruction()
{
}

void world_context::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{

}


END_NAMESPACE