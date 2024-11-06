#ifndef _FE_CORE_PRIVATE_STRLEN_HXX_
#define _FE_CORE_PRIVATE_STRLEN_HXX_
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




namespace FE::internal
{
    template<typename CharT>
    _FE_NODISCARD_ _FE_FORCE_INLINE_ _FE_CONSTEXPR17_ uint64 strlen(const CharT* const str_p) noexcept
    {
        FE_NEGATIVE_STATIC_ASSERT((sizeof(CharT) > sizeof(UTF32)), "Static assertion failure: failed to assert that the size of CharT must not be greater than the size of UTF32.");
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