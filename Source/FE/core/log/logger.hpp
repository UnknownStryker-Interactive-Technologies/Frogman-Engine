#ifndef _FE_LOGGER_HPP_
#define _FE_LOGGER_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>
#include <FE/core/private/debug.h>
#include <FE/core/fstream_guard.hxx>

//std
#include <filesystem>
#include <fstream>
#include <string>


#ifdef _ENABLE_LOG_
#define IF_LOGGER_ENABLED(true_p, false_p) true_p
#define ENABLE_IF_LOGGER_ENABLED(code_p) code_p
#else
#define IF_LOGGER_ENABLED(true_p, false_p) false_p
#define ENABLE_IF_LOGGER_ENABLED(code_p)
#endif




BEGIN_NAMESPACE(FE::log)


class fatal_error_logger_base;

class message_logger_base;


class logger_base
{
public:
    _MAYBE_UNUSED_ static constexpr uint16 line_info_buffer_size = 32;
    _MAYBE_UNUSED_ static constexpr uint32 default_buffer_size = _ALLOWED_DIRECTORY_LENGTH_;

    using directory_buffer_type = std::basic_string<var::directory_char_t, std::char_traits<var::directory_char_t>>;
    using buffer_type = std::string;

protected:
    std::ofstream m_file_logger;
    FE::ofstream_guard m_file_guard;
    buffer_type  m_log_buffer;
    directory_buffer_type  m_directory_buffer;

    void __reserve() noexcept;

public:
    logger_base() noexcept;
    ~logger_base() noexcept = default;

    void set_root_directory(directory_char_t* root_directory_name_p = (std::filesystem::current_path() / "Logs").c_str()) noexcept;

    void mkdir(directory_char_t* folder_name_p) noexcept;
    void cd(directory_char_t* folder_name_p) noexcept;

    static void do_log(std::initializer_list<const void*> arguments_p) noexcept;


    template<class FatalErrorLogger>
    static fatal_error_logger_base& get_fatal_error_logger() noexcept
    {
        FE_STATIC_ASSERT(((std::is_base_of<fatal_error_logger_base, FatalErrorLogger>::value == false) && (std::is_same<fatal_error_logger_base, FatalErrorLogger>::value == false)), "FatalErrorLogger must be derived from logger_base.");
        thread_local static std::unique_ptr<fatal_error_logger_base> tl_s_fatal_error_logger = std::make_unique<FatalErrorLogger>();
        return *tl_s_fatal_error_logger;
    }

    template<class Logger>
    static message_logger_base& get_logger() noexcept
    {
        FE_STATIC_ASSERT(((std::is_base_of<message_logger_base, Logger>::value == false) && (std::is_same<message_logger_base, Logger>::value == false)), "FatalErrorLogger must be derived from logger_base.");
        thread_local static std::unique_ptr<message_logger_base> tl_s_fatal_error_logger = std::make_unique<Logger>();
        return *tl_s_fatal_error_logger;
    }
};


// It logs into file and console
class fatal_error_logger_base : public logger_base
{
public:
    using base_type = logger_base;

    fatal_error_logger_base() noexcept;
    ~fatal_error_logger_base() noexcept;

    void do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept;
};


// It logs into file and console
class message_logger_base : public logger_base
{
public:
    using base_type = logger_base;

    message_logger_base() noexcept;
    ~message_logger_base() noexcept;

    void do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept;

};


END_NAMESPACE


#endif