#ifndef _FE_FRAMEWORK_SYSTEM_BASE_HPP_
#define _FE_FRAMEWORK_SYSTEM_BASE_HPP_
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
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)


class component_base;


class system_base
{
protected:
	std::weak_ptr<system_base> m_dependancy;

public:
	system_base() noexcept = default;
	virtual ~system_base() noexcept = 0;

	virtual void operator()(component_base* const component_p) = 0;
};

END_NAMESPACE
#endif