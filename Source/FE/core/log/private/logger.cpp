#include <FE/core/log/logger.hpp>
#pragma warning(disable: 4996)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#define _CRT_SECURE_NO_WARNINGS
#include <FE/core/algorithm/string.hxx>
#include <FE/core/clock.hpp>
#include <FE/core/log/format_string.h>
#include <FE/core/thread.hpp>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>
#include <filesystem>




BEGIN_NAMESPACE(FE::log)


logger_base::logger_base() noexcept : m_file_logger(), m_file_guard(m_file_logger)
{
    this->m_log_buffer.reserve(default_buffer_size);
<<<<<<< HEAD
    std::memset(this->m_log_buffer.data(), null, this->m_log_buffer.capacity() * sizeof(typename buffer_type::value_type));

    this->m_directory_buffer.reserve(default_buffer_size);
    std::memset(this->m_directory_buffer.data(), null, _ALLOWED_DIRECTORY_LENGTH_ * sizeof(directory_char_t));
=======
    std::memset(this->m_log_buffer.data(), _FE_NULL_, this->m_log_buffer.capacity() * sizeof(typename buffer_type::value_type));

    this->m_directory_buffer.reserve(default_buffer_size);
    std::memset(this->m_directory_buffer.data(), _FE_NULL_, _ALLOWED_DIRECTORY_LENGTH_ * sizeof(directory_char_t));
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

    this->set_root_directory((std::filesystem::current_path() / "Logs").c_str());
}

void logger_base::set_root_directory(directory_char_t* root_directory_name_p) noexcept
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

void logger_base::mkdir(directory_char_t* folder_name_p) noexcept
{
    FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    FE_ABORT_IF(folder_name_p == nullptr, "folder_name_p was nullptr.");

#ifdef _WINDOWS_X86_64_
    this->m_directory_buffer += L"\\";
#else
    #ifdef _LINUX_X86_64_
    this->m_directory_buffer += "/";
    #endif
#endif

    this->m_directory_buffer += folder_name_p;
    std::filesystem::path l_directory_name = this->m_directory_buffer.c_str();

    if (std::filesystem::exists(l_directory_name) == false)
    {
        ::std::filesystem::create_directory(l_directory_name);
    }
    cd(STRING(..));
}

void logger_base::cd(directory_char_t* folder_name_p) noexcept
{
    FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    FE_ABORT_IF(folder_name_p == nullptr, "folder_name_p was nullptr.");

    SWITCH(folder_name_p)
    {
    CASE(STRING(..)):
    {
        auto l_reverse_iterator = this->m_directory_buffer.rbegin();
#ifdef _WINDOWS_X86_64_
        while (*l_reverse_iterator != L'\\')
#else
    #ifdef _LINUX_X86_64_
        while (*l_reverse_iterator != '/')
    #endif
#endif
        {
            ++l_reverse_iterator;
        }
<<<<<<< HEAD
        *l_reverse_iterator = null;
=======
        *l_reverse_iterator = _FE_NULL_;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
        this->m_directory_buffer = this->m_directory_buffer.c_str();
    }
        break;

    default:
    {
#ifdef _WINDOWS_X86_64_
        this->m_directory_buffer += L"\\";
#else
    #ifdef _LINUX_X86_64_
        m_directory_buffer += "/";
    #endif
#endif

        this->m_directory_buffer += folder_name_p;
        FE_ABORT_IF(std::filesystem::exists(this->m_directory_buffer.c_str()) == false, "Current directory is invalid.");
    }
        break;
    }
}

void logger_base::do_log(std::initializer_list<const void*> arguments_p) noexcept
{
    std::cerr << ::FE::log::buffered_string_formatter(arguments_p);
}

void logger_base::__reserve() noexcept
{
    this->m_log_buffer.reserve(default_buffer_size);
    this->m_directory_buffer.reserve(default_buffer_size);
}




fatal_error_logger_base::fatal_error_logger_base() noexcept : base_type()
{
#ifdef _WINDOWS_X86_64_
    var::wchar l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = STRING(\0);
    std::mbstowcs(l_current_local_time_buffer, FE::clock::get_current_local_time(), FE::clock::current_local_time_buffer_size);

    var::wchar l_thread_id[FE::thread::max_thread_id_digit_length] = STRING(\0);
    std::swprintf(l_thread_id, FE::thread::max_thread_id_digit_length, STRING(%lu), FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    FE::log::logger_base::mkdir(STRING(Crash Report));
    FE::log::logger_base::cd(STRING(Crash Report));
    std::filesystem::path l_path_to_log_file = this->m_directory_buffer.c_str();
    var::wchar l_path_to_log_file_buffer[default_buffer_size - 1] = STRING(\0);
    FE::algorithm::string::concatenate<var::wchar>
    (
            l_path_to_log_file_buffer,
            default_buffer_size,
            {
                l_path_to_log_file.c_str(),
                STRING(\\thread ),
                l_thread_id,
                STRING( @ ),
                l_current_local_time_buffer,
                STRING(.txt)
            }
    );

#else
#ifdef _LINUX_X86_64_
<<<<<<< HEAD
    var::ASCII l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = STRING(\0);
=======
    var::character l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = STRING(\0);
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    std::strcpy(l_current_local_time_buffer, FE::clock::get_current_local_time());

    std::filesystem::path l_path_to_log_file = std::filesystem::path{ m_directory_buffer.c_str() } / l_current_local_time_buffer;

<<<<<<< HEAD
    var::ASCII l_thread_id[FE::thread::max_thread_id_digit_length] = STRING(\0);
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, STRING(%lu), FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::ASCII l_path_to_log_file_buffer[default_buffer_size - 1] = STRING(\0);
    FE::algorithm::string::concatenate<var::ASCII>
=======
    var::character l_thread_id[FE::thread::max_thread_id_digit_length] = STRING(\0);
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, STRING(%lu), FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::character l_path_to_log_file_buffer[default_buffer_size - 1] = STRING(\0);
    FE::algorithm::string::concatenate<var::character>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    (
            l_path_to_log_file_buffer,
            default_buffer_size,
            {
                l_path_to_log_file.c_str(),
                STRING(/thread ),
                l_thread_id,
                STRING( @ ),
                l_current_local_time_buffer,
                STRING(.txt)
            }
    );
#endif
#endif
    this->m_file_logger.open(l_path_to_log_file_buffer, std::ios::out);

    FE::log::logger_base::cd(STRING(..));

    this->m_file_logger << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    this->m_file_logger << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";

    std::cerr << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    std::cerr << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";
}

fatal_error_logger_base::~fatal_error_logger_base() noexcept
{
}

<<<<<<< HEAD
void fatal_error_logger_base::do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept
=======
void fatal_error_logger_base::do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
{
    if (UNLIKELY(this->m_log_buffer.capacity() <= default_buffer_size)) _UNLIKELY_
    {
        this->__reserve();
    }

    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

<<<<<<< HEAD
    FE::algorithm::string::concatenate<var::ASCII>
=======
    FE::algorithm::string::concatenate<var::character>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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
    std::string l_stack_trace_dumps_string = boost::stacktrace::to_string(l_stack_trace_dumps);

    this->m_file_logger << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

    this->m_file_logger << l_stack_trace_dumps_string.data() << '\n';

    this->m_file_logger << "\n-------------------------------------------------------- END OF RECORD -------------------------------------------------------\n";




    std::cerr << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

    std::cerr << l_stack_trace_dumps_string.data() << '\n';

    std::cerr << "\n-------------------------------------------------------- END OF RECORD -------------------------------------------------------\n";
<<<<<<< HEAD

    this->m_file_logger.close();
=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
}




message_logger_base::message_logger_base() noexcept : base_type()
{
#ifdef _WINDOWS_X86_64_
    var::wchar l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = L"\0";
    std::mbstowcs(l_current_local_time_buffer, FE::clock::get_current_local_time(), FE::clock::current_local_time_buffer_size);

    var::wchar l_thread_id[FE::thread::max_thread_id_digit_length] = L"\0";
    std::swprintf(l_thread_id, FE::thread::max_thread_id_digit_length, L"%llu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    FE::log::logger_base::mkdir(l_current_local_time_buffer);
    FE::log::logger_base::cd(l_current_local_time_buffer);
    std::filesystem::path l_path_to_log_file = this->m_directory_buffer.c_str();
    var::wchar l_path_to_log_file_buffer[default_buffer_size - 1] = L"\0";
    FE::algorithm::string::concatenate<var::wchar>
        (
            l_path_to_log_file_buffer,
            default_buffer_size,
            {
                l_path_to_log_file.c_str(),
                L"\\thread ",
                l_thread_id,
                L" @ ",
                l_current_local_time_buffer,
                 L".txt"
            }
    );

#else
#ifdef _LINUX_X86_64_
<<<<<<< HEAD
    var::ASCII l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = "\0";
=======
    var::character l_current_local_time_buffer[FE::clock::current_local_time_buffer_size] = "\0";
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    std::strcpy(l_current_local_time_buffer, FE::clock::get_current_local_time());

    std::filesystem::path l_path_to_log_file = std::filesystem::path{ m_directory_buffer.c_str() } / l_current_local_time_buffer;

<<<<<<< HEAD
    var::ASCII l_thread_id[FE::thread::max_thread_id_digit_length] = "\0";
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, "%lu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::ASCII l_path_to_log_file_buffer[default_buffer_size - 1] = "\0";
    FE::algorithm::string::concatenate<var::ASCII>
=======
    var::character l_thread_id[FE::thread::max_thread_id_digit_length] = "\0";
    std::snprintf(l_thread_id, FE::thread::max_thread_id_digit_length, "%lu", FE::thread::this_thread_id()); // hashed thread-ids from std::hash are too long and hard to read 

    var::character l_path_to_log_file_buffer[default_buffer_size - 1] = "\0";
    FE::algorithm::string::concatenate<var::character>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
        (
            l_path_to_log_file_buffer,
            default_buffer_size,
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
#endif
    this->m_file_logger.open(l_path_to_log_file_buffer, std::ios::out);

    FE::log::logger_base::cd(STRING(..));

    this->m_file_logger << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    this->m_file_logger << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";

    std::cerr << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
    std::cerr << "\n-------------------------------------------------- BEGIN LOG --------------------------------------------------\n\n";
}

message_logger_base::~message_logger_base() noexcept
{
    this->m_file_logger << "\n-------------------------------------------------- END OF LOG --------------------------------------------------\n\n";
    std::cerr << "\n-------------------------------------------------- END OF LOG --------------------------------------------------\n\n";
}

<<<<<<< HEAD
void message_logger_base::do_log(ASCII* const message_p, ASCII* const file_name_p, ASCII* const function_name_p, uint32 line_p) noexcept
=======
void message_logger_base::do_log(character* const message_p, character* const file_name_p, character* const function_name_p, uint32 line_p) noexcept
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
{
    if (UNLIKELY(this->m_log_buffer.capacity() <= default_buffer_size)) _UNLIKELY_
    {
        __reserve();
    }

    typename buffer_type::value_type l_source_code_line_info_buffer[line_info_buffer_size] = "\0";

    std::snprintf(l_source_code_line_info_buffer, line_info_buffer_size, "%u", line_p);

<<<<<<< HEAD
    FE::algorithm::string::concatenate<var::ASCII>
=======
    FE::algorithm::string::concatenate<var::character>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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
<<<<<<< HEAD
    std::memset(this->m_log_buffer.data(), null, this->m_log_buffer.length() * sizeof(typename buffer_type::value_type));
=======
    std::memset(this->m_log_buffer.data(), _FE_NULL_, this->m_log_buffer.length() * sizeof(typename buffer_type::value_type));
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
}

END_NAMESPACE

