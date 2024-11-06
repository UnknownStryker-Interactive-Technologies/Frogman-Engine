#ifndef _FE_FRAMEWORK_GAME_THREAD_HPP_
#define _FE_FRAMEWORK_GAME_THREAD_HPP_
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

// std
#include <thread>




BEGIN_NAMESPACE(FE::framework)

class game_thread
{
	std::thread m_thread;
	var::boolean m_is_interrupted;
	static void __main(game_thread* const this_p);

public:
	game_thread() noexcept = default;
	~game_thread() noexcept = default;

	void run();
	void shutdown();

private:
	game_thread(const game_thread&) = delete;
	game_thread& operator=(const game_thread&) = delete;
};

END_NAMESPACE
#endif