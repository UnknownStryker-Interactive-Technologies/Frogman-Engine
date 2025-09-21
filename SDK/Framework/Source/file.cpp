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
#include <FE/framework/file.hpp>




BEGIN_NAMESPACE(FE)


std::pmr::string serialize_init_file(const init& file_p) noexcept
{
    (void)file_p;
    return std::pmr::string();
}

init deserialize_init_file(const std::pmr::string& buffer_p) noexcept
{
    (void)buffer_p;
    return init();
}


END_NAMESPACE