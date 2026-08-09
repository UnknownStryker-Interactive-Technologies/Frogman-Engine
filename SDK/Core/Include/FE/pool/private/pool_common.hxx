#ifndef _FE_CORE_PRIVATE_POOL_COMMON_HXX_
#define _FE_CORE_PRIVATE_POOL_COMMON_HXX_
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
#include <FE/do_once.hxx>
#include <FE/list.hxx>
#include <FE/memory.hxx>
#include <FE/type_traits.hxx>

// third-party
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
#include <absl/container/flat_hash_set.h> // absl::flat_hash_set for page base pointer validation
#endif

// std
#include <algorithm> //  std::sort() for defragmentation
#include <execution> // std::execution::par
#include <memory_resource> // std::pmr::memory_resource


#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memoryapi.h> // to use VirtualAlloc and VirtualFree
#endif




BEGIN_NAMESPACE(FE)


enum struct PoolType : uint8
{
    _Block = 0,
    _Scalable = 1,
    _BlockLargePage = 2,
    _ScalableLargePage = 3
};

enum struct PageGroup : uint8
{
    _AvailablePages = 0,
    _UnavailablePages = 1
};

template<PoolType PoolType, class Alignment>
class pool;


namespace internal::pool
{
    template<PoolType PoolType, class Alignment>
    class chunk;

    struct block_info
    {
        var::int32 _address;
        var::int32 _size_in_bytes;
    };

    class from_low_address
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._address < rhs_p._address;
        }
    };

    class less_than
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._size_in_bytes < rhs_p._size_in_bytes;
        }
    };

    class greater_than
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._size_in_bytes > rhs_p._size_in_bytes;
        }
    };

    _FE_FORCE_INLINE_ bool operator==(const block_info& lhs_p, const block_info& rhs_p) noexcept
    {
        return lhs_p._address == rhs_p._address;
    }

    _FE_FORCE_INLINE_ bool operator!=(const block_info& lhs_p, const block_info& rhs_p) noexcept
    {
        return lhs_p._address != rhs_p._address;
    }

    namespace large
    {
        struct block_info
        {
            var::int32 _address;
            var::int32 _size_in_bytes;
        };

        class from_low_address
        {
        public:
            _FE_FORCE_INLINE_ bool operator()(const large::block_info& lhs_p, const large::block_info& rhs_p) noexcept
            {
                return lhs_p._address < rhs_p._address;
            }
        };

        class less_than
        {
        public:
            _FE_FORCE_INLINE_ bool operator()(const large::block_info& lhs_p, const large::block_info& rhs_p) noexcept
            {
                return lhs_p._size_in_bytes < rhs_p._size_in_bytes;
            }
        };

        class greater_than
        {
        public:
            _FE_FORCE_INLINE_ bool operator()(const large::block_info& lhs_p, const large::block_info& rhs_p) noexcept
            {
                return lhs_p._size_in_bytes > rhs_p._size_in_bytes;
            }
        };

        _FE_FORCE_INLINE_ bool operator==(const large::block_info& lhs_p, const large::block_info& rhs_p) noexcept
        {
            return lhs_p._address == rhs_p._address;
        }

        _FE_FORCE_INLINE_ bool operator!=(const large::block_info& lhs_p, const large::block_info& rhs_p) noexcept
        {
            return lhs_p._address != rhs_p._address;
        }
    }

    template <FE::size InBytes>
    class uninitialized_bytes
    {
        var::byte m_page[InBytes];
    };

    enum struct PageListClass : FE::int8
    {
        _Unavailable = -1, // 0B
        _6_25_PercentRemaining = 0, // 256B
        _12_5_Percent = 1, // 512B
        _25_Percent = 2,   // 1KiB
        _50_Percent = 3,   // 2KiB
        _75_Percent = 4,   // 3KiB
        _100_Percent = 5   // 4KiB
    };

    _FE_FORCE_INLINE_ void __enable_large_pages() noexcept
    {
        HANDLE l_token;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &l_token))
        {
            FE_EXIT_IF(true, FE::ErrorCode::_FatalWinAPI_Error_4XX_OpenProcessTokenFailure, "Frogman Engine Runtime Error: Failed to enable large pages.");
            return;
        }

        TOKEN_PRIVILEGES l_tp;
        LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &l_tp.Privileges[0].Luid);

        l_tp.PrivilegeCount = 1;
        l_tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        AdjustTokenPrivileges(l_token, FALSE, &l_tp, 0, NULL, NULL);
        FE_EXIT_IF(GetLastError() != 0, FE::ErrorCode::_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure, "Frogman Engine Runtime Error: Failed to enable large pages.");
        CloseHandle(l_token);
    }
}


END_NAMESPACE
#endif