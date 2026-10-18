#ifndef _FROGMAN_ENGINE_GC_BASE_HXX
#define _FROGMAN_ENGINE_GC_BASE_HXX
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
#include <memory>
#include <vector>




BEGIN_NAMESPACE(FE)


class gc_metadata_base;

class gc_base
{
    friend class gc_metadata_base;

	std::shared_ptr<std::pmr::vector<FE::gc_base*>> m_reference_edges;

public:
    gc_base() noexcept;
    virtual ~gc_base() noexcept = default;
};


END_NAMESPACE
#endif