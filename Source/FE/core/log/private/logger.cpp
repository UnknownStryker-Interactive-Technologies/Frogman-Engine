#include <FE/core/log/logger.hpp>
#pragma warning(disable: 4996)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#define _CRT_SECURE_NO_WARNINGS
#include <FE/miscellaneous/misc.h>
#include <FE/core/algorithm/string.hxx>
#include <FE/core/clock.hpp>
#include <FE/core/thread.hpp>
#include <FE/core/log/logger.hpp>
#include <boost/stacktrace.hpp>
#include <csignal>
#include <filesystem>




BEGIN_NAMESPACE(FE::log)


logger::logger() noexcept : m_file_logger(), m_file_guard(m_file_logger)
{
    this->m_log_buffer.reserve(default_buffer_size);
    std::memset(this->m_log_buffer.data(), _NULL_, this->m_log_buffer.capacity() * sizeof(typename buffer_type::value_type));

    this->m_directory_buffer.reserve(default_buffer_size);
    std::memset(this->m_directory_buffer.data(), _NULL_, this->m_directory_buffer.capacity() * sizeof(typename directory_buffer_type::value_type));

    this->set_root_directory(
        //(logger::s_root_directory_argument == nullptr) ?
        //(std::filesystem::current_path() / "Logs").c_str() : logger::s_root_directory_argument
    );
}

void logger::set_root_directory(const directory_char_type* root_directory_name_p) noexcept
{
    FE_ABORT_IF(root_directory_name_p == nullptr, "root_directory_name_p was nullptr.");

    std::filesystem::path l_directory_name = root_directory_name_p;

    if (std::filesystem::exists(l_directory_name) == false)
    {
        std::filesystem::create_directory(l_directory_name);
    }
    
    this->m_directory_buffer.reserve(default_buffer_size);
    this->m_directory_buffer = l_directory_name.c_str();
}

void logger::mkdir(const directory_char_type* folder_name_p) noexcept
{
    FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    FE_ABORT_IF(folder_name_p == nullptr, "folder_name_p was nullptr.");

#ifdef _WINDOWS_X86_64_
    this->m_directory_buffer += L"\\";
#elif defined(_LINUX_X86_64_)
    this->m_directory_buffer += "//";
#endif

    this->m_directory_buffer += folder_name_p;
    std::filesystem::path l_directory_name = this->m_directory_buffer.c_str();

    if (std::filesystem::exists(l_directory_name) == false)
    {
        ::std::filesystem::create_directory(l_directory_name);
    }
    cd(STRING(..));
}

void logger::cd(const directory_char_type* folder_name_p) noexcept
{
    FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    FE_ABORT_IF(folder_name_p == nullptr, "folder_name_p was nullptr.");

    SWITCH(folder_name_p)
    {
    CASE(".."):
    {
        auto l_reverse_iterator = this->m_directory_buffer.rbegin();
#ifdef _WINDOWS_X86_64_
        while (*l_reverse_iterator != L'\\')
#elif defined(_LINUX_X86_64_)
        while (*l_reverse_iterator != '/')
#endif
        {
            ++l_reverse_iterator;
        }
        *l_reverse_iterator = _NULL_;
        this->m_directory_buffer = this->m_directory_buffer.c_str();
    }
        break;

    default:
    {
#ifdef _WINDOWS_X86_64_
        this->m_directory_buffer += L"\\";
#elif defined(_LINUX_X86_64_)
        m_directory_buffer += "//";
#endif

        this->m_directory_buffer += folder_name_p;
        FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    }
        break;
    }
}

void logger::do_log(const char* content_p, const directory_char_type* filename_p) noexcept
{
    FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    FE_ABORT_IF(content_p == nullptr, "content_p was nullptr.");
    FE_ABORT_IF(filename_p == nullptr, "filename_p was nullptr.");

    std::filesystem::path l_directory_name = this->m_directory_buffer.c_str();

    l_directory_name /= filename_p;

    this->m_file_logger.open(l_directory_name);
    this->m_file_logger << content_p;
    std::cerr << content_p;
}

void logger::__reserve() noexcept
{
    this->m_log_buffer.reserve(default_buffer_size);
    this->m_directory_buffer.reserve(default_buffer_size);
}




fatal_error_log::fatal_error_log() noexcept : base_type()
{
#ifdef _WINDOWS_X86_64_
    var::wchar l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = L"\0";
    std::mbstowcs(l_current_local_time_buffer, FE::clock::get_current_local_time(), FE::clock::current_local_time_buffer_size);

    var::wchar l_thread_id[FE::thread::max_thread_id_digit_length] = L"\0";
    std::swprintf(l_thread_id, FE::thread::max_thread_id_digit_length, L"%llu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    FE::log::logger::mkdir(STRING(Crash Report));
    FE::log::logger::cd(STRING(Crash Report));
    std::filesystem::path l_path_to_log_file = this->m_directory_buffer.c_str();
    var::wchar l_path_to_log_file_buffer[allowed_directory_string_length] = L"\0";
    FE::algorithm::string::concatenate<var::wchar>
    (
            l_path_to_log_file_buffer,
            allowed_directory_string_length,
            {
                l_path_to_log_file.c_str(),
                L"\\thread ",
                l_thread_id,
                L" @ ",
                l_current_local_time_buffer,
                 L".txt"
            }
    );

#elif defined(_LINUX_X86_64_)
    var::character l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = "\0";
    std::strcpy(l_current_local_time_buffer, FE::clock::get_current_local_time());

    std::filesystem::path l_path_to_log_file = std::filesystem::path{ m_directory_buffer.c_str() } / l_current_local_time_buffer;

    var::character l_thread_id[FE::thread::max_thread_id_digit_length] = "\0";
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, "%llu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::character l_path_to_log_file_buffer[allowed_directory_string_length] = "\0";
    FE::algorithm::string::concatenate<var::character>
    (
            l_path_to_log_file_buffer,
            allowed_directory_string_length,
            {
                l_path_to_log_file.c_str(),
                "/thread ",
                l_thread_id,
                " @ ",
                l_current_local_time_buffer,
                ".txt"
            }
    );

#endif
    this->m_file_logger.open(l_path_to_log_file_buffer);

    FE::log::logger::cd(STRING(..));

    this->m_file_logger << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    this->m_file_logger << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";

    std::cerr << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    std::cerr << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";
}

fatal_error_log::~fatal_error_log() noexcept
{
    this->m_file_guard.close();
}

void fatal_error_log::do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept
{
    if (this->m_log_buffer.capacity() <= default_buffer_size) _UNLIKELY_
    {
        this->__reserve();
    }

    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

    FE::algorithm::string::concatenate<var::character>
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

    this->m_file_logger << this->m_log_buffer.data() << "\n\n\n\n\n";
    std::cerr << this->m_log_buffer.data() << "\n\n\n\n\n";


    boost::stacktrace::stacktrace l_stack_trace_dumps;
    std::string l_stack_trace_dumps_string = std::move(boost::stacktrace::to_string(l_stack_trace_dumps));

    this->m_file_logger << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

    this->m_file_logger << l_stack_trace_dumps_string.data() << '\n';

    this->m_file_logger << "\n-------------------------------------------------------- END OF RECORD -------------------------------------------------------\n";




    std::cerr << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

    std::cerr << l_stack_trace_dumps_string.data() << '\n';

    std::cerr << "\n-------------------------------------------------------- END OF RECORD -------------------------------------------------------\n";
}




file_log::file_log() noexcept : base_type()
{
#ifdef _WINDOWS_X86_64_
    var::wchar l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = L"\0";
    std::mbstowcs(l_current_local_time_buffer, FE::clock::get_current_local_time(), FE::clock::current_local_time_buffer_size);

    var::wchar l_thread_id[FE::thread::max_thread_id_digit_length] = L"\0";
    std::swprintf(l_thread_id, FE::thread::max_thread_id_digit_length, L"%llu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    FE::log::logger::mkdir(l_current_local_time_buffer);
    FE::log::logger::cd(l_current_local_time_buffer);
    std::filesystem::path l_path_to_log_file = this->m_directory_buffer.c_str();
    var::wchar l_path_to_log_file_buffer[allowed_directory_string_length] = L"\0";
    FE::algorithm::string::concatenate<var::wchar>
        (
            l_path_to_log_file_buffer,
            allowed_directory_string_length,
            {
                l_path_to_log_file.c_str(),
                L"\\thread ",
                l_thread_id,
                L" @ ",
                l_current_local_time_buffer,
                 L".txt"
            }
    );

#elif defined(_LINUX_X86_64_)
    var::character l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = "\0";
    std::strcpy(l_current_local_time_buffer, FE::clock::get_current_local_time());

    std::filesystem::path l_path_to_log_file = std::filesystem::path{ m_directory_buffer.c_str() } / l_current_local_time_buffer;

    var::character l_thread_id[FE::thread::max_thread_id_digit_length] = "\0";
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, "%llu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::character l_path_to_log_file_buffer[allowed_directory_string_length] = "\0";
    FE::algorithm::string::concatenate<var::character>
        (
            l_path_to_log_file_buffer,
            allowed_directory_string_length,
            {
                l_path_to_log_file.c_str(),
                "/thread ",
                l_thread_id,
                " @ ",
                l_current_local_time_buffer,
                ".txt"
            }
    );

#endif
    this->m_file_logger.open(l_path_to_log_file_buffer);

    FE::log::logger::cd(STRING(..));

    this->m_file_logger << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    this->m_file_logger << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";

    std::cerr << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    std::cerr << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";
}

file_log::~file_log() noexcept
{
    this->m_file_logger << "\n-------------------------------------------------- END OF LOG --------------------------------------------------\n\n";

    this->m_file_guard.close();

    std::cerr << "\n-------------------------------------------------- END OF LOG --------------------------------------------------\n\n";
}

void file_log::do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept
{
    if (this->m_log_buffer.capacity() <= default_buffer_size) _UNLIKELY_
    {
        __reserve();
    }

    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

    FE::algorithm::string::concatenate<var::character>
        (
            this->m_log_buffer.data(),
            default_buffer_size,
            {
                "[Time: ", FE::clock::get_current_local_time(), " | File Directory: ", file_name_p, " | Function Name: ", function_name_p, " | Code Line Number: ", l_source_code_line_info_buffer,
                "\nActual Message:\n\n",
                message_p, "\n\n\n\n\n"
            }
    );

    this->m_file_logger << this->m_log_buffer.data();
    std::cerr << this->m_log_buffer.data();

    // re-calculate length
    this->m_log_buffer = this->m_log_buffer.c_str();
    std::memset(this->m_log_buffer.data(), _NULL_, this->m_log_buffer.length() * sizeof(typename buffer_type::value_type));
}

END_NAMESPACE
