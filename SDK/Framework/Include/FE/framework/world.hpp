#ifndef _FE_FRAMEWORK_WORLD_HPP_
#define _FE_FRAMEWORK_WORLD_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/framework/archetype_base.hpp>




BEGIN_NAMESPACE(FE)


class world : public FE::archetype_base
{
public:
	world() noexcept;
	~world() noexcept;

protected:
};


END_NAMESPACE
#endif

