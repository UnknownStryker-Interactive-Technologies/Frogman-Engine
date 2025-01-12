#ifndef _FE_CORE_CLOCK_HPP_
#define _FE_CORE_CLOCK_HPP_
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
    boost::chrono::duration<var::float64, boost::milli> m_delta;

public:
    clock() noexcept = default;
    ~clock() noexcept = default;

    _FE_FORCE_INLINE_ void start_clock() noexcept { this->m_starting_point = boost::chrono::steady_clock::now(); }
    _FE_FORCE_INLINE_ void end_clock() noexcept { this->m_ending_point = boost::chrono::steady_clock::now(); }

    _FE_FORCE_INLINE_ float64 get_delta_milliseconds() noexcept
    {
        this->m_delta = this->m_ending_point - this->m_starting_point;
        return this->m_delta.count();
    }

    static const char_type* get_current_local_time() noexcept;
};


END_NAMESPACE
#endif