#ifndef _FROGMAN_ENGINE_APP_HPP_
#define _FROGMAN_ENGINE_APP_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/framework/framework.hxx>




BEGIN_NAMESPACE(FE)


FE::uint32 get_directory_of_current_executable(var::ASCII* out_buffer_p, FE::uint32 buffer_size_p) noexcept;


_FE_FORCE_INLINE_ void schedule_app_restart() noexcept
{
	FE::framework::framework_base::get_framework().request_restart();
}

_FE_FORCE_INLINE_ void cancel_app_restart() noexcept
{
	FE::framework::framework_base::get_framework().cancel_restart();
}

_FE_FORCE_INLINE_ FE::uint32 get_max_concurrency() noexcept
{
	return FE::framework::framework_base::get_framework().get_program_options().get_max_concurrency();
}

_FE_FORCE_INLINE_ const std::locale& get_current_system_locale() noexcept
{
	return FE::framework::framework_base::get_framework().get_current_system_locale();
}


END_NAMESPACE
#endif