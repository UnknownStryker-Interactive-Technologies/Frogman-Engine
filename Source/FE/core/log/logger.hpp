#ifndef _FE_LOG_HPP_
#define _FE_LOG_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>
#include <FE/core/private/debug.h>
#include <FE/core/fstream_guard.hxx>
#include <filesystem>
#include <fstream>
#include <string>


#if defined(_ENABLE_LOG_)
#define _IS_LOGGER_ENABLED_
#define IF_LOGGER_ENABLED(true_p, false_p) true_p
#define ENABLE_IF_LOGGER_ENABLED(code_p) code_p
#else
#define IF_LOGGER_ENABLED(true_p, false_p) false_p
#define ENABLE_IF_LOGGER_ENABLED(code_p)
#endif




BEGIN_NAMESPACE(FE::log)


class logger
{
public:
    _MAYBE_UNUSED_ static constexpr uint16 line_info_buffer_size = 32;
    _MAYBE_UNUSED_ static constexpr uint32 default_buffer_size = 10240;

#ifdef _WINDOWS_X86_64_
    using directory_char_type = var::wchar;
#elif defined(_LINUX_64BIT_OS_)
    using directory_char_type = var::character
#endif
    using directory_buffer_type = std::basic_string<directory_char_type, std::char_traits<directory_char_type>>;
    using filename_buffer_type = directory_buffer_type;
    using buffer_type = std::basic_string<char, std::char_traits<char>>;

protected:
    std::ofstream m_file_logger;
    FE::ofstream_guard m_file_guard;
    buffer_type  m_log_buffer;
    directory_buffer_type  m_directory_buffer;

    void __reserve() noexcept;

public:
    logger() noexcept;
    ~logger() noexcept = default;

    void set_root_directory(const directory_char_type* root_directory_name_p = (std::filesystem::current_path() / "Logs").c_str()) noexcept;

    void mkdir(const directory_char_type* folder_name_p) noexcept;
    void cd(const directory_char_type* folder_name_p) noexcept;

    void do_log(const char* content_p, const directory_char_type* filename_p = nullptr) noexcept;
};




class fatal_error_log : public logger
{
public:
    using base_type = logger;

    fatal_error_log() noexcept;
    ~fatal_error_log() noexcept;

    void do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept;
};

class file_log : public logger
{
public:
    using base_type = logger;

    file_log() noexcept;
    ~file_log() noexcept;

    void do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept;
};


END_NAMESPACE


namespace FE::internal::log
{
#ifdef _ENABLE_LOG_
    _FORCE_INLINE_ void __FE_LOG_IMPLEMENTATION(const char* const message_p, const char* const file_name_p, const char* const function_name_p, FE::uint32 line_p) noexcept
    {
        thread_local static ::FE::log::file_log tl_s_init;
        tl_s_init.do_log(message_p, file_name_p, function_name_p, line_p);
    }
#endif

#if defined(_ENABLE_ASSERT_) || defined(_ENABLE_EXIT_)
    _FORCE_INLINE_ void __FE_ABORT_IMPLEMENTATION(const char* const message_p, const char* const file_name_p, const char* const function_name_p, FE::uint32 line_p) noexcept
    {
        thread_local static ::FE::log::fatal_error_log tl_s_init;
        tl_s_init.do_log(message_p, file_name_p, function_name_p, line_p);
    }
#endif
}


#endif