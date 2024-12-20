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
#include <FE/log/logger.hpp>
#include <FE/algorithm/string.hxx>
#include <FE/clock.hpp>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>




BEGIN_NAMESPACE(FE::log)


logger_base::logger_base() noexcept : m_log_buffer()
{
    this->m_log_buffer.reserve(default_buffer_size);
    std::memset(this->m_log_buffer.data(), null, this->m_log_buffer.capacity() * sizeof(typename buffer_type::value_type));
}


// DO NOT USE in-house library functions within this file, except for FE::algorithm::string::concatenate<CharT>().
// The functions in this file are used to implement FE_ASSERT, FE_NEGATIVE_ASSERT, FE_EXIT, etc. Calling them within this file will result in an infinite recursive loop.
void fatal_error_logger_base::do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept
{
    if (std::strlen(message_p) >= default_buffer_size) _FE_UNLIKELY_
    {
        std::cerr << "The message is too long, the string must be shorter than the default_buffer_size.";
        std::exit(FE::error_code_cast(FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow));
    }
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
    if (std::strlen(message_p) >= default_buffer_size) _FE_UNLIKELY_
    {
        std::cerr << "The message is too long, the string must be shorter than the default_buffer_size.";
        std::exit(FE::error_code_cast(FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow));
    }
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

    std::cout << this->m_log_buffer.data();

    std::memset(this->m_log_buffer.data(), null, default_buffer_size * sizeof(typename buffer_type::value_type));
}


END_NAMESPACE

