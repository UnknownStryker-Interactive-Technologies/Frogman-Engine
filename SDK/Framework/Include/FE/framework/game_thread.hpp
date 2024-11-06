#ifndef _FE_FRAMEWORK_GAME_THREAD_HPP_
#define _FE_FRAMEWORK_GAME_THREAD_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
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