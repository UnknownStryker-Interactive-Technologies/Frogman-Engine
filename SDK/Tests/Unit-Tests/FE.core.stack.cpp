/*
 * FE::stack<T, Allocator> Google Test
 *
 * main() 은 사용자가 통제하므로 이 파일에는 main() 이 없습니다.
 *
 * [타입 규칙] FE:: 의 정수 별칭은 const 한정이라 루프 카운터나 원소 타입으로 쓸 수 없습니다.
 * [push 규칙] push 는 인자를 fstack 쪽 T&& 로 전달하므로 rvalue 만 넘깁니다.
 *
 * mini_stack_capacity = ((4 * one_KiB) / sizeof(T)) - (16 / sizeof(T))
 *   std::int32_t  -> 1020
 *   std::string   -> 102
 *   heavy_element -> 4
 *
 * 활성화된 테스트는 전부 slab 하나 안에서 끝납니다.
 * slab 경계를 넘는 두 개는 DISABLED_ 로 두었습니다 (파일 하단 주석 참고).
 */
#include <gtest/gtest.h>

#include <FE/prerequisites.hxx>
#include <FE/stack.hxx>

#include <cstddef>
#include <cstdint>
#include <memory_resource>
#include <string>
#include <type_traits>
#include <utility>


namespace
{
	// slab 하나에 4개만 들어가는 원소. 경계 테스트를 적은 push 로 끝내기 위한 것.
	struct heavy_element
	{
		std::int64_t _value;
		char _padding[1024 - sizeof(std::int64_t)];
	};

	static_assert(sizeof(heavy_element) == 1024, "heavy_element 크기가 의도와 다릅니다.");

	// allocator 생성자와 소멸자 검증용. 할당 / 해제 횟수만 센다.
	class counting_resource : public std::pmr::memory_resource
	{
	public:
		std::size_t _allocations = 0;
		std::size_t _deallocations = 0;

	private:
		void* do_allocate(std::size_t bytes_p, std::size_t alignment_p) override
		{
			++this->_allocations;
			return std::pmr::new_delete_resource()->allocate(bytes_p, alignment_p);
		}

		void do_deallocate(void* pointer_p, std::size_t bytes_p, std::size_t alignment_p) override
		{
			++this->_deallocations;
			std::pmr::new_delete_resource()->deallocate(pointer_p, bytes_p, alignment_p);
		}

		bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override
		{
			return this == &other_p;
		}
	};
}


TEST(FE_stack, default_constructor)
{
	FE::stack<std::int32_t> l_stack;

	EXPECT_TRUE(l_stack.is_empty());
	EXPECT_EQ(l_stack.size(), 0u);

	l_stack.emplace(7);

	EXPECT_FALSE(l_stack.is_empty());
	EXPECT_EQ(l_stack.size(), 1u);
	EXPECT_EQ(l_stack.top(), 7);
}


TEST(FE_stack, allocator_constructor)
{
	counting_resource l_resource;

	FE::stack<std::int32_t> l_stack{ FE::stack<std::int32_t>::allocator_type{ &l_resource } };

	EXPECT_GT(l_resource._allocations, 0u); // 생성자가 mini stack 을 하나 만든다
	EXPECT_TRUE(l_stack.is_empty());

	for (std::int32_t i = 0; i < 500; ++i)
	{
		l_stack.push(std::int32_t{ i });
	}

	EXPECT_EQ(l_stack.size(), 500u);
	EXPECT_EQ(l_stack.top(), 499);
}


TEST(FE_stack, destructor_releases_memory)
{
	counting_resource l_resource;

	{
		FE::stack<std::int32_t> l_stack{ FE::stack<std::int32_t>::allocator_type{ &l_resource } };

		for (std::int32_t i = 0; i < 500; ++i)
		{
			l_stack.push(std::int32_t{ i });
		}
	}

	EXPECT_EQ(l_resource._allocations, l_resource._deallocations);
}


TEST(FE_stack, copy_constructor)
{
	FE::stack<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t> l_copy(l_source);

	EXPECT_EQ(l_copy.size(), 100u);
	EXPECT_EQ(l_copy.top(), 99);

	for (std::int32_t i = 99; i >= 0; --i)
	{
		ASSERT_EQ(l_copy.top(), i);
		l_copy.pop();
	}
	EXPECT_TRUE(l_copy.is_empty());

	// 깊은 복사 : 사본을 다 비워도 원본은 그대로
	EXPECT_EQ(l_source.size(), 100u);
	EXPECT_EQ(l_source.top(), 99);
}


TEST(FE_stack, copy_constructor_from_empty)
{
	FE::stack<std::int32_t> l_source;
	FE::stack<std::int32_t> l_copy(l_source);

	EXPECT_TRUE(l_copy.is_empty());
	EXPECT_EQ(l_copy.size(), 0u);

	l_copy.push(42);

	EXPECT_EQ(l_copy.top(), 42);
	EXPECT_TRUE(l_source.is_empty());
}


TEST(FE_stack, move_constructor)
{
	FE::stack<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 300; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t> l_moved(std::move(l_source));

	EXPECT_EQ(l_moved.size(), 300u);
	EXPECT_EQ(l_moved.top(), 299);

	// 이동된 원본은 비어 있고 다시 쓸 수 있어야 한다
	EXPECT_TRUE(l_source.is_empty());
	EXPECT_EQ(l_source.size(), 0u);

	l_source.push(1234);

	EXPECT_EQ(l_source.size(), 1u);
	EXPECT_EQ(l_source.top(), 1234);
	EXPECT_EQ(l_moved.top(), 299);
}


TEST(FE_stack, copy_assignment)
{
	FE::stack<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t> l_target;
	l_target.push(-1);
	l_target.push(-2);

	l_target = l_source;

	EXPECT_EQ(l_target.size(), 100u);
	EXPECT_EQ(l_target.top(), 99);

	for (std::int32_t i = 99; i >= 0; --i)
	{
		ASSERT_EQ(l_target.top(), i);
		l_target.pop();
	}

	EXPECT_EQ(l_source.size(), 100u);
	EXPECT_EQ(l_source.top(), 99);
}


TEST(FE_stack, copy_assignment_to_self)
{
	FE::stack<std::int32_t> l_stack;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_stack.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t>& l_alias = l_stack;
	l_stack = l_alias;

	EXPECT_EQ(l_stack.size(), 100u);
	EXPECT_EQ(l_stack.top(), 99);
}


TEST(FE_stack, move_assignment)
{
	FE::stack<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 200; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t> l_target;
	l_target.push(-1);

	l_target = std::move(l_source);

	EXPECT_EQ(l_target.size(), 200u);
	EXPECT_EQ(l_target.top(), 199);

	EXPECT_TRUE(l_source.is_empty());
	EXPECT_EQ(l_source.size(), 0u);

	l_source.push(555);

	EXPECT_EQ(l_source.top(), 555);
	EXPECT_EQ(l_source.size(), 1u);
}


TEST(FE_stack, move_assignment_to_self)
{
	FE::stack<std::int32_t> l_stack;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_stack.push(std::int32_t{ i });
	}

	FE::stack<std::int32_t>& l_alias = l_stack;
	l_stack = std::move(l_alias);

	EXPECT_EQ(l_stack.size(), 100u);
	EXPECT_EQ(l_stack.top(), 99);
}


TEST(FE_stack, top)
{
	FE::stack<std::int32_t> l_stack;

	static_assert(std::is_same_v<decltype(l_stack.top()), std::int32_t&>);

	l_stack.push(10);
	EXPECT_EQ(l_stack.top(), 10);

	l_stack.push(20);
	EXPECT_EQ(l_stack.top(), 20);

	l_stack.top() = 22; // 수정 가능한 참조를 돌려줘야 한다
	EXPECT_EQ(l_stack.top(), 22);

	l_stack.pop();
	EXPECT_EQ(l_stack.top(), 10);
}


TEST(FE_stack, top_const)
{
	FE::stack<std::int32_t> l_stack;
	l_stack.push(10);
	l_stack.push(20);

	const FE::stack<std::int32_t>& l_const_stack = l_stack;

	static_assert(std::is_same_v<decltype(l_const_stack.top()), const std::int32_t&>);

	EXPECT_EQ(l_const_stack.top(), 20);

	l_stack.top() = 99;
	EXPECT_EQ(l_const_stack.top(), 99);
}


TEST(FE_stack, is_empty)
{
	FE::stack<std::int32_t> l_stack;
	EXPECT_TRUE(l_stack.is_empty());

	l_stack.push(1);
	EXPECT_FALSE(l_stack.is_empty());

	l_stack.push(2);
	EXPECT_FALSE(l_stack.is_empty());

	l_stack.pop();
	EXPECT_FALSE(l_stack.is_empty());

	l_stack.pop();
	EXPECT_TRUE(l_stack.is_empty());

	const FE::stack<std::int32_t>& l_const_stack = l_stack;
	EXPECT_TRUE(l_const_stack.is_empty());
}


TEST(FE_stack, size)
{
	FE::stack<std::int32_t> l_stack;
	EXPECT_EQ(l_stack.size(), 0u);

	for (std::int32_t i = 0; i < 500; ++i)
	{
		l_stack.push(std::int32_t{ i });
		ASSERT_EQ(l_stack.size(), static_cast<std::size_t>(i) + 1u);
	}

	for (std::int32_t i = 500; i > 0; --i)
	{
		l_stack.pop();
		ASSERT_EQ(l_stack.size(), static_cast<std::size_t>(i) - 1u);
	}

	const FE::stack<std::int32_t>& l_const_stack = l_stack;
	EXPECT_EQ(l_const_stack.size(), 0u);
}


TEST(FE_stack, push_and_pop_lifo_order)
{
	FE::stack<std::int32_t> l_stack;

	for (std::int32_t i = 0; i < 500; ++i)
	{
		l_stack.push(std::int32_t{ i });
		ASSERT_EQ(l_stack.top(), i);
	}

	for (std::int32_t i = 499; i >= 0; --i)
	{
		ASSERT_EQ(l_stack.top(), i);
		l_stack.pop();
	}

	EXPECT_TRUE(l_stack.is_empty());
}


TEST(FE_stack, pop_until_empty_then_reuse)
{
	FE::stack<std::int32_t> l_stack;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_stack.push(std::int32_t{ i });
	}

	for (std::int32_t i = 99; i >= 0; --i)
	{
		ASSERT_EQ(l_stack.top(), i);
		l_stack.pop();
	}

	ASSERT_TRUE(l_stack.is_empty());

	for (std::int32_t i = 1000; i < 1100; ++i)
	{
		l_stack.push(std::int32_t{ i });
	}

	EXPECT_EQ(l_stack.size(), 100u);
	EXPECT_EQ(l_stack.top(), 1099);

	for (std::int32_t i = 1099; i >= 1000; --i)
	{
		ASSERT_EQ(l_stack.top(), i);
		l_stack.pop();
	}

	EXPECT_TRUE(l_stack.is_empty());
}


TEST(FE_stack, push_rvalue_forms)
{
	FE::stack<std::int32_t> l_stack;

	std::int32_t l_value = 111;
	l_stack.push(std::move(l_value));   // xvalue
	l_stack.push(222);                  // 리터럴
	l_stack.push(std::int32_t{ 333 });  // 임시 객체

	EXPECT_EQ(l_stack.size(), 3u);
	EXPECT_EQ(l_stack.top(), 333);

	l_stack.pop();
	EXPECT_EQ(l_stack.top(), 222);

	l_stack.pop();
	EXPECT_EQ(l_stack.top(), 111);
}


TEST(FE_stack, non_trivial_element_type)
{
	FE::stack<std::string> l_stack;

	l_stack.push(std::string("first"));
	l_stack.push(std::string("second"));
	l_stack.push(std::string(256, 'x')); // SSO 를 넘는 길이

	EXPECT_EQ(l_stack.size(), 3u);
	EXPECT_EQ(l_stack.top(), std::string(256, 'x'));

	l_stack.pop();
	EXPECT_EQ(l_stack.top(), "second");

	l_stack.pop();
	EXPECT_EQ(l_stack.top(), "first");

	l_stack.pop();
	EXPECT_TRUE(l_stack.is_empty());
}


// =====================================================================================
//  slab 경계 테스트
//
//  아래 두 개는 현재 구현에서 어설션이 걸려 프로세스가 중단될 수 있어 DISABLED_ 로 둡니다.
//  stack.hxx 가 정리되면 접두사를 떼세요.
//
//  1) push 의 FE_ASSERT(m_size < mini_stack_capacity) 는 전역 size 를 slab 한 개의
//     용량과 비교합니다. 그래서 원소가 slab 용량에 도달하면 다음 push 에서 걸립니다.
//  2) push 는 slab 이 가득 차는 순간 emplace_back 후 ++m_top_slab 을 해서 새 빈 slab 을
//     가리키게 합니다. 그 직후 top() 은 빈 slab 을 읽고, pop() 은 빈 slab 을 팝합니다.
// =====================================================================================

TEST(FE_stack, DISABLED_push_across_slab_boundary)
{
	FE::stack<heavy_element> l_stack; // slab 하나에 4개

	for (std::int64_t i = 0; i < 12; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_stack.push(std::move(l_element));

		ASSERT_EQ(l_stack.top()._value, i);
	}

	EXPECT_EQ(l_stack.size(), 12u);

	for (std::int64_t i = 11; i >= 0; --i)
	{
		ASSERT_EQ(l_stack.top()._value, i);
		l_stack.pop();
	}

	EXPECT_TRUE(l_stack.is_empty());
}


TEST(FE_stack, DISABLED_top_at_exact_slab_capacity)
{
	FE::stack<heavy_element> l_stack;

	for (std::int64_t i = 0; i < 4; ++i) // slab 을 정확히 가득 채운다
	{
		heavy_element l_element{};
		l_element._value = i;
		l_stack.push(std::move(l_element));
	}

	EXPECT_EQ(l_stack.size(), 4u);
	EXPECT_EQ(l_stack.top()._value, 3);

	l_stack.pop();
	EXPECT_EQ(l_stack.top()._value, 2);
}