#ifndef _FE_CORE_DO_ONCE_HXX_
#define _FE_CORE_DO_ONCE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

// std function for do once
#include <functional>


namespace FE::internal
{
	struct do_once
	{
		_FORCE_INLINE_ do_once(const std::function<void()>& code_p) noexcept
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
	frequency ::FE::internal::do_once __FE_DO_ONCE_INSTANCE__( [&]() { __VA_ARGS__; } ); \
}


#endif