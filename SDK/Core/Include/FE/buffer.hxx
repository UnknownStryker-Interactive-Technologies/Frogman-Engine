#ifndef _FE_CORE_BUFFER_HXX_
#define _FE_CORE_BUFFER_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <concepts>
#include <cstring>
#include <memory>
#include <utility>




BEGIN_NAMESPACE(FE)




template <class Allocator = std::allocator<char>>
class buffer
{
public:
    using value_type      = char;
    using allocator_type  = Allocator;
    using size_type       = typename std::allocator_traits<Allocator>::size_type;
    using difference_type = typename std::allocator_traits<Allocator>::difference_type;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

	static_assert(std::is_same_v<value_type, typename Allocator::value_type>, "Allocator must be for char type");

private:
    using alloc_traits = std::allocator_traits<Allocator>;

    pointer m_data;
    size_type m_size;
    _FE_NO_UNIQUE_ADDRESS_ Allocator m_alloc;

public:
    buffer() noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc()
    {
    }

    explicit buffer(const Allocator& alloc_p) noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc(alloc_p)
    {
    }

    explicit buffer(size_type count_p, const Allocator& alloc_p = Allocator()) noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc(alloc_p)
    {
        resize(count_p);
    }

    buffer(const buffer& other_p) noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc(alloc_traits::select_on_container_copy_construction(other_p.m_alloc))
    {
        assign_copy(other_p.data(), other_p.m_size);
    }

    buffer(const buffer& other_p, const Allocator& alloc_p) noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc(alloc_p)
    {
        assign_copy(other_p.data(), other_p.m_size);
    }

    buffer(buffer&& other_p) noexcept
        : m_data(std::exchange(other_p.m_data, nullptr))
        , m_size(std::exchange(other_p.m_size, 0))
        , m_alloc(std::move(other_p.m_alloc))
    {
    }

    buffer(buffer&& other_p, const Allocator& alloc_p) noexcept
        : m_data(nullptr)
        , m_size(0)
        , m_alloc(alloc_p)
    {
        if (m_alloc == other_p.m_alloc)
        {
            m_data = std::exchange(other_p.m_data, nullptr);
            m_size = std::exchange(other_p.m_size, 0);
        }
        else
        {
            assign_copy(other_p.data(), other_p.m_size);
        }
    }

    ~buffer() noexcept
    {
        if (m_data != nullptr)
        {
            alloc_traits::deallocate(m_alloc, m_data, m_size);
        }
    }

    buffer& operator=(const buffer& other_p) noexcept
    {
        if (this == &other_p)
        {
            return *this;
        }

        if (m_data != nullptr)
        {
            alloc_traits::deallocate(m_alloc, m_data, m_size);
        }

        if constexpr (alloc_traits::propagate_on_container_copy_assignment::value)
        {
            m_alloc = other_p.m_alloc;
        }

        assign_copy(other_p.data(), other_p.m_size);
        return *this;
    }

    buffer& operator=(buffer&& other_p) noexcept
    {
        if (this == &other_p)
        {
            return *this;
        }

        if (m_data != nullptr)
        {
            alloc_traits::deallocate(m_alloc, m_data, m_size);
        }

        if constexpr (alloc_traits::propagate_on_container_move_assignment::value)
        {
            m_alloc = std::move(other_p.m_alloc);
            m_data  = std::exchange(other_p.m_data, nullptr);
            m_size  = std::exchange(other_p.m_size, 0);
        }
        else if (m_alloc == other_p.m_alloc)
        {
            m_data = std::exchange(other_p.m_data, nullptr);
            m_size = std::exchange(other_p.m_size, 0);
        }
        else
        {
            assign_copy(other_p.data(), other_p.m_size);
        }
        return *this;
    }

    value_type* data() noexcept
    {
        return std::to_address(m_data);
    }

    const value_type* data() const noexcept
    {
        return std::to_address(m_data);
    }

    size_type size() const noexcept
    {
        return m_size;
    }

    void resize(size_type count_p) noexcept
    {
        if (count_p == m_size)
        {
            return;
        }

        pointer l_new_data = nullptr;

        if (count_p > 0)
        {
            l_new_data = alloc_traits::allocate(m_alloc, count_p);
            FE_ASSERT(l_new_data != nullptr);

            value_type* const l_raw  = std::to_address(l_new_data);
            const size_type   l_keep = (count_p < m_size) ? count_p : m_size;

            if (l_keep > 0)
            {
                std::memcpy(l_raw, data(), l_keep);
            }
            if (count_p > l_keep)
            {
                std::memset(l_raw + l_keep, 0, count_p - l_keep);
            }
        }

        if (m_data != nullptr)
        {
            alloc_traits::deallocate(m_alloc, m_data, m_size);
        }
        m_data = l_new_data;
        m_size = count_p;
    }

private:
    void assign_copy(const value_type* src_p, size_type count_p) noexcept
    {
        if (count_p == 0)
        {
            m_data = nullptr;
            m_size = 0;
            return;
        }

        FE_ASSERT(src_p != nullptr);

        m_data = alloc_traits::allocate(m_alloc, count_p);
        FE_ASSERT(m_data != nullptr);

        std::memcpy(std::to_address(m_data), src_p, count_p);
        m_size = count_p;
    }
};


END_NAMESPACE
#endif