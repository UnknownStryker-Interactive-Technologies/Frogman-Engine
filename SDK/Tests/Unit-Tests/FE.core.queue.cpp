// =====================================================================================
//  Test Generated Claude, and reviewed by Joey Lee
//	FE::queue - Google Test & Google Benchmark
//
//  main() 은 사용자가 통제하므로 이 파일에는 main() 이 없습니다.
//
//  [주의 1] FE:: 의 정수 별칭(FE::int32, FE::int64 ...)은 const 한정 타입이라
//           루프 카운터나 컨테이너 원소 타입으로 쓸 수 없습니다. 표준 타입을 씁니다.
//
//  [주의 2] push 는 인자를 fqueue 쪽 T&& 로 전달하므로 rvalue 만 받습니다.
//           lvalue 를 그대로 넘기면 Arguments 가 T& 로 연역되어 컴파일되지 않습니다.
// =====================================================================================
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/prerequisites.hxx>
#include <FE/queue.hxx>

#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory_resource>
#include <queue>
#include <random>
#include <string>
#include <type_traits>
#include <utility>


// slab 경계를 적은 push 로 넘기기 위한 1 KiB 짜리 원소. 4 KiB slab 하나에 4개 들어간다.
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




// =====================================================================================
//  Google Test
// =====================================================================================

TEST(FE_queue, type_aliases)
{
	static_assert(std::is_same_v<FE::queue<std::int32_t>::value_type, std::int32_t>);
	static_assert(std::is_same_v<FE::queue<std::int32_t>::reference, std::int32_t&>);
	static_assert(std::is_same_v<FE::queue<std::int32_t>::const_reference, const std::int32_t&>);
	static_assert(std::is_unsigned_v<FE::queue<std::int32_t>::size_type>);
	static_assert(std::is_same_v<xtl::queue<std::int32_t>, FE::queue<std::int32_t>>);

	SUCCEED();
}


TEST(FE_queue, default_constructor)
{
	FE::queue<std::int32_t> l_queue;

	EXPECT_TRUE(l_queue.is_empty());
	EXPECT_EQ(l_queue.size(), 0u);

	l_queue.push(7);

	EXPECT_FALSE(l_queue.is_empty());
	EXPECT_EQ(l_queue.size(), 1u);
	EXPECT_EQ(l_queue.front(), 7);
	EXPECT_EQ(l_queue.back(), 7);
}


TEST(FE_queue, allocator_constructor)
{
	counting_resource l_resource;

	FE::queue<std::int32_t> l_queue{ FE::queue<std::int32_t>::allocator_type{ &l_resource } };

	EXPECT_GT(l_resource._allocations, 0u); // 생성자가 mini queue 를 하나 만든다
	EXPECT_TRUE(l_queue.is_empty());

	for (std::int32_t i = 0; i < 5000; ++i)
	{
		l_queue.push(std::int32_t{ i });
	}

	EXPECT_EQ(l_queue.size(), 5000u);
	EXPECT_EQ(l_queue.front(), 0);
	EXPECT_EQ(l_queue.back(), 4999);
}


TEST(FE_queue, destructor_releases_memory)
{
	counting_resource l_resource;

	{
		FE::queue<std::int32_t> l_queue{ FE::queue<std::int32_t>::allocator_type{ &l_resource } };

		for (std::int32_t i = 0; i < 8192; ++i)
		{
			l_queue.push(std::int32_t{ i });
		}

		EXPECT_GT(l_resource._allocations, 1u);
	}

	EXPECT_EQ(l_resource._allocations, l_resource._deallocations);
}


TEST(FE_queue, copy_constructor)
{
	FE::queue<heavy_element> l_source;

	for (std::int64_t i = 0; i < 12; ++i) // slab 3개 분량
	{
		heavy_element l_element{};
		l_element._value = i;
		l_source.push(std::move(l_element));
	}

	for (std::int64_t i = 0; i < 4; ++i) // front slab 을 한 칸 전진시킨다
	{
		ASSERT_EQ(l_source.front()._value, i);
		l_source.pop();
	}

	FE::queue<heavy_element> l_copy(l_source);

	EXPECT_EQ(l_copy.size(), 8u);
	EXPECT_EQ(l_copy.front()._value, 4);
	EXPECT_EQ(l_copy.back()._value, 11);

	for (std::int64_t i = 4; i < 12; ++i)
	{
		ASSERT_EQ(l_copy.front()._value, i);
		l_copy.pop();
	}
	EXPECT_TRUE(l_copy.is_empty());

	// 깊은 복사 : 사본을 다 비워도 원본은 그대로
	EXPECT_EQ(l_source.size(), 8u);
	EXPECT_EQ(l_source.front()._value, 4);
}


TEST(FE_queue, copy_constructor_from_empty)
{
	FE::queue<std::int32_t> l_source;
	FE::queue<std::int32_t> l_copy(l_source);

	EXPECT_TRUE(l_copy.is_empty());
	EXPECT_EQ(l_copy.size(), 0u);

	l_copy.push(42);

	EXPECT_EQ(l_copy.front(), 42);
	EXPECT_TRUE(l_source.is_empty());
}


TEST(FE_queue, move_constructor)
{
	FE::queue<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 3000; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::queue<std::int32_t> l_moved(std::move(l_source));

	EXPECT_EQ(l_moved.size(), 3000u);
	EXPECT_EQ(l_moved.front(), 0);
	EXPECT_EQ(l_moved.back(), 2999);

	// 이동된 원본은 비어 있고 다시 쓸 수 있어야 한다
	EXPECT_TRUE(l_source.is_empty());
	EXPECT_EQ(l_source.size(), 0u);

	l_source.push(1234);

	EXPECT_EQ(l_source.size(), 1u);
	EXPECT_EQ(l_source.front(), 1234);
	EXPECT_EQ(l_moved.front(), 0);
}


TEST(FE_queue, copy_assignment)
{
	FE::queue<heavy_element> l_source;

	for (std::int64_t i = 0; i < 12; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_source.push(std::move(l_element));
	}

	FE::queue<heavy_element> l_target;

	for (std::int64_t i = 900; i < 907; ++i) // 이미 원소를 가진 대상에 대입
	{
		heavy_element l_element{};
		l_element._value = i;
		l_target.push(std::move(l_element));
	}

	l_target = l_source;

	EXPECT_EQ(l_target.size(), 12u);
	EXPECT_EQ(l_target.front()._value, 0);
	EXPECT_EQ(l_target.back()._value, 11);

	for (std::int64_t i = 0; i < 12; ++i)
	{
		ASSERT_EQ(l_target.front()._value, i);
		l_target.pop();
	}

	EXPECT_EQ(l_source.size(), 12u);
	EXPECT_EQ(l_source.front()._value, 0);
}


TEST(FE_queue, copy_assignment_to_self)
{
	FE::queue<std::int32_t> l_queue;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_queue.push(std::int32_t{ i });
	}

	FE::queue<std::int32_t>& l_alias = l_queue;
	l_queue = l_alias;

	EXPECT_EQ(l_queue.size(), 100u);
	EXPECT_EQ(l_queue.front(), 0);
	EXPECT_EQ(l_queue.back(), 99);
}


TEST(FE_queue, move_assignment)
{
	FE::queue<std::int32_t> l_source;

	for (std::int32_t i = 0; i < 2000; ++i)
	{
		l_source.push(std::int32_t{ i });
	}

	FE::queue<std::int32_t> l_target;
	l_target.push(-1);
	l_target.push(-2);

	l_target = std::move(l_source);

	EXPECT_EQ(l_target.size(), 2000u);
	EXPECT_EQ(l_target.front(), 0);
	EXPECT_EQ(l_target.back(), 1999);

	EXPECT_TRUE(l_source.is_empty());
	EXPECT_EQ(l_source.size(), 0u);

	l_source.push(555);

	EXPECT_EQ(l_source.front(), 555);
	EXPECT_EQ(l_source.size(), 1u);
}


TEST(FE_queue, move_assignment_to_self)
{
	FE::queue<std::int32_t> l_queue;

	for (std::int32_t i = 0; i < 100; ++i)
	{
		l_queue.push(std::int32_t{ i });
	}

	FE::queue<std::int32_t>& l_alias = l_queue;
	l_queue = std::move(l_alias);

	EXPECT_EQ(l_queue.size(), 100u);
	EXPECT_EQ(l_queue.front(), 0);
	EXPECT_EQ(l_queue.back(), 99);
}


TEST(FE_queue, front)
{
	FE::queue<std::int32_t> l_queue;
	l_queue.push(10);
	l_queue.push(20);
	l_queue.push(30);

	static_assert(std::is_same_v<decltype(l_queue.front()), std::int32_t&>);

	EXPECT_EQ(l_queue.front(), 10);

	l_queue.front() = 11;
	EXPECT_EQ(l_queue.front(), 11);

	l_queue.pop();
	EXPECT_EQ(l_queue.front(), 20);

	l_queue.pop();
	EXPECT_EQ(l_queue.front(), 30);
}


TEST(FE_queue, front_const)
{
	FE::queue<std::int32_t> l_queue;
	l_queue.push(10);
	l_queue.push(20);

	const FE::queue<std::int32_t>& l_const_queue = l_queue;

	static_assert(std::is_same_v<decltype(l_const_queue.front()), const std::int32_t&>);

	EXPECT_EQ(l_const_queue.front(), 10);

	l_queue.front() = 99;
	EXPECT_EQ(l_const_queue.front(), 99);
}


TEST(FE_queue, back)
{
	FE::queue<std::int32_t> l_queue;

	l_queue.push(10);
	EXPECT_EQ(l_queue.back(), 10);

	l_queue.push(20);
	EXPECT_EQ(l_queue.back(), 20);

	l_queue.push(30);
	EXPECT_EQ(l_queue.back(), 30);

	static_assert(std::is_same_v<decltype(l_queue.back()), std::int32_t&>);

	l_queue.back() = 33;
	EXPECT_EQ(l_queue.back(), 33);

	l_queue.pop(); // front 를 소모해도 back 은 유지된다
	EXPECT_EQ(l_queue.back(), 33);
	EXPECT_EQ(l_queue.front(), 20);
}


TEST(FE_queue, back_const)
{
	FE::queue<std::int32_t> l_queue;
	l_queue.push(10);
	l_queue.push(20);

	const FE::queue<std::int32_t>& l_const_queue = l_queue;

	static_assert(std::is_same_v<decltype(l_const_queue.back()), const std::int32_t&>);

	EXPECT_EQ(l_const_queue.back(), 20);
}


TEST(FE_queue, back_across_slab_boundary)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 12; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));

		ASSERT_EQ(l_queue.back()._value, i);
		ASSERT_EQ(l_queue.front()._value, 0);
	}
}


TEST(FE_queue, is_empty)
{
	FE::queue<std::int32_t> l_queue;
	EXPECT_TRUE(l_queue.is_empty());

	l_queue.push(1);
	EXPECT_FALSE(l_queue.is_empty());

	l_queue.push(2);
	EXPECT_FALSE(l_queue.is_empty());

	l_queue.pop();
	EXPECT_FALSE(l_queue.is_empty());

	l_queue.pop();
	EXPECT_TRUE(l_queue.is_empty());

	const FE::queue<std::int32_t>& l_const_queue = l_queue;
	EXPECT_TRUE(l_const_queue.is_empty());
}


TEST(FE_queue, size)
{
	FE::queue<std::int32_t> l_queue;
	EXPECT_EQ(l_queue.size(), 0u);

	for (std::int32_t i = 0; i < 3079; ++i)
	{
		l_queue.push(std::int32_t{ i });
		ASSERT_EQ(l_queue.size(), static_cast<std::size_t>(i) + 1u);
	}

	for (std::int32_t i = 0; i < 3079; ++i)
	{
		l_queue.pop();
		ASSERT_EQ(l_queue.size(), static_cast<std::size_t>(3079 - i - 1));
	}

	const FE::queue<std::int32_t>& l_const_queue = l_queue;
	EXPECT_EQ(l_const_queue.size(), 0u);
}


TEST(FE_queue, push_single_slab)
{
	FE::queue<std::int32_t> l_queue;

	for (std::int32_t i = 0; i < 10; ++i)
	{
		l_queue.push(std::int32_t{ i });

		ASSERT_EQ(l_queue.front(), 0);
		ASSERT_EQ(l_queue.back(), i);
		ASSERT_EQ(l_queue.size(), static_cast<std::size_t>(i) + 1u);
	}
}


TEST(FE_queue, push_rvalue_forms)
{
	FE::queue<std::int32_t> l_queue;

	std::int32_t l_value = 111;
	l_queue.push(std::move(l_value));   // xvalue
	l_queue.push(222);                  // 리터럴
	l_queue.push(std::int32_t{ 333 });  // 임시 객체

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.front(), 111);
	EXPECT_EQ(l_queue.back(), 333);
}


TEST(FE_queue, push_beyond_mini_queue_capacity)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 23; ++i) // slab 5개 하고도 3개 더
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	EXPECT_EQ(l_queue.size(), 23u);
	EXPECT_EQ(l_queue.front()._value, 0);
	EXPECT_EQ(l_queue.back()._value, 22);

	for (std::int64_t i = 0; i < 23; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	EXPECT_TRUE(l_queue.is_empty());
}


// ★ 이 테스트는 현재 구현에서 실패한다. 버그를 최소 단위로 고립시킨 재현 케이스이며,
//    수정 후 통과해야 한다. ★
//
// slab 하나만 쓴다. push 의 가득참 판정(size() == mini_queue_capacity)이 false 라
// 링 분기에 진입조차 하지 않는다. 즉 emplace / std::next / end() 래핑 /
// front-back slab 전진은 이 경로에서 단 한 번도 실행되지 않는다.
TEST(FE_queue, push_after_pop_within_single_slab)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 4; ++i) // slab 하나를 가득 채운다
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}
	ASSERT_EQ(l_queue.size(), 4u);

	l_queue.pop(); // size() 가 3 이 된다
	ASSERT_EQ(l_queue.front()._value, 1);

	heavy_element l_element{};
	l_element._value = 4;
	l_queue.push(std::move(l_element)); // 가득참 판정이 false 라 같은 slab 으로 들어간다
	ASSERT_EQ(l_queue.size(), 4u);

	EXPECT_EQ(l_queue.front()._value, 1);
	l_queue.pop();

	EXPECT_EQ(l_queue.front()._value, 2);
	l_queue.pop();

	EXPECT_EQ(l_queue.front()._value, 3);
	l_queue.pop();

	EXPECT_EQ(l_queue.front()._value, 4); // 버퍼 밖 슬롯을 읽으면 여기서 깨진다
	l_queue.pop();

	EXPECT_TRUE(l_queue.is_empty());
}


// front slab 이 아직 원소를 들고 있는 상태에서 링을 한 바퀴 채우면
// front 앞에 새 slab 이 삽입되어야 하고, FIFO 순서는 유지되어야 한다.
TEST(FE_queue, push_grows_when_ring_is_full)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 4; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	for (std::int64_t i = 0; i < 2; ++i) // 절반만 비운다
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	for (std::int64_t i = 4; i < 16; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	EXPECT_EQ(l_queue.size(), 14u);
	EXPECT_EQ(l_queue.front()._value, 2);
	EXPECT_EQ(l_queue.back()._value, 15);

	for (std::int64_t i = 2; i < 16; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	EXPECT_TRUE(l_queue.is_empty());
}


TEST(FE_queue, push_refills_partially_drained_slab)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 4; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	for (std::int64_t i = 0; i < 2; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	for (std::int64_t i = 4; i < 10; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	EXPECT_EQ(l_queue.size(), 8u);
	EXPECT_EQ(l_queue.front()._value, 2);
	EXPECT_EQ(l_queue.back()._value, 9);

	for (std::int64_t i = 2; i < 10; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	EXPECT_TRUE(l_queue.is_empty());
}


TEST(FE_queue, pop_preserves_fifo_order)
{
	FE::queue<std::int32_t> l_queue;

	for (std::int32_t i = 0; i < 3083; ++i)
	{
		l_queue.push(std::int32_t{ i });
	}

	for (std::int32_t i = 0; i < 3083; ++i)
	{
		ASSERT_EQ(l_queue.front(), i);
		l_queue.pop();
		ASSERT_EQ(l_queue.size(), static_cast<std::size_t>(3083 - i - 1));
	}

	EXPECT_TRUE(l_queue.is_empty());
}


TEST(FE_queue, pop_until_empty_then_reuse)
{
	FE::queue<heavy_element> l_queue;

	for (std::int64_t i = 0; i < 12; ++i)
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	for (std::int64_t i = 0; i < 12; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	ASSERT_TRUE(l_queue.is_empty());

	for (std::int64_t i = 100; i < 112; ++i) // 완전히 비운 뒤 재사용
	{
		heavy_element l_element{};
		l_element._value = i;
		l_queue.push(std::move(l_element));
	}

	EXPECT_EQ(l_queue.size(), 12u);
	EXPECT_EQ(l_queue.front()._value, 100);

	for (std::int64_t i = 100; i < 112; ++i)
	{
		ASSERT_EQ(l_queue.front()._value, i);
		l_queue.pop();
	}

	EXPECT_TRUE(l_queue.is_empty());
}


TEST(FE_queue, push_pop_cycles_repeatedly)
{
	FE::queue<heavy_element> l_queue;
	std::int64_t l_next = 0;

	for (std::int32_t round = 0; round < 16; ++round)
	{
		const std::int64_t l_begin = l_next;

		for (std::int64_t i = l_begin; i < l_begin + 8; ++i)
		{
			heavy_element l_element{};
			l_element._value = i;
			l_queue.push(std::move(l_element));
			++l_next;
		}

		for (std::int64_t i = l_begin; i < l_begin + 8; ++i)
		{
			ASSERT_EQ(l_queue.front()._value, i) << "round: " << round;
			l_queue.pop();
		}

		ASSERT_TRUE(l_queue.is_empty()) << "round: " << round;
	}
}


TEST(FE_queue, non_trivial_element_type)
{
	FE::queue<std::string> l_queue;

	l_queue.push(std::string("first"));
	l_queue.push(std::string("second"));
	l_queue.push(std::string(256, 'x')); // SSO 를 넘는 길이

	EXPECT_EQ(l_queue.size(), 3u);
	EXPECT_EQ(l_queue.front(), "first");
	EXPECT_EQ(l_queue.back(), std::string(256, 'x'));

	l_queue.pop();
	EXPECT_EQ(l_queue.front(), "second");

	l_queue.pop();
	EXPECT_EQ(l_queue.front().size(), 256u);

	l_queue.pop();
	EXPECT_TRUE(l_queue.is_empty());
}


TEST(FE_queue, matches_std_queue_behaviour)
{
	FE::queue<std::int32_t> l_queue;
	std::queue<std::int32_t> l_oracle;

	std::mt19937 l_random(20250817u);
	std::int32_t l_next_value = 0;

	for (std::int32_t step = 0; step < 200000; ++step)
	{
		if (l_oracle.empty() || ((l_random() % 3u) != 0u))
		{
			l_queue.push(std::int32_t{ l_next_value });
			l_oracle.push(l_next_value);
			++l_next_value;

			ASSERT_EQ(l_queue.back(), l_oracle.back()) << "step: " << step;
		}
		else
		{
			ASSERT_EQ(l_queue.front(), l_oracle.front()) << "step: " << step;
			l_queue.pop();
			l_oracle.pop();
		}

		ASSERT_EQ(l_queue.size(), l_oracle.size()) << "step: " << step;
		ASSERT_EQ(static_cast<bool>(l_queue.is_empty()), l_oracle.empty()) << "step: " << step;
	}
}




// =====================================================================================
//  Google Benchmark : FE::queue vs std::queue
// =====================================================================================

// N 개 push. 큐의 생성 / 소멸 비용까지 포함된다 (양쪽 동일 조건).
template <class QueueT>
void BM_push(benchmark::State& state_p)
{
	const std::int64_t l_count = state_p.range(0);

	for (auto _ : state_p)
	{
		QueueT l_queue;

		for (std::int64_t i = 0; i < l_count; ++i)
		{
			l_queue.push(static_cast<std::int32_t>(i));
		}

		benchmark::DoNotOptimize(l_queue.size());
		benchmark::ClobberMemory();
	}

	state_p.SetItemsProcessed(state_p.iterations() * l_count);
}


// N 개 pop. 채우는 구간은 타이머에서 제외한다.
template <class QueueT>
void BM_pop(benchmark::State& state_p)
{
	const std::int64_t l_count = state_p.range(0);

	for (auto _ : state_p)
	{
		state_p.PauseTiming();
		QueueT l_queue;
		for (std::int64_t i = 0; i < l_count; ++i)
		{
			l_queue.push(static_cast<std::int32_t>(i));
		}
		state_p.ResumeTiming();

		for (std::int64_t i = 0; i < l_count; ++i)
		{
			benchmark::DoNotOptimize(l_queue.front());
			l_queue.pop();
		}

		benchmark::ClobberMemory();
	}

	state_p.SetItemsProcessed(state_p.iterations() * l_count);
}


// N 개 push 후 N 개 pop (한 사이클 전체 비용)
template <class QueueT>
void BM_push_then_pop(benchmark::State& state_p)
{
	const std::int64_t l_count = state_p.range(0);

	for (auto _ : state_p)
	{
		QueueT l_queue;

		for (std::int64_t i = 0; i < l_count; ++i)
		{
			l_queue.push(static_cast<std::int32_t>(i));
		}

		for (std::int64_t i = 0; i < l_count; ++i)
		{
			benchmark::DoNotOptimize(l_queue.front());
			l_queue.pop();
		}

		benchmark::ClobberMemory();
	}

	state_p.SetItemsProcessed(state_p.iterations() * l_count * 2);
}


// 상주 원소 수를 고정한 채 push 1회 + pop 1회 반복.
// 할당 / 해제 노이즈가 가장 적어 순수 push / pop 단가 비교에 적합하다.
template <class QueueT>
void BM_steady_state_push_pop(benchmark::State& state_p)
{
	const std::int64_t l_resident = state_p.range(0);

	QueueT l_queue;
	for (std::int64_t i = 0; i < l_resident; ++i)
	{
		l_queue.push(static_cast<std::int32_t>(i));
	}

	std::int64_t l_counter = l_resident;

	for (auto _ : state_p)
	{
		l_queue.push(static_cast<std::int32_t>(l_counter));
		++l_counter;

		benchmark::DoNotOptimize(l_queue.front());
		l_queue.pop();
	}

	state_p.SetItemsProcessed(state_p.iterations());
}


// 매크로 인자에 콤마가 들어가지 않도록 별칭을 쓴다.
using fe_int_queue = FE::queue<std::int32_t>;
using std_int_queue = std::queue<std::int32_t>;

BENCHMARK_TEMPLATE(BM_push, fe_int_queue);
BENCHMARK_TEMPLATE(BM_push, std_int_queue);

BENCHMARK_TEMPLATE(BM_pop, fe_int_queue);
BENCHMARK_TEMPLATE(BM_pop, std_int_queue);

BENCHMARK_TEMPLATE(BM_push_then_pop, fe_int_queue);
BENCHMARK_TEMPLATE(BM_push_then_pop, std_int_queue);

BENCHMARK_TEMPLATE(BM_steady_state_push_pop, fe_int_queue);
BENCHMARK_TEMPLATE(BM_steady_state_push_pop, std_int_queue);