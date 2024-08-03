#include <FE/core/log/logger.hpp>
#pragma warning(disable: 4996)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#define _CRT_SECURE_NO_WARNINGS
#include <FE/core/algorithm/string.hxx>
#include <FE/core/clock.hpp>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>




BEGIN_NAMESPACE(FE::log)


void fatal_error_logger_base::do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept
{
    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

    FE::algorithm::string::concatenate<var::ASCII>
        (
            this->m_log_buffer.data(),
            default_buffer_size,
            {
                "[Time: ", FE::clock::get_current_local_time(), "]\t", message_p, "\n",
                "File Directory: ", file_name_p, "\n",
                "Function Name: ", function_name_p, "\n",
                "Code Line Number: ", l_source_code_line_info_buffer
            }
    );

    std::cerr << this->m_log_buffer.data() << "\n\n\n\n\n";


    boost::stacktrace::stacktrace l_stack_trace_dumps;
    std::string l_stack_trace_dumps_string = boost::stacktrace::to_string(l_stack_trace_dumps);


    std::cerr << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

    std::cerr << l_stack_trace_dumps_string.data() << '\n';

    std::cerr << "\n-------------------------------------------------------- END OF RECORD -------------------------------------------------------\n";
}


void message_logger_base::do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept
{
    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

    FE::algorithm::string::concatenate<var::ASCII>
        (
            this->m_log_buffer.data(),
            default_buffer_size,
            {
                "[Time: ", FE::clock::get_current_local_time(), " | File Directory: ", file_name_p, " | Function Name: ", function_name_p, " | Code Line Number: ", l_source_code_line_info_buffer,
                "\nActual Message:\n\n",
                message_p, "\n\n\n\n\n"
            }
    );

    std::cerr << this->m_log_buffer.data();

    // re-calculate length
    this->m_log_buffer = this->m_log_buffer.c_str();
    std::memset(this->m_log_buffer.data(), null, this->m_log_buffer.length() * sizeof(typename buffer_type::value_type));
}


END_NAMESPACE
