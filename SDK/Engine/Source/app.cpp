/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/app.hpp>

#include <windows.h>




FE::uint32 FE::get_directory_of_current_executable(var::directory_char_t* out_buffer_p, FE::uint32 buffer_size_p) noexcept
{
#ifdef _FE_ON_WINDOWS_X86_64_
        _FE_MAYBE_UNUSED_ DWORD l_length = GetModuleFileNameW(nullptr, out_buffer_p, buffer_size_p);
        FE_EXIT_IF(l_length > _MAX_PATH_LENGTH_, FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "The directory is too long; the app should be installed in a folder with its directory length shorter than 4096.");
        FE_ASSERT(buffer_size_p > l_length, "Assertion failed: the directory buffer size must be greater than ${%u@0} bytes.", &l_length);
        return (FE::uint32)l_length;
#else

#endif
}
