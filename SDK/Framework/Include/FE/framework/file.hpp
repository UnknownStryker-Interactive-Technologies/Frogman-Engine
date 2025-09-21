#ifndef _FE_FRAMEWORK_FRAMEWORK_FILE_HPP_
#define _FE_FRAMEWORK_FRAMEWORK_FILE_HPP_
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

#include <robin_hood.h>

#include <vector>




BEGIN_NAMESPACE(FE)

// Data structure representation of a .init file.
using init = robin_hood::unordered_map<	std::pmr::string,
										robin_hood::unordered_map<std::pmr::string, std::pmr::vector<std::pmr::string>>
										>;

std::pmr::string serialize_init_file(const init& file_p) noexcept;
init deserialize_init_file(const std::pmr::string& buffer_p) noexcept;


END_NAMESPACE
#endif