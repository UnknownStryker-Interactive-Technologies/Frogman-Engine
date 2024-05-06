#ifndef _FE_CORE_PRIVATE_STRLEN_H_
#define _FE_CORE_PRIVATE_STRLEN_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>


namespace FE::internal
{
    template<typename CharT>
    _NODISCARD_ _CONSTEXPR17_ uint64 strlen(const CharT* const str_p) noexcept
    {
        const CharT* l_iterator_pointer = str_p;

        if (*str_p == _NULL_)
        {
            return 0;
        }

        while (*(++l_iterator_pointer) != _NULL_) _LIKELY_ {}

        return (l_iterator_pointer - str_p);
    }
}

#endif