#ifndef _FE_CORE_SHARED_LOCK_GUARD_HPP_
#define _FE_CORE_SHARED_LOCK_GUARD_HPP_
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




BEGIN_NAMESPACE(FE)

template<class SharedMutex>
class shared_lock_guard final
{
private:
	SharedMutex& m_shared_mutex;

public:
	typedef SharedMutex shared_mutex_type;

	shared_lock_guard(SharedMutex& shared_mutex_p) noexcept : m_shared_mutex(shared_mutex_p)
	{
		m_shared_mutex.lock_shared();
	}

	// Adopts a SharedMutex that the calling thread already owns in the shared mode.
	shared_lock_guard(SharedMutex& shared_mutex_p, std::adopt_lock_t) noexcept : m_shared_mutex(shared_mutex_p) {}

	~shared_lock_guard() noexcept
	{
		m_shared_mutex.unlock_shared();
	}

	_FE_FORCE_INLINE_ SharedMutex& get_shared_mutex() const noexcept { return m_shared_mutex; }

	shared_lock_guard(const shared_lock_guard& other_p) noexcept = delete;
	shared_lock_guard(shared_lock_guard&& rvalue_p) noexcept = delete;
	shared_lock_guard& operator=(const shared_lock_guard& other_p) noexcept = delete;
	shared_lock_guard& operator=(shared_lock_guard&& rvalue_p) noexcept = delete;
};

END_NAMESPACE
#endif