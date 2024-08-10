#ifndef _FE_CORE_PRIVATE_STRLEN_HXX_
#define _FE_CORE_PRIVATE_STRLEN_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/types.hxx>
#include <FE/private/debug.h>




namespace FE::internal
{
    template<typename CharT>
    _NODISCARD_ _FORCE_INLINE_ _CONSTEXPR17_ uint64 strlen(const CharT* const str_p) noexcept
    {
        FE_STATIC_ASSERT((sizeof(CharT) > sizeof(UTF32)), "Static assertion failure: failed to assert that the size of CharT must not be greater than the size of UTF32.");
        const CharT* l_iterator = str_p;

        if (str_p == nullptr || *str_p == null)
        {
            return 0;
        }

        while (*(++l_iterator) != null) {}

        return static_cast<uint64>(l_iterator - str_p);
    }
}

#endif