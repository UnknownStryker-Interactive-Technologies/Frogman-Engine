#pragma once
#include <iostream>


class HelloWord
{
public:
	void tickle() const noexcept
	{
		std::cout << "Hello, World!" << std::endl;
	}
};