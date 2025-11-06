#include <FE/app.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>




FE::uint32 FE::get_directory_of_current_executable(var::ASCII* out_buffer_p, FE::uint32 buffer_size_p) noexcept
{
    _FE_MAYBE_UNUSED_ DWORD l_length = GetModuleFileNameA(nullptr, out_buffer_p, buffer_size_p);
    FE_EXIT_IF(l_length > _MAX_PATH_LENGTH_, FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "The directory is too long; the app should be installed in a folder with its directory length shorter than 4096.");
    FE_ASSERT(buffer_size_p > l_length, "Assertion failed: the directory buffer size must be greater than ${%u@0} bytes.", &l_length);
	return (FE::uint32)l_length;
}
