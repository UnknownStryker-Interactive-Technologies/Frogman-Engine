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
#include <FE/framework/system_base.hpp>
#include <FE/framework/framework.hpp>




BEGIN_NAMESPACE(FE::framework)


system_base::system_base() noexcept
{
}

void system_base::on_construction()
{
}

void system_base::on_destruction()
{
}


END_NAMESPACE