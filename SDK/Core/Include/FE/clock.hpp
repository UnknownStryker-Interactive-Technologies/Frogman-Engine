#ifndef _FE_CORE_CLOCK_HPP_
#define _FE_CORE_CLOCK_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/types.hxx>

// boost
#include <boost/chrono.hpp>

// std
#include <memory>




BEGIN_NAMESPACE(FE)


class clock final
{
public:
    using char_type = char;

    // const char* FE::clock::get_current_local_time() noexcept;
    _FE_MAYBE_UNUSED_ static constexpr int current_local_time_buffer_size = 64;

private:
    boost::chrono::steady_clock::time_point m_starting_point;
    boost::chrono::steady_clock::time_point m_ending_point;
    boost::chrono::duration<var::float64, boost::milli> m_delta_time;

public:
    _FE_CONSTEXPR17_ clock() noexcept : m_starting_point(), m_ending_point(), m_delta_time() {}
    _FE_FORCE_INLINE_ ~clock() noexcept {}

    _FE_FORCE_INLINE_ void start_clock() noexcept { this->m_starting_point = boost::chrono::steady_clock::now(); }
    _FE_FORCE_INLINE_ void end_clock() noexcept { this->m_ending_point = boost::chrono::steady_clock::now(); }

    _FE_FORCE_INLINE_ float64 get_delta_time() noexcept
    {
        this->m_delta_time = this->m_ending_point - this->m_starting_point;
        return this->m_delta_time.count();
    }

    static const char_type* get_current_local_time() noexcept;
};


END_NAMESPACE
#endif