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
#define _CRT_SECURE_NO_WARNINGS
#include <FE/clock.hxx>
#include <ctime>


const FE::clock::char_type* FE::clock::get_current_local_time() noexcept
{
    thread_local static FE::clock::char_type tl_s_local_time_string_buffer[current_local_time_buffer_size]{ (FE::clock::char_type)'\0' };

    auto l_current_time = boost::chrono::system_clock::now();
    std::time_t l_current_time_t = boost::chrono::system_clock::to_time_t(l_current_time);
    tm l_time;

#ifdef _FE_ON_WINDOWS_X86_64_
    localtime_s(&l_time, &l_current_time_t);
    std::strftime(tl_s_local_time_string_buffer, current_local_time_buffer_size, "%Y-%B-%d-%A  %p %Ih.%Mm.%Ss", &l_time);

#elif defined(_FE_ON_LINUX_X86_64_) 
    std::strftime(tl_s_local_time_string_buffer, current_local_time_buffer_size, "%Y-%B-%d-%A  %p %Ih.%Mm.%Ss", localtime_r(&l_current_time_t, &l_time));
#endif
    return tl_s_local_time_string_buffer;
}
