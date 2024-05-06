#ifndef _FE_FRAMEWORK_PRIVATE_INTERNAL_FUNCTIONS_H_
#define _FE_FRAMEWORK_PRIVATE_INTERNAL_FUNCTIONS_H_
#include <FE/core/types.hxx>

#include <chrono>
#ifdef _WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#elif defined(_LINUX_X86_64_)
#include <pthread.h>
#endif

#define _BUFFER_SIZE_ 64




BEGIN_NAMESPACE(FE::framework::internal)


_FORCE_INLINE_ var::uint64 this_thread_id() noexcept
{
#ifdef _WINDOWS_X86_64_
    return GetCurrentThreadId();
#elif defined(_LINUX_X86_64_)
    return (FE::var::uint64)pthread_self();
#endif
}

_FORCE_INLINE_ const char* get_current_local_time() noexcept
{
    thread_local static char tl_s_local_time_string_buffer[_BUFFER_SIZE_]{ '\0' };

    auto l_current_time = ::std::chrono::system_clock::now();
    std::time_t l_current_time_t = ::std::chrono::system_clock::to_time_t(l_current_time);
    tm l_time;

#ifdef _WINDOWS_X86_64_
    localtime_s(&l_time, &l_current_time_t);
    ::std::strftime(tl_s_local_time_string_buffer, _BUFFER_SIZE_, "%Y-%B-%d-%A  %p %Ih.%Mm.%Ss", &l_time);

#elif defined(_LINUX_X86_64_)
    ::std::strftime(tl_s_local_time_string_buffer, _BUFFER_SIZE_, "%Y-%B-%d-%A  %p %Ih.%Mm.%Ss", localtime_r(&l_current_time_t, &l_time));
#endif
    return tl_s_local_time_string_buffer;
}


END_NAMESPACE;
#endif