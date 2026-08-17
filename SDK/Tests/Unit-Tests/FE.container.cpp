#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
/*
Google Test suite for FE::fqueue.

Covers every public method:
  - fqueue(), ~fqueue()
  - fqueue(std::initializer_list&&), fqueue(InputIterator, InputIterator)
  - fqueue(const fqueue&), fqueue(fqueue&&)
  - operator=(std::initializer_list&&), operator=(const fqueue&), operator=(fqueue&&)
  - push, pop, pop_all
  - front (const/non-const), back (const/non-const)
  - is_empty, count, size, max_size, capacity
  - swap
  - xtl::fqueue alias

Notes:
  - Adjust the #include path below to match the project layout.
  - Assertion-violating calls (push on a full queue, pop on an empty queue,
	empty initializer_list, front/back on an empty queue) are intentionally
	not exercised: FE_ASSERT / FE_NEGATIVE_ASSERT behavior depends on the
	build configuration, so death tests would not be portable.
  - test_input_iterator below is a self-contained class-type iterator that
	satisfies the static_asserts of the range constructor.
*/
#include <FE/farray.hxx>
#include <FE/fqueue.hxx>
#include <FE/fstack.hxx>

#include<FE/pool/memory_resource.hxx>

#include <FE/concurrent_vector.hxx>

// ms ppl
#include <concurrent_vector.h>

// taskflow for the concurrent push_back performance tests
#include <taskflow.hpp>

// std
#include <queue>
#include <stack>
#include <string>
#include <shared_mutex>
#include <unordered_map>
#include <vector>
#include <cstddef>
#include <iterator>
#include <memory>
#include <random>
#include <utility>

namespace
{
	// Counts live instances to verify that constructions and destructions balance.
	class object_counter
	{
	public:
		static inline long long s_alive = 0;
		static inline long long s_destructed = 0;

		object_counter() noexcept : m_value(0) { ++s_alive; }
		object_counter(int value_p) noexcept : m_value(value_p) { ++s_alive; }
		object_counter(const object_counter& other_p) noexcept : m_value(other_p.m_value) { ++s_alive; }
		object_counter(object_counter&& rvalue_p) noexcept : m_value(rvalue_p.m_value) { rvalue_p.m_value = -1; ++s_alive; }
		object_counter& operator=(const object_counter& other_p) noexcept = default;
		object_counter& operator=(object_counter&& rvalue_p) noexcept { m_value = rvalue_p.m_value; rvalue_p.m_value = -1; return *this; }
		~object_counter() { --s_alive; ++s_destructed; }

		int value() const noexcept { return m_value; }
		static void reset() noexcept { s_alive = 0; s_destructed = 0; }

	private:
		int m_value;
	};

	// Minimal class-type random access iterator over a raw array.
	// The range constructor requires a class type with a matching value_type,
	// operator-, relational operators, and construction from a raw pointer.
	template<typename T>
	class test_input_iterator
	{
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

		test_input_iterator() noexcept : m_ptr(nullptr) {}
		test_input_iterator(T* ptr_p) noexcept : m_ptr(ptr_p) {}

		reference operator*() const noexcept { return *m_ptr; }
		pointer operator->() const noexcept { return m_ptr; }
		reference operator[](difference_type index_p) const noexcept { return m_ptr[index_p]; }

		test_input_iterator& operator++() noexcept { ++m_ptr; return *this; }
		test_input_iterator operator++(int) noexcept { test_input_iterator l_tmp = *this; ++m_ptr; return l_tmp; }
		test_input_iterator& operator--() noexcept { --m_ptr; return *this; }
		test_input_iterator operator--(int) noexcept { test_input_iterator l_tmp = *this; --m_ptr; return l_tmp; }
		test_input_iterator& operator+=(difference_type offset_p) noexcept { m_ptr += offset_p; return *this; }
		test_input_iterator& operator-=(difference_type offset_p) noexcept { m_ptr -= offset_p; return *this; }
		test_input_iterator operator+(difference_type offset_p) const noexcept { return test_input_iterator(m_ptr + offset_p); }
		test_input_iterator operator-(difference_type offset_p) const noexcept { return test_input_iterator(m_ptr - offset_p); }
		difference_type operator-(const test_input_iterator& other_p) const noexcept { return m_ptr - other_p.m_ptr; }

		bool operator==(const test_input_iterator& other_p) const noexcept { return m_ptr == other_p.m_ptr; }
		bool operator!=(const test_input_iterator& other_p) const noexcept { return m_ptr != other_p.m_ptr; }
		bool operator<(const test_input_iterator& other_p) const noexcept { return m_ptr < other_p.m_ptr; }
		bool operator<=(const test_input_iterator& other_p) const noexcept { return m_ptr <= other_p.m_ptr; }
		bool operator>(const test_input_iterator& other_p) const noexcept { return m_ptr > other_p.m_ptr; }
		bool operator>=(const test_input_iterator& other_p) const noexcept { return m_ptr >= other_p.m_ptr; }

	private:
		T* m_ptr;
	};

	// Builds a wrapped queue whose logical contents are {3, 4, 5, 6}:
	// the physical layout is [5, 6, 3, 4] with front in the middle of the buffer.
	FE::fqueue<int, 4> make_wrapped_queue()
	{
		FE::fqueue<int, 4> l_queue{ 1, 2, 3, 4 };
		l_queue.pop();
		l_queue.pop();
		l_queue.push(5);
		l_queue.push(6);
		return l_queue;
	}
}


// ---------------------------------------------------------------- fqueue()
TEST(FQueueDefaultConstructor, ConstructsEmptyQueue)
{
	FE::fqueue<int, 4> l_queue;

	EXPECT_TRUE(l_queue.is_empty());
	EXPECT_EQ(l_queue.size(), 0u);
	EXPECT_EQ(l_queue.count(), 0u);
	EXPECT_EQ(l_queue.capacity(), 4u);
}

TEST(FQueueDefaultConstructor, IsImmediatelyUsable)
{
	FE::fqueue<int, 4> l_queue;
	l_queue.push(42);

	EXPECT_FALSE(l_queue.is_empty());
	EXPECT_EQ(l_queue.front(), 42);
	EXPECT_EQ(l_queue.back(), 42);
	EXPECT_EQ(l_queue.size(), 1u);
}


// --------------------------------------------------------------- ~fqueue()
TEST(FQueueDestructor, DestructsRemainingElements)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_queue;
		l_queue.push(1);
		l_queue.push(2);
		l_queue.push(3);
		EXPECT_EQ(object_counter::s_alive, 3);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}

TEST(FQueueDestructor, DestructsFullQueue)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_queue;
		for (int i = 0; i < 4; ++i)
		{
			l_queue.push(i);
		}
		EXPECT_EQ(object_counter::s_alive, 4);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}

TEST(FQueueDestructor, DestructsWrappedQueue)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_queue;
		for (int i = 1; i <= 4; ++i)
		{
			l_queue.push(i);
		}
		l_queue.pop();
		l_queue.pop();
		l_queue.push(5);
		l_queue.push(6); // physical layout wraps around the buffer end
		EXPECT_EQ(object_counter::s_alive, 4);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// --------------------------------------- fqueue(std::initializer_list&&)
TEST(FQueueInitializerListConstructor, PreservesFIFOOrder)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 3);
	EXPECT_TRUE(l_queue.is_empty());
}

TEST(FQueueInitializerListConstructor, SetsFrontAndBack)
{
	FE::fqueue<int, 8> l_queue{ 10, 20, 30 };

	EXPECT_EQ(l_queue.front(), 10);
	EXPECT_EQ(l_queue.back(), 30);
}

TEST(FQueueInitializerListConstructor, SupportsFullCapacityList)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3, 4 };

	EXPECT_EQ(l_queue.size(), 4u);
	EXPECT_EQ(l_queue.front(), 1);
	EXPECT_EQ(l_queue.back(), 4);
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_EQ(l_queue.pop(), i);
	}
}

TEST(FQueueInitializerListConstructor, PushAfterConstructionAppendsBehindLastElement)
{
	FE::fqueue<int, 4> l_queue{ 1, 2 };
	l_queue.push(3);

	EXPECT_EQ(l_queue.back(), 3);
	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 3);
}

TEST(FQueueInitializerListConstructor, WorksWithNonTrivialType)
{
	FE::fqueue<std::string, 4> l_queue{ std::string("alpha"), std::string("beta") };

	EXPECT_EQ(l_queue.size(), 2u);
	EXPECT_EQ(l_queue.front(), "alpha");
	EXPECT_EQ(l_queue.back(), "beta");
}


// ------------------------------------ fqueue(InputIterator, InputIterator)
TEST(FQueueIteratorConstructor, CopiesRangeInOrder)
{
	int l_source[3] = { 10, 20, 30 };
	FE::fqueue<int, 4> l_queue(test_input_iterator<int>{ l_source }, test_input_iterator<int>{ l_source + 3 });

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.front(), 10);
	EXPECT_EQ(l_queue.back(), 30);
	EXPECT_EQ(l_queue.pop(), 10);
	EXPECT_EQ(l_queue.pop(), 20);
	EXPECT_EQ(l_queue.pop(), 30);
}

TEST(FQueueIteratorConstructor, SupportsFullCapacityRange)
{
	int l_source[4] = { 1, 2, 3, 4 };
	FE::fqueue<int, 4> l_queue(test_input_iterator<int>{ l_source }, test_input_iterator<int>{ l_source + 4 });

	EXPECT_EQ(l_queue.size(), 4u);
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_EQ(l_queue.pop(), i);
	}
}

TEST(FQueueIteratorConstructor, DoesNotModifySourceRange)
{
	std::string l_source[2] = { "left", "right" };
	FE::fqueue<std::string, 4> l_queue(test_input_iterator<std::string>{ l_source }, test_input_iterator<std::string>{ l_source + 2 });

	EXPECT_EQ(l_source[0], "left");
	EXPECT_EQ(l_source[1], "right");
	EXPECT_EQ(l_queue.pop(), "left");
	EXPECT_EQ(l_queue.pop(), "right");
}


// -------------------------------------------------- fqueue(const fqueue&)
TEST(FQueueCopyConstructor, CopiesLinearQueue)
{
	FE::fqueue<int, 4> l_original{ 1, 2, 3 };
	FE::fqueue<int, 4> l_copy = l_original;

	EXPECT_EQ(l_copy.size(), 3u);
	EXPECT_EQ(l_copy.pop(), 1);
	EXPECT_EQ(l_copy.pop(), 2);
	EXPECT_EQ(l_copy.pop(), 3);

	// The source must be left untouched.
	EXPECT_EQ(l_original.size(), 3u);
	EXPECT_EQ(l_original.pop(), 1);
	EXPECT_EQ(l_original.pop(), 2);
	EXPECT_EQ(l_original.pop(), 3);
}

TEST(FQueueCopyConstructor, CopiesEmptyQueue)
{
	FE::fqueue<int, 4> l_original;
	FE::fqueue<int, 4> l_copy = l_original;

	EXPECT_TRUE(l_copy.is_empty());

	l_copy.push(7);
	EXPECT_EQ(l_copy.front(), 7);
	EXPECT_EQ(l_copy.back(), 7);
}

TEST(FQueueCopyConstructor, CopiesWrappedQueue)
{
	FE::fqueue<int, 4> l_original = make_wrapped_queue(); // logical contents: 3, 4, 5, 6
	FE::fqueue<int, 4> l_copy = l_original;

	EXPECT_EQ(l_copy.size(), 4u);
	EXPECT_EQ(l_copy.front(), 3);
	EXPECT_EQ(l_copy.back(), 6);
	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_copy.pop(), i);
	}

	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_original.pop(), i);
	}
}

TEST(FQueueCopyConstructor, CopiesFullQueue)
{
	FE::fqueue<int, 4> l_original{ 1, 2, 3, 4 };
	FE::fqueue<int, 4> l_copy = l_original;

	EXPECT_EQ(l_copy.size(), 4u);
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_EQ(l_copy.pop(), i);
	}
}

TEST(FQueueCopyConstructor, BalancesObjectLifetimes)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_original;
		l_original.push(1);
		l_original.push(2);
		{
			FE::fqueue<object_counter, 4> l_copy = l_original;
			EXPECT_EQ(object_counter::s_alive, 4);
		}
		EXPECT_EQ(object_counter::s_alive, 2);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// ------------------------------------------------------- fqueue(fqueue&&)
TEST(FQueueMoveConstructor, MovesLinearQueue)
{
	FE::fqueue<std::string, 4> l_source{ std::string("a"), std::string("b"), std::string("c") };
	FE::fqueue<std::string, 4> l_destination = std::move(l_source);

	EXPECT_EQ(l_destination.size(), 3u);
	EXPECT_EQ(l_destination.pop(), "a");
	EXPECT_EQ(l_destination.pop(), "b");
	EXPECT_EQ(l_destination.pop(), "c");

	EXPECT_TRUE(l_source.is_empty());
	EXPECT_EQ(l_source.size(), 0u);
}

TEST(FQueueMoveConstructor, MovesWrappedQueue)
{
	FE::fqueue<int, 4> l_source = make_wrapped_queue(); // logical contents: 3, 4, 5, 6
	FE::fqueue<int, 4> l_destination = std::move(l_source);

	EXPECT_EQ(l_destination.size(), 4u);
	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_destination.pop(), i);
	}
	EXPECT_TRUE(l_source.is_empty());
}

TEST(FQueueMoveConstructor, MovedFromQueueIsReusable)
{
	FE::fqueue<int, 4> l_source{ 1, 2, 3 };
	FE::fqueue<int, 4> l_destination = std::move(l_source);

	l_source.push(9);
	EXPECT_EQ(l_source.size(), 1u);
	EXPECT_EQ(l_source.front(), 9);
	EXPECT_EQ(l_source.back(), 9);
	EXPECT_EQ(l_source.pop(), 9);
}

TEST(FQueueMoveConstructor, MoveOfEmptyQueueYieldsEmptyQueue)
{
	FE::fqueue<int, 4> l_source;
	FE::fqueue<int, 4> l_destination = std::move(l_source);

	EXPECT_TRUE(l_destination.is_empty());
	l_destination.push(1);
	EXPECT_EQ(l_destination.front(), 1);
}

TEST(FQueueMoveConstructor, BalancesObjectLifetimes)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_source;
		l_source.push(1);
		l_source.push(2);
		l_source.push(3);

		FE::fqueue<object_counter, 4> l_destination = std::move(l_source);
		// The moved-from shells must be destructed by the move; only the
		// destination's three elements may remain alive.
		EXPECT_EQ(object_counter::s_alive, 3);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// --------------------------------- operator=(std::initializer_list&&)
TEST(FQueueInitializerListAssignment, ReplacesExistingContents)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	l_queue = { 7, 8 };

	EXPECT_EQ(l_queue.size(), 2u);
	EXPECT_EQ(l_queue.pop(), 7);
	EXPECT_EQ(l_queue.pop(), 8);
}

TEST(FQueueInitializerListAssignment, ReleasesPreviousElements)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_queue;
		l_queue.push(1);
		l_queue.push(2);
		l_queue.push(3);

		l_queue = { object_counter(7), object_counter(8) };

		EXPECT_EQ(object_counter::s_alive, 2);
		EXPECT_EQ(l_queue.pop().value(), 7);
		EXPECT_EQ(l_queue.pop().value(), 8);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// ------------------------------------------- operator=(const fqueue&)
TEST(FQueueCopyAssignment, CopiesContents)
{
	FE::fqueue<int, 4> l_source{ 1, 2, 3 };
	FE::fqueue<int, 4> l_destination{ 9 };

	l_destination = l_source;

	EXPECT_EQ(l_destination.size(), 3u);
	EXPECT_EQ(l_destination.pop(), 1);
	EXPECT_EQ(l_destination.pop(), 2);
	EXPECT_EQ(l_destination.pop(), 3);

	EXPECT_EQ(l_source.size(), 3u);
	EXPECT_EQ(l_source.front(), 1);
}

TEST(FQueueCopyAssignment, AssigningEmptyQueueClearsDestination)
{
	FE::fqueue<int, 4> l_source;
	FE::fqueue<int, 4> l_destination{ 1, 2 };

	l_destination = l_source;

	EXPECT_TRUE(l_destination.is_empty());
	EXPECT_EQ(l_destination.size(), 0u);

	l_destination.push(5);
	EXPECT_EQ(l_destination.front(), 5);
}

TEST(FQueueCopyAssignment, SelfAssignmentIsNoOp)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	FE::fqueue<int, 4>& l_alias = l_queue;

	l_queue = l_alias;

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 3);
}

TEST(FQueueCopyAssignment, CopiesWrappedQueueInLogicalOrder)
{
	FE::fqueue<int, 4> l_source = make_wrapped_queue(); // logical contents: 3, 4, 5, 6
	FE::fqueue<int, 4> l_destination;

	l_destination = l_source;

	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_destination.pop(), i);
	}
}

TEST(FQueueCopyAssignment, ReleasesPreviousElements)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_source;
		l_source.push(1);
		l_source.push(2);

		FE::fqueue<object_counter, 4> l_destination;
		l_destination.push(3);
		l_destination.push(4);
		l_destination.push(5);

		l_destination = l_source;

		EXPECT_EQ(object_counter::s_alive, 4); // 2 in the source + 2 copies
		EXPECT_EQ(l_destination.pop().value(), 1);
		EXPECT_EQ(l_destination.pop().value(), 2);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// ------------------------------------------------ operator=(fqueue&&)
TEST(FQueueMoveAssignment, TransfersContents)
{
	FE::fqueue<std::string, 4> l_source{ std::string("x"), std::string("y") };
	FE::fqueue<std::string, 4> l_destination{ std::string("old") };

	l_destination = std::move(l_source);

	EXPECT_EQ(l_destination.size(), 2u);
	EXPECT_EQ(l_destination.pop(), "x");
	EXPECT_EQ(l_destination.pop(), "y");

	EXPECT_TRUE(l_source.is_empty());
	l_source.push(std::string("reused"));
	EXPECT_EQ(l_source.front(), "reused");
}

TEST(FQueueMoveAssignment, MoveAssigningEmptyQueueClearsDestination)
{
	FE::fqueue<int, 4> l_source;
	FE::fqueue<int, 4> l_destination{ 1, 2 };

	l_destination = std::move(l_source);

	EXPECT_TRUE(l_destination.is_empty());

	l_destination.push(6);
	EXPECT_EQ(l_destination.front(), 6);
}

TEST(FQueueMoveAssignment, SelfMoveAssignmentIsNoOp)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	FE::fqueue<int, 4>& l_alias = l_queue;

	l_queue = std::move(l_alias);

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 3);
}

TEST(FQueueMoveAssignment, ReleasesPreviousElements)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_source;
		l_source.push(1);

		FE::fqueue<object_counter, 4> l_destination;
		l_destination.push(2);
		l_destination.push(3);

		l_destination = std::move(l_source);

		EXPECT_EQ(object_counter::s_alive, 1);
		EXPECT_EQ(l_destination.pop().value(), 1);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// ------------------------------------------------------------------ push
TEST(FQueuePush, AppendsInFIFOOrder)
{
	FE::fqueue<int, 4> l_queue;
	l_queue.push(1);
	l_queue.push(2);
	l_queue.push(3);

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.front(), 1);
	EXPECT_EQ(l_queue.back(), 3);
}

TEST(FQueuePush, ForwardsConstructorArguments)
{
	FE::fqueue<std::string, 4> l_queue;
	l_queue.emplace(3, 'x');          // std::string(3, 'x')
	l_queue.push("literal");       // std::string(const char*)

	EXPECT_EQ(l_queue.front(), "xxx");
	EXPECT_EQ(l_queue.back(), "literal");
}

TEST(FQueuePush, FillsToFullCapacity)
{
	FE::fqueue<int, 4> l_queue;
	for (int i = 1; i <= 4; ++i)
	{
		l_queue.push(i * 10);
	}

	EXPECT_EQ(l_queue.size(), 4u);
	EXPECT_EQ(l_queue.size(), l_queue.capacity());
	EXPECT_EQ(l_queue.front(), 10);
	EXPECT_EQ(l_queue.back(), 40);
}

TEST(FQueuePush, MaintainsOrderAcrossWrapAround)
{
	FE::fqueue<int, 4> l_queue;
	for (int i = 1; i <= 4; ++i)
	{
		l_queue.push(i);
	}
	l_queue.pop();
	l_queue.pop();
	l_queue.push(5);
	l_queue.push(6); // this push writes past the physical end of the buffer

	EXPECT_EQ(l_queue.size(), 4u);
	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_queue.pop(), i);
	}
}

TEST(FQueuePush, SupportsMoveOnlyTypes)
{
	FE::fqueue<std::unique_ptr<int>, 4> l_queue;
	l_queue.emplace(std::make_unique<int>(7));
	l_queue.emplace(std::make_unique<int>(8));

	EXPECT_EQ(*l_queue.front(), 7);
	EXPECT_EQ(*l_queue.back(), 8);

	std::unique_ptr<int> l_popped = l_queue.pop();
	EXPECT_EQ(*l_popped, 7);
	EXPECT_EQ(l_queue.size(), 1u);
}

TEST(FQueuePush, WorksWithCapacityOne)
{
	FE::fqueue<int, 1> l_queue;
	l_queue.push(1);
	EXPECT_EQ(l_queue.front(), 1);
	EXPECT_EQ(l_queue.back(), 1);
	EXPECT_EQ(l_queue.pop(), 1);

	l_queue.push(2); // every push after the first wraps around
	EXPECT_EQ(l_queue.front(), 2);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_TRUE(l_queue.is_empty());
}


// ------------------------------------------------------------------- pop
TEST(FQueuePop, ReturnsElementsInFIFOOrder)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };

	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.size(), 2u);
	EXPECT_EQ(l_queue.front(), 2);

	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 3);
	EXPECT_TRUE(l_queue.is_empty());
}

TEST(FQueuePop, ReturnsByValue)
{
	FE::fqueue<std::string, 2> l_queue;
	l_queue.push(std::string("hello"));

	std::string l_value = l_queue.pop();

	EXPECT_EQ(l_value, "hello");
	EXPECT_TRUE(l_queue.is_empty());
}

TEST(FQueuePop, DestroysPoppedElement)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_queue;
		l_queue.push(1);
		l_queue.push(2);
		{
			object_counter l_popped = l_queue.pop();
			EXPECT_EQ(l_popped.value(), 1);
			EXPECT_EQ(object_counter::s_alive, 2); // l_popped + the remaining element
		}
		EXPECT_EQ(object_counter::s_alive, 1);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}

TEST(FQueuePop, QueueIsReusableAfterBecomingEmptyViaPop)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3, 4 };
	for (int i = 1; i <= 4; ++i)
	{
		l_queue.pop(); // the front pointer wraps back to the buffer start
	}
	EXPECT_TRUE(l_queue.is_empty());

	l_queue.push(5);
	EXPECT_EQ(l_queue.front(), 5);
	EXPECT_EQ(l_queue.back(), 5);
	EXPECT_EQ(l_queue.pop(), 5);
}


// --------------------------------------------------------------- pop_all
TEST(FQueuePopAll, EmptiesTheQueue)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	l_queue.pop_all();

	EXPECT_TRUE(l_queue.is_empty());
	EXPECT_EQ(l_queue.size(), 0u);
	EXPECT_EQ(l_queue.count(), 0u);
}

TEST(FQueuePopAll, DestructsEveryElement)
{
	object_counter::reset();
	FE::fqueue<object_counter, 4> l_queue;
	l_queue.emplace(1);
	l_queue.emplace(2);
	l_queue.emplace(3);

	l_queue.pop_all();

	EXPECT_EQ(object_counter::s_alive, 0);
	EXPECT_EQ(object_counter::s_destructed, 3);
}

TEST(FQueuePopAll, DestructsSingleElementQueue)
{
	object_counter::reset();
	FE::fqueue<object_counter, 4> l_queue;
	l_queue.emplace(1);

	l_queue.pop_all();

	EXPECT_EQ(object_counter::s_alive, 0);
	EXPECT_EQ(object_counter::s_destructed, 1);
}

TEST(FQueuePopAll, DestructsFullQueue)
{
	object_counter::reset();
	FE::fqueue<object_counter, 4> l_queue;
	for (int i = 0; i < 4; ++i)
	{
		l_queue.emplace(i);
	}

	l_queue.pop_all();

	EXPECT_EQ(object_counter::s_alive, 0);
	EXPECT_EQ(object_counter::s_destructed, 4);
}

TEST(FQueuePopAll, DestructsWrappedQueue)
{
	object_counter::reset();
	FE::fqueue<object_counter, 4> l_queue;
	for (int i = 1; i <= 4; ++i)
	{
		l_queue.emplace(i);
	}
	l_queue.pop(); // 1 destructed here (+ the returned temporary)
	l_queue.pop(); // 2 destructed here (+ the returned temporary)
	l_queue.emplace(5);
	l_queue.emplace(6); // physical layout now wraps around the buffer end
	long long l_destructed_before = object_counter::s_destructed;

	l_queue.pop_all();

	EXPECT_EQ(object_counter::s_alive, 0);
	EXPECT_EQ(object_counter::s_destructed - l_destructed_before, 4);
}

TEST(FQueuePopAll, QueueIsReusableAfterPopAll)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	l_queue.pop_all();

	l_queue.push(42);

	EXPECT_EQ(l_queue.size(), 1u);
	EXPECT_EQ(l_queue.front(), 42);
	EXPECT_EQ(l_queue.back(), 42);
	EXPECT_EQ(l_queue.pop(), 42);
}

TEST(FQueuePopAll, IsNoOpOnEmptyQueue)
{
	FE::fqueue<int, 4> l_queue;
	l_queue.pop_all();

	EXPECT_TRUE(l_queue.is_empty());
	l_queue.push(1);
	EXPECT_EQ(l_queue.front(), 1);
}


// ------------------------------------------------------- front (2 overloads)
TEST(FQueueFront, ReturnsOldestElement)
{
	FE::fqueue<int, 4> l_queue;
	l_queue.push(10);
	EXPECT_EQ(l_queue.front(), 10);

	l_queue.push(20);
	EXPECT_EQ(l_queue.front(), 10); // pushing must not change the front

	l_queue.pop();
	EXPECT_EQ(l_queue.front(), 20);
}

TEST(FQueueFront, MutableFrontAllowsModification)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	l_queue.front() = 100;

	EXPECT_EQ(l_queue.pop(), 100);
	EXPECT_EQ(l_queue.pop(), 2);
}

TEST(FQueueFront, ConstOverloadIsAccessible)
{
	const FE::fqueue<int, 4> l_queue{ 1, 2, 3 };

	EXPECT_EQ(l_queue.front(), 1);
}

TEST(FQueueFront, TracksWrapAroundOfFrontPointer)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3, 4 };
	l_queue.pop();
	l_queue.pop();
	l_queue.pop();
	l_queue.push(5);
	l_queue.push(6);

	EXPECT_EQ(l_queue.front(), 4);
	l_queue.pop(); // the front pointer wraps back to the buffer start here
	EXPECT_EQ(l_queue.front(), 5);
}


// -------------------------------------------------------- back (2 overloads)
TEST(FQueueBack, ReturnsNewestElement)
{
	FE::fqueue<int, 4> l_queue;
	l_queue.push(10);
	EXPECT_EQ(l_queue.back(), 10);

	l_queue.push(20);
	EXPECT_EQ(l_queue.back(), 20);

	l_queue.pop();
	EXPECT_EQ(l_queue.back(), 20); // popping must not change the back
}

TEST(FQueueBack, MutableBackAllowsModification)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3 };
	l_queue.back() = 300;

	EXPECT_EQ(l_queue.pop(), 1);
	EXPECT_EQ(l_queue.pop(), 2);
	EXPECT_EQ(l_queue.pop(), 300);
}

TEST(FQueueBack, ConstOverloadIsAccessible)
{
	const FE::fqueue<int, 4> l_queue{ 1, 2, 3 };

	EXPECT_EQ(l_queue.back(), 3);
}

TEST(FQueueBack, TracksWrapAroundOfBackPointer)
{
	FE::fqueue<int, 4> l_queue{ 1, 2, 3, 4 };
	l_queue.pop();
	l_queue.push(5); // the back pointer wraps to the buffer start here

	EXPECT_EQ(l_queue.back(), 5);
	EXPECT_EQ(l_queue.front(), 2);
	EXPECT_EQ(l_queue.size(), 4u);
}


// -------------------------------------------------------------- is_empty
TEST(FQueueIsEmpty, ReflectsElementCount)
{
	FE::fqueue<int, 4> l_queue;
	EXPECT_TRUE(l_queue.is_empty());

	l_queue.push(1);
	EXPECT_FALSE(l_queue.is_empty());

	l_queue.pop();
	EXPECT_TRUE(l_queue.is_empty());

	l_queue.push(2);
	l_queue.push(3);
	l_queue.pop_all();
	EXPECT_TRUE(l_queue.is_empty());
}


// --------------------------------------------------------- count / size
TEST(FQueueCountAndSize, AreAlwaysEqualAndTrackElementCount)
{
	FE::fqueue<int, 4> l_queue;
	EXPECT_EQ(l_queue.count(), l_queue.size());
	EXPECT_EQ(l_queue.size(), 0u);

	l_queue.push(1);
	l_queue.push(2);
	EXPECT_EQ(l_queue.count(), l_queue.size());
	EXPECT_EQ(l_queue.size(), 2u);

	l_queue.pop();
	EXPECT_EQ(l_queue.count(), l_queue.size());
	EXPECT_EQ(l_queue.size(), 1u);

	l_queue.pop_all();
	EXPECT_EQ(l_queue.count(), l_queue.size());
	EXPECT_EQ(l_queue.size(), 0u);
}


// -------------------------------------------------- max_size / capacity
TEST(FQueueCapacityAndMaxSize, ReturnCompileTimeCapacity)
{
	FE::fqueue<int, 1> l_tiny;
	EXPECT_EQ(l_tiny.capacity(), 1u);
	EXPECT_EQ(l_tiny.max_size(), 1u);

	FE::fqueue<int, 7> l_small;
	EXPECT_EQ(l_small.capacity(), 7u);
	EXPECT_EQ(l_small.max_size(), 7u);

	FE::fqueue<std::string, 32> l_medium;
	EXPECT_EQ(l_medium.capacity(), 32u);
	EXPECT_EQ(l_medium.max_size(), 32u);

	// The capacity must not depend on how many elements are stored.
	l_small.push(1);
	l_small.push(2);
	EXPECT_EQ(l_small.capacity(), 7u);
	EXPECT_EQ(l_small.max_size(), 7u);
}


// ------------------------------------------------------------------ swap
TEST(FQueueSwap, ExchangesContents)
{
	FE::fqueue<int, 4> l_first{ 1, 2 };
	FE::fqueue<int, 4> l_second{ 3, 4, 5 };

	l_first.swap(l_second);

	EXPECT_EQ(l_first.size(), 3u);
	EXPECT_EQ(l_first.pop(), 3);
	EXPECT_EQ(l_first.pop(), 4);
	EXPECT_EQ(l_first.pop(), 5);

	EXPECT_EQ(l_second.size(), 2u);
	EXPECT_EQ(l_second.pop(), 1);
	EXPECT_EQ(l_second.pop(), 2);
}

TEST(FQueueSwap, SwapsWithEmptyQueue)
{
	FE::fqueue<int, 4> l_first{ 1, 2 };
	FE::fqueue<int, 4> l_second;

	l_first.swap(l_second);

	EXPECT_TRUE(l_first.is_empty());
	EXPECT_EQ(l_second.size(), 2u);
	EXPECT_EQ(l_second.pop(), 1);
	EXPECT_EQ(l_second.pop(), 2);
}

TEST(FQueueSwap, HandlesWrappedQueues)
{
	FE::fqueue<int, 4> l_first = make_wrapped_queue(); // logical contents: 3, 4, 5, 6
	FE::fqueue<int, 4> l_second{ 9 };

	l_first.swap(l_second);

	EXPECT_EQ(l_first.size(), 1u);
	EXPECT_EQ(l_first.pop(), 9);

	EXPECT_EQ(l_second.size(), 4u);
	for (int i = 3; i <= 6; ++i)
	{
		EXPECT_EQ(l_second.pop(), i);
	}
}

TEST(FQueueSwap, BalancesObjectLifetimes)
{
	object_counter::reset();
	{
		FE::fqueue<object_counter, 4> l_first;
		l_first.push(1);

		FE::fqueue<object_counter, 4> l_second;
		l_second.push(2);
		l_second.push(3);

		l_first.swap(l_second);

		EXPECT_EQ(object_counter::s_alive, 3);
		EXPECT_EQ(l_first.size(), 2u);
		EXPECT_EQ(l_first.front().value(), 2);
		EXPECT_EQ(l_second.size(), 1u);
		EXPECT_EQ(l_second.front().value(), 1);
	}
	EXPECT_EQ(object_counter::s_alive, 0);
}


// -------------------------------------------------------- xtl::fqueue
TEST(XtlFQueueAlias, ProvidesDefaultTraitsArgument)
{
	xtl::fqueue<int, 4> l_queue;
	l_queue.push(1);
	l_queue.push(2);

	EXPECT_EQ(l_queue.front(), 1);
	EXPECT_EQ(l_queue.back(), 2);
	EXPECT_EQ(l_queue.pop(), 1);
}


// ------------------------------------------- cross-method regression test
// Drives fqueue with a long pseudo-random operation sequence and checks it
// against std::queue after every step. This exercises wrap-around of both
// pointers, the full and the empty state, and pop_all in the middle of use.
TEST(FQueueStress, MatchesStdQueueReferenceModel)
{
	constexpr FE::size l_capacity = 8;
	FE::fqueue<int, l_capacity> l_queue;
	std::queue<int> l_reference;

	std::mt19937 l_generator(20260818u);
	std::uniform_int_distribution<int> l_operation(0, 99);
	std::uniform_int_distribution<int> l_value(-1000, 1000);

	for (int i = 0; i < 20000; ++i)
	{
		if (i % 997 == 0)
		{
			l_queue.pop_all();
			while (l_reference.empty() == false)
			{
				l_reference.pop();
			}
		}

		bool l_do_push = l_operation(l_generator) < 55;
		if (l_reference.size() >= l_capacity)
		{
			l_do_push = false;
		}
		if (l_reference.empty())
		{
			l_do_push = true;
		}

		if (l_do_push)
		{
			const int l_new_value = l_value(l_generator);
			l_queue.push(l_new_value);
			l_reference.push(l_new_value);
		}
		else
		{
			ASSERT_EQ(l_queue.pop(), l_reference.front());
			l_reference.pop();
		}

		ASSERT_EQ(l_queue.size(), l_reference.size());
		ASSERT_EQ(l_queue.is_empty(), l_reference.empty());
		if (l_reference.empty() == false)
		{
			ASSERT_EQ(l_queue.front(), l_reference.front());
			ASSERT_EQ(l_queue.back(), l_reference.back());
		}
	}
}


TEST(queue, push_pop)
{
	FE::fqueue<std::string, 3> l_languages;
	l_languages.push("Java");
	l_languages.push("Python");
	l_languages.push("C#");

	EXPECT_TRUE(l_languages.pop() == "Java");
	EXPECT_TRUE(l_languages.pop() == "Python");
	l_languages.push("C++");

	EXPECT_TRUE(l_languages.front() == "C#");
	EXPECT_TRUE(l_languages.back() == "C++");

	l_languages.pop_all();
	EXPECT_TRUE(l_languages.is_empty());
}




void FE_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fqueue<const char*, 64> l_queue;

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(FE_queue_push_and_pop);


void std_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	std::queue<const char*> l_queue;

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(std_queue_push_and_pop);













TEST(fstack, trivially_constructible_and_destructible)
{
	FE::fstack<int, 10> l_another_stack = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	FE::fstack<int, 10> l_stack(l_another_stack.cbegin(), l_another_stack.cend());
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	{
		FE::fstack<int, 10> l_copied_stack = l_stack;
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();
		
		while(l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<int, 10> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, 0);
		}

		l_copied_stack = { 5, 4, 3, 2, 1 };
		l_copied_stack.swap(l_stack);
	}
}

TEST(fstack, constructible_and_destructible)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "."};
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rbegin = l_stack.crbegin();
	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rend = l_stack.crend();

	{
		FE::fstack<std::string, 5> l_copied_stack = l_stack;
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();

		while (l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<std::string, 5> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, "\0");
		}

		l_copied_stack = { "hello", ",", " ", "C++", "."};
		l_copied_stack.swap(l_stack);
	}
}

TEST(fstack, multiple_assignments_)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "." };
	l_stack = { "hello", ",", " ", "C++", "." };
	EXPECT_EQ(strcmp(l_stack.cbegin()->c_str(), "hello"), 0);
	EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
	{
		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}


	FE::fstack<std::string, 5> l_another_stack = { "hi", ",", " ", "world", "." };
	l_stack = std::move(l_another_stack);
	{
		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}
	l_stack = std::move(l_another_stack);
}

TEST(fstack, container_comparison_)
{
	FE::fstack<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
    EXPECT_EQ(strcmp(l_languages1.top().c_str(), "C"), 0);
	FE::fstack<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
	EXPECT_TRUE(l_languages1 == l_languages2);

    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Java"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Python"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C#"), 0);
    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C

	l_languages1.push("Python");
	l_languages2.pop();
	EXPECT_TRUE(l_languages1 != l_languages2); // Python is not equivalent to Java
}




void FE_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fstack<const char*, 64> l_stack;

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(FE_stack_push_and_pop);


void std_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	std::stack<const char*, std::vector<const char*>> l_stack;

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(std_stack_push_and_pop);




TEST(farray, all)
{
	{
		FE::farray<const char*, 64> l_array;
		l_array.push_back("Pizza");
		
		EXPECT_EQ(strcmp(l_array.back(), "Pizza"), 0);
		l_array.pop_back();
	}
}




namespace
{
	struct TestObject
	{
		int _var1;
		double _var2;
		std::string _var3;

		TestObject(const int a, const double b, const std::string& c)
			: _var1{ a }, _var2{ b }, _var3{ c }
		{
		}
	};
}


static void fe_concurrent_vector_push_back(benchmark::State& state)
{
	FE::concurrent_vector<TestObject> l_vector;
	for (auto _ : state)
	{
		l_vector.push_back(TestObject{ 42, 3.14, "Test" });
	}
}
BENCHMARK(fe_concurrent_vector_push_back)->Iterations(50000);


static void std_vector_push_back(benchmark::State& state)
{
	std::vector<TestObject> l_vector;
	for (auto _ : state)
	{
		l_vector.push_back(TestObject{ 42, 3.14, "Test" });
	}
}
BENCHMARK(std_vector_push_back)->Iterations(50000);


static void ppl_concurrent_vector_push_back(benchmark::State& state)
{
	concurrency::concurrent_vector<TestObject> l_vector;
	for (auto _ : state)
	{
		l_vector.push_back(TestObject{ 42, 3.14, "Test" });
	}
}
BENCHMARK(ppl_concurrent_vector_push_back)->Iterations(50000);




static constexpr FE::uint32 thread_count = 64;
void fe_concurrent_vector_concurrent_push_back(benchmark::State& state)
{
	tf::Taskflow l_tasks;
	FE::concurrent_vector<TestObject> l_vector;

	for (var::uint32 i = 0; i < thread_count; ++i)
	{
		l_tasks.emplace(
			[&]()
			{
				l_vector.push_back(TestObject{ 42, 3.14, "Test" });
			}
		);
	}

	tf::Executor l_executor(thread_count);
	for (auto _ : state)
	{
		l_executor.run(l_tasks);
	}
}
BENCHMARK(fe_concurrent_vector_concurrent_push_back)->Iterations(10);


static void std_vector_concurrent_push_back(benchmark::State& state)
{
	tf::Taskflow l_tasks;
	std::mutex l_lock;
	std::vector<TestObject> l_vector;
	for (var::uint32 i = 0; i < thread_count; ++i)
	{
		l_tasks.emplace(
			[&]()
			{
				std::lock_guard<std::mutex> l_mut(l_lock);
				l_vector.push_back(TestObject{ 42, 3.14, "Test" });
			}
		);
	}

	tf::Executor l_executor(thread_count);
	for (auto _ : state)
	{
		l_executor.run(l_tasks);
	}
}
BENCHMARK(std_vector_concurrent_push_back)->Iterations(10);


static void ppl_concurrent_vector_concurrent_push_back(benchmark::State& state)
{
	tf::Taskflow l_tasks;
	concurrency::concurrent_vector<TestObject> l_vector;

	for (var::uint32 i = 0; i < thread_count; ++i)
	{
		l_tasks.emplace(
			[&]()
			{
				l_vector.push_back(TestObject{ 42, 3.14, "Test" });
			}
		);
	}

	tf::Executor l_executor(thread_count);
	for (auto _ : state)
	{
		l_executor.run(l_tasks);
	}
}
BENCHMARK(ppl_concurrent_vector_concurrent_push_back)->Iterations(10);




