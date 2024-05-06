#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <FE/core/containers/array.hxx>
#include <FE/core/string.hxx>




TEST(farray, all)
{
	{
		FE::farray<const char*, 64> l_array;
		l_array.push_back("Pizza");
		
		EXPECT_TRUE(FE::algorithm::string::compare(l_array.back(), "Pizza"));
		l_array.pop_back();
	}
}


TEST(array, constructor_and_destructor)
{
	{
		FE::array<FE::string> l_array;
	}

	{
		FE::array<FE::string> l_array(2, "A bottle of water");
		EXPECT_TRUE(l_array[0] == "A bottle of water");
		EXPECT_TRUE(l_array[1] == "A bottle of water");
	}

	{
		std::initializer_list<FE::string> l_initializer_list = {"A", "bottle", "of", "water"};
		FE::array<FE::string> l_array(l_initializer_list.begin(), l_initializer_list.end());
		
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");

	}

	{
		FE::array<FE::string> l_array(2, "A bottle of water");
		FE::array<FE::string> l_another_array = l_array;
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<FE::string> l_array(2, "A bottle of water");
		FE::array<FE::string> l_another_array = std::move(l_array);
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<FE::string> l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}

	{
		FE::array<FE::string> l_array(2, "A bottle of water");
		FE::array<FE::string> l_another_array;
		l_another_array = l_array;
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<FE::string> l_array(2, "A bottle of water");
		FE::array<FE::string> l_another_array;
		l_another_array = std::move(l_array);
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<FE::string> l_array;
		l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}
}




TEST(array, Element_access)
{
	{
		FE::array<FE::string> l_array;
		l_array.reserve(2);
		l_array.assign(2, "A bottle of water");

		EXPECT_TRUE(l_array[0] == "A bottle of water");
		EXPECT_TRUE(l_array[1] == "A bottle of water");
	}

	{
		std::initializer_list<FE::string> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<FE::string> l_array;
		l_array.reserve(8);
		l_array.assign(l_initializer_list.begin(), l_initializer_list.end());

		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}

	{
		FE::array<FE::string> l_array;
		l_array.reserve(8);
		l_array.assign({ "A", "bottle", "of", "water" });

		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");

	}

	{
		FE::array<FE::string> l_array = { "A", "bottle", "of", "water" };

		FE::array<FE::string> l_another_array;
		l_another_array.reserve(8);
		l_another_array.assign_range(l_array.make_range(FE::index_t{ 1 }, FE::index_t{ 4 }));


		EXPECT_TRUE(l_another_array.front() == "bottle");
		EXPECT_TRUE(l_another_array[1] == "of");
		EXPECT_TRUE(l_another_array.back() == "water");
		EXPECT_TRUE(l_another_array.data()[0] == "bottle");
	}
}




TEST(array, Iterators)
{
	{
		FE::array<FE::string> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<FE::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto value : l_array)
		{
			EXPECT_TRUE(value == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<FE::string> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<FE::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.cbegin(); it != l_array.cend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<FE::string> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<FE::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.rbegin(); it != l_array.rend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<FE::string> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<FE::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.crbegin(); it != l_array.crend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}
}




TEST(array, Capacity)
{
	{
		FE::array<FE::string> l_array = { "water", "of", "bottle", "A" };
		EXPECT_FALSE(l_array.is_empty());

		EXPECT_EQ(l_array.size(), 4);

		l_array.reserve(10);
		EXPECT_EQ(l_array.capacity(), 10);

		l_array.extend(6);
		EXPECT_EQ(l_array.capacity(), 16);

		l_array.shrink_to_fit();
		EXPECT_EQ(l_array.capacity(), 4);
	}
}




TEST(array, Modifiers)
{
	{
		{
			FE::array<FE::string> l_another_array = { "Christmas" };
			l_another_array.extend(2);
			l_another_array.emplace_back("Tree");
			EXPECT_TRUE(l_another_array[1] == "Tree");

			l_another_array.emplace_back("s");
			EXPECT_TRUE(l_another_array.back() == "s");
		}

	
		{
			FE::array<FE::string> l_array = { "Bye" };
			l_array.resize(16);
			EXPECT_FALSE(l_array.is_empty());

			FE::array<FE::string> l_another_array = { "Hi", ", ", "world", "!"};
			l_array.append_range(l_another_array.make_range(1, l_another_array.size()));
			EXPECT_TRUE(l_array[0] == "Bye");
			EXPECT_TRUE(l_array[1] == ", ");
			EXPECT_TRUE(l_array[2] == "world");
			EXPECT_TRUE(l_array[3] == "!");
			
			EXPECT_TRUE(l_array.back() == "!");
			l_array.pop_back();

			l_array.resize(1);
			EXPECT_EQ(l_array.size(), 1);
			EXPECT_EQ(l_array.capacity(), 1);
		}
	}
}






TEST(array, TrivialConstructorAndDestructor)
{
	using namespace FE;
	{
		FE::array<character*> l_array;
	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "A bottle of water"));
	}

	{
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<character*> l_array(l_initializer_list.begin(), l_initializer_list.end());

		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_array[2], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_array[3], "water"));

	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array = l_array;
		EXPECT_TRUE(algorithm::string::compare(l_another_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array[1], "A bottle of water"));
	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array = std::move(l_array);
		EXPECT_TRUE(algorithm::string::compare(l_another_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array[1], "A bottle of water"));
	}

	{
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_array[2], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_array[3], "water"));
	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array;
		l_another_array = l_array;
		EXPECT_TRUE(algorithm::string::compare(l_another_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array[1], "A bottle of water"));
	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array;
		l_another_array = std::move(l_array);
		EXPECT_TRUE(algorithm::string::compare(l_another_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array[1], "A bottle of water"));
	}

	{
		FE::array<character*> l_array;
		l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_array[2], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_array[3], "water"));
	}
}




TEST(array, TrivialElementAccess)
{
	using namespace FE;
	{
		FE::array<character*> l_array;
		l_array.reserve(2);
		l_array.assign(2, "A bottle of water");

		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A bottle of water"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "A bottle of water"));
	}

	{
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<character*> l_array;
		l_array.reserve(8);
		l_array.assign(l_initializer_list.begin(), l_initializer_list.end());

		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_array[2], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_array[3], "water"));
	}

	{
		FE::array<character*> l_array;
		l_array.reserve(8);
		l_array.assign({ "A", "bottle", "of", "water" });

		EXPECT_TRUE(algorithm::string::compare(l_array[0], "A"));
		EXPECT_TRUE(algorithm::string::compare(l_array[1], "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_array[2], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_array[3], "water"));

	}

	{
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };

		FE::array<character*> l_another_array;
		l_another_array.reserve(8);
		l_another_array.assign_range(l_array.make_range(FE::index_t{ 1 }, FE::index_t{ 4 }));


		EXPECT_TRUE(algorithm::string::compare(l_another_array.front(), "bottle"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array[1], "of"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array.back(), "water"));
		EXPECT_TRUE(algorithm::string::compare(l_another_array.data()[0], "bottle"));
	}
}




TEST(array, TrivialIterators)
{
	using namespace FE;
	{
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto value : l_array)
		{
			EXPECT_TRUE(algorithm::string::compare(value, *l_l_initializer_list_iterator));
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.cbegin(); it != l_array.cend(); ++it)
		{
			EXPECT_TRUE(algorithm::string::compare(*it, *l_l_initializer_list_iterator));
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.rbegin(); it != l_array.rend(); ++it)
		{
			EXPECT_TRUE(algorithm::string::compare(*it, *l_l_initializer_list_iterator));
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.crbegin(); it != l_array.crend(); ++it)
		{
			EXPECT_TRUE(algorithm::string::compare(*it, *l_l_initializer_list_iterator));
			++l_l_initializer_list_iterator;
		}
	}
}




TEST(array, TrivialCapacity)
{
	using namespace FE;
	{
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
		EXPECT_FALSE(l_array.is_empty());

		EXPECT_EQ(l_array.size(), 4);

		l_array.reserve(10);
		EXPECT_EQ(l_array.capacity(), 10);

		l_array.extend(6);
		EXPECT_EQ(l_array.capacity(), 16);

		l_array.shrink_to_fit();
		EXPECT_EQ(l_array.capacity(), 4);
	}
}




TEST(array, TrivialModifiers)
{
	using namespace FE;
	{
		{
			FE::array<character*> l_another_array = { "Christmas", "Tree" };
			l_another_array.extend(1);
			l_another_array.emplace_back("s");
			EXPECT_TRUE(algorithm::string::compare(l_another_array.back(), "s"));
		}


		{
			FE::array<character*> l_array = { "Bye" };
			l_array.resize(16);
			EXPECT_FALSE(l_array.is_empty());

			FE::array<character*> l_another_array = { "Hi", ", ", "world", "!" };
			l_array.append_range(l_another_array.make_range(1, l_another_array.size()));
			EXPECT_TRUE(algorithm::string::compare(l_array[0], "Bye"));
			EXPECT_TRUE(algorithm::string::compare(l_array[1], ", "));
			EXPECT_TRUE(algorithm::string::compare(l_array[2], "world"));
			EXPECT_TRUE(algorithm::string::compare(l_array[3], "!"));

			EXPECT_TRUE(algorithm::string::compare(l_array.back(), "!"));
			l_array.pop_back();

			l_array.resize(1);
			EXPECT_EQ(l_array.size(), 1);
			EXPECT_EQ(l_array.capacity(), 1);
		}
	}
}