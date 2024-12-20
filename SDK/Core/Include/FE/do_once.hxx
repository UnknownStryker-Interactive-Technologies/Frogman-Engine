#ifndef _FE_CORE_DO_ONCE_HXX_
#define _FE_CORE_DO_ONCE_HXX_
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




namespace FE::internal
{
	class do_once
	{
	public:
		template<typename Lambda>
		inline do_once(Lambda&& code_p) noexcept
		{
			code_p();
		}
	};
}

#ifdef _DO_ONCE_PER_APP_EXECUTION_
    #error _DO_ONCE_PER_APP_EXECUTION_ is a reserved Frogman Engine macro keyword.
#endif 
// A directive for executing a given code once per an application launch.
#define _DO_ONCE_PER_APP_EXECUTION_ static

#ifdef _DO_ONCE_AT_THREAD_PROCESS_
    #error _DO_ONCE_AT_THREAD_PROCESS_ is a reserved Frogman Engine macro keyword.
#endif 
// A directive for executing a given code once per thread.
#define _DO_ONCE_AT_THREAD_PROCESS_ thread_local static 

#ifdef FE_DO_ONCE
    #error FE_DO_ONCE is a reserved Frogman Engine macro keyword.
#endif
#define FE_DO_ONCE(frequency, ...)\
{ \
 	_FE_MAYBE_UNUSED_ frequency ::FE::internal::do_once __FE_DO_ONCE_INSTANCE__( [&]() { __VA_ARGS__; } ); \
}


#endif