#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/string_view.hxx>
#include <FE/framework/string.hxx>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

TEST(basic_string_view, constructors_and_assignment)
{
	{
		FE::string l_string = "basic_string_view";
		FE::string_view l_string_view = l_string;
		EXPECT_TRUE(l_string_view == l_string);

		FE::string_view l_another_string_view = l_string_view;
		EXPECT_TRUE(l_another_string_view == l_string_view);
	}

	{
		FE::string l_string = "basic_string_view";
		FE::string_view l_string_view = l_string;
		EXPECT_TRUE(l_string_view == l_string);

		FE::string_view l_another_string_view = std::move(l_string_view);
		EXPECT_TRUE(l_another_string_view == l_string);
	}




	{
		FE::string l_string = "basic_string_view";
		FE::string_view l_string_view;
		l_string_view = l_string;
		EXPECT_TRUE(l_string_view == l_string);

		FE::string_view l_another_string_view;
		l_another_string_view = l_string_view;
		EXPECT_TRUE(l_another_string_view == l_string_view);
	}

	{
		FE::string l_string = "basic_string_view";
		FE::string_view l_string_view;
		l_string_view = l_string;
		EXPECT_TRUE(l_string_view == l_string);
		
		FE::string_view l_another_string_view;
		l_another_string_view = std::move(l_string_view);
		EXPECT_TRUE(l_another_string_view == l_string);
	}
}




TEST(basic_string_view, Iterators)
{
	{
		FE::string l_string = "Cheese Burger";
		FE::string_view l_string_view = l_string;
		FE::string_view::iterator l_iterator = l_string_view.begin();

		for (FE::string::value_type c : l_string)
		{
			EXPECT_EQ(*l_iterator, c);
			++l_iterator;
		}
	}

	{
		FE::string l_string = "Cheese Burger";
		FE::string_view l_string_view = l_string;
		FE::string_view::const_iterator l_iterator = l_string_view.cbegin();

		for (FE::string::value_type c : l_string)
		{
			EXPECT_EQ(*l_iterator, c);
			++l_iterator;
		}
	}


	{
		FE::string l_string = "Cheese Burger";
		FE::algorithm::string::invert(l_string.data());
		FE::string_view l_string_view = l_string;
		FE::string_view::reverse_iterator l_reverse_iterator = l_string_view.rbegin();

		FE::string l_cheese_burger = "Cheese Burger";

		for (FE::string::value_type c : l_cheese_burger)
		{
			EXPECT_EQ(*l_reverse_iterator, c);
			++l_reverse_iterator;
		}
	}

	{
		FE::string l_string = "Cheese Burger";
		FE::algorithm::string::invert(l_string.data());
		FE::string_view l_string_view = l_string;
		FE::string_view::const_reverse_iterator l_const_reverse_iterator = l_string_view.crbegin();

		FE::string l_cheese_burger = "Cheese Burger";

		for (FE::string::value_type c : l_cheese_burger)
		{
			EXPECT_EQ(*l_const_reverse_iterator, c);
			++l_const_reverse_iterator;
		}
	}
}




TEST(basic_string_view, Element_access)
{
	FE::string l_string = "Aglio e olio";
	FE::string_view l_string_view = l_string;
	EXPECT_EQ(l_string_view[1], 'g');
	
	EXPECT_EQ(l_string_view.front(), 'A');
	EXPECT_EQ(l_string_view.back(), 'o');

	EXPECT_TRUE(strcmp(l_string_view.data(), "Aglio e olio") == 0);
}




TEST(basic_string_view, Capacity)
{
	FE::string l_string = "Aglio e olio";
	FE::string_view l_string_view = l_string;

	EXPECT_EQ(l_string_view.length(), strlen("Aglio e olio"));

	EXPECT_EQ(l_string_view.max_length(), FE::max_value<FE::string_view::length_type>);

	EXPECT_FALSE(l_string_view.is_empty());
}




TEST(basic_string_view, Modifiers)
{
	{
		FE::string l_string = "insufficient";
		FE::string_view l_string_view = l_string;
		l_string_view.remove_prefix(2);
		EXPECT_TRUE(l_string_view == "sufficient");
	}

	{
		FE::string l_string = "efficiently";
		FE::string_view l_string_view = l_string;
		l_string_view.remove_suffix(2);
		EXPECT_TRUE(l_string_view == "efficient");
	}
}




TEST(basic_string_view, Operations)
{
	{
		FE::string l_string = "Aglio e olio";
		FE::string_view l_string_view = l_string;

		FE::string l_dest_string;
		l_dest_string.reserve(10);
		FE::string_view l_dest_string_view = l_dest_string;

		l_string_view.copy(l_dest_string, l_dest_string.capacity(), 5, 0);
		l_dest_string_view.reset_range();
		EXPECT_TRUE(l_dest_string_view == "Aglio");
	}


	{
		FE::string l_string = "Aglio e olio";
		FE::string_view l_string_view = l_string;
		FE::string_view l_another_string_view = l_string_view.substr(8, 4);
		EXPECT_TRUE(l_another_string_view == "olio");
	}


	{
		FE::string l_string = "Aglio, olio e peperoncino";
		FE::string_view l_string_view = l_string;

		EXPECT_TRUE(l_string_view.starts_with("Aglio"));
		EXPECT_TRUE(l_string_view.ends_with("peperoncino"));

		EXPECT_TRUE(l_string_view.contains("olio"));

		std::optional<FE::algorithm::string::range> l_position = l_string_view.find(" e ");
		EXPECT_TRUE(FE::algorithm::string::compare_ranged(l_string.c_str(), *l_position, " e ", FE::algorithm::string::range{ 0, 3 }));

		l_string = "peperoncino peperoncino";
		l_string_view.reset_range();
		l_position = l_string_view.rfind("peperoncino");
		EXPECT_TRUE(FE::algorithm::string::compare_ranged(l_string.c_str(), *l_position, "peperoncino", FE::algorithm::string::range{ 0, 11 }));

		FE::algorithm::string::count l_result = l_string_view.count_chars('p', FE::index_t{11});
		EXPECT_EQ(l_result._match_count, 2);
		EXPECT_EQ(l_result._target, 'p');
	}
}