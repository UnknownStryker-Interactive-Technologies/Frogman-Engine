#include <gtest/gtest.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/miscellaneous/google_test_extension.h>
#include <FE/core/singleton.hxx>
#include <string>
#include <vector>


class my_singleton_class
{
	FRIEND_CLASS_SINGLETON(my_singleton_class);

public:
	std::vector<std::string> m_string_table;

private:
	my_singleton_class() noexcept {}

	my_singleton_class(FE::size_t size_p) noexcept
	{
		this->m_string_table.reserve(size_p);
	}
};



TEST(singleton, instantiation)
{
	FE::singleton<my_singleton_class>::singleton_instance(10).m_string_table.emplace_back("FE::singleton<T>");
	FE::singleton<my_singleton_class>::destruct_singleton();
}
