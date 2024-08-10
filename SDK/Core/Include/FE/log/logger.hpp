#ifndef _FE_LOGGER_HPP_
#define _FE_LOGGER_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/log/format_string.h>
#include <FE/private/debug.h>
#include <FE/types.hxx>

//std
#include <memory>
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

    using buffer_type = std::string;

protected:
    buffer_type  m_log_buffer;

public:
    _FORCE_INLINE_ logger_base() noexcept : m_log_buffer()
    {
        this->m_log_buffer.reserve(default_buffer_size);
        std::memset(this->m_log_buffer.data(), null, this->m_log_buffer.capacity() * sizeof(typename buffer_type::value_type));

        std::cerr << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
        std::cerr << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";
    }

    ~logger_base() noexcept = default;

    _FORCE_INLINE_ static void do_log(std::initializer_list<const void*> arguments_p) noexcept
    {
        std::cerr << ::FE::log::buffered_string_formatter(arguments_p);
    }


    template<class FatalErrorLogger>
    _FORCE_INLINE_ static fatal_error_logger_base& get_fatal_error_logger() noexcept
    {
        FE_STATIC_ASSERT(((std::is_base_of<fatal_error_logger_base, FatalErrorLogger>::value == false) && (std::is_same<fatal_error_logger_base, FatalErrorLogger>::value == false)), "FatalErrorLogger must be derived from logger_base.");
        thread_local static std::unique_ptr<fatal_error_logger_base> tl_s_fatal_error_logger = std::make_unique<FatalErrorLogger>();
        return *tl_s_fatal_error_logger;
    }

    template<class Logger>
    _FORCE_INLINE_ static message_logger_base& get_logger() noexcept
    {
        FE_STATIC_ASSERT(((std::is_base_of<message_logger_base, Logger>::value == false) && (std::is_same<message_logger_base, Logger>::value == false)), "FatalErrorLogger must be derived from logger_base.");
        thread_local static std::unique_ptr<message_logger_base> tl_s_fatal_error_logger = std::make_unique<Logger>();
        return *tl_s_fatal_error_logger;
    }
};


class fatal_error_logger_base : public logger_base
{
public:
    using base_type = logger_base;

    _FORCE_INLINE_ fatal_error_logger_base() noexcept : base_type() {}
    ~fatal_error_logger_base() noexcept = default;

    void do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept;
};

class message_logger_base : public logger_base
{
public:
    using base_type = logger_base;

    _FORCE_INLINE_ message_logger_base() noexcept : base_type() {}
    _FORCE_INLINE_ ~message_logger_base() noexcept { std::cerr << "\n-------------------------------------------------- END OF LOG --------------------------------------------------\n\n"; }

    void do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept;
};


END_NAMESPACE
#endif