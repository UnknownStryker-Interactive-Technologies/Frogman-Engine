#ifndef _FE_CORE_SINGLETON_HXX_
#define _FE_CORE_SINGLETON_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/memory_block.hxx>


#define FRIEND_CLASS_SINGLETON(typename_p) friend class ::FE::memory_block<typename_p>; friend class ::FE::singleton<typename_p>


BEGIN_NAMESPACE(FE)

/*
singleton<T> can be used to reduce singleton code pattern duplication.
tutorial:
1. add the macro FRIEND_CLASS_SINGLETON(typename_p); to the desired class declaration.
2. set the constructors private to hide those.

For further information, please check our Unit Test Cases.
*/
template<typename T>
class singleton final
{
private:
	thread_local static FE::memory_block<T> tl_s_instance;

public:
	typedef T value_type;
	typedef FE::memory_block<T> memory_block_type;

	singleton() noexcept = delete;
	~singleton() noexcept = delete;
	singleton(const singleton& deprecated_p) noexcept = delete;
	singleton& operator=(const singleton& deprecated_p) noexcept = delete;
	singleton(singleton&& deprecated_p) noexcept = delete;
	singleton& operator=(singleton&& deprecated_p) noexcept = delete;


	_FORCE_INLINE_ static T& singleton_instance() noexcept 
	{
		if (singleton<T>::tl_s_instance.is_constructed() == false) _UNLIKELY_
		{
			singleton<T>::tl_s_instance.construct();
		}

		return *singleton<T>::tl_s_instance;
	}

	template<typename ... Arguments>
	_FORCE_INLINE_ static T& singleton_instance(Arguments&& ...arguments_p) noexcept
	{
		if (singleton<T>::tl_s_instance.is_constructed() == false) 	_UNLIKELY_
		{
			singleton<T>::tl_s_instance.construct(arguments_p...);
		}

		return *singleton<T>::tl_s_instance;
	}

	_FORCE_INLINE_ static void destruct_singleton() noexcept
	{
		singleton<T>::tl_s_instance.destruct();
	}
};

template<typename T>
thread_local ::FE::memory_block<T> FE::singleton<T>::tl_s_instance;

END_NAMESPACE
#endif