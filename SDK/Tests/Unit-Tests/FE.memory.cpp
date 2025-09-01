#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/smart_ptr.hxx>
#include <memory_resource>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/memory.hpp>
#include <FE/algorithm/string.hxx>
using namespace FE;




TEST(memmove, string_insertion)
{
	std::unique_ptr<char[]> l_string(new char[64] {"Freddy's Pizza\0"});
	ASCII l_string2[] = " Fazbear";
	uint64 l_string2_len = algorithm::string::length(l_string2);

	uint64 l_target_location = algorithm::string::find_the_first(l_string.get(), "'")->_begin;
	auto l_rest = algorithm::string::find_the_first(l_string.get(), "'s Pizza");
	uint64 l_rest_length = l_rest->_end - l_rest->_begin;

	FE::memmove(l_string.get() + l_target_location + l_string2_len, l_string.get() + l_target_location, l_rest_length);

	FE::memcpy(l_string.get() + l_target_location, l_string2, l_string2_len);

	EXPECT_TRUE(algorithm::string::compare(l_string.get(), "Freddy Fazbear's Pizza"));
}

TEST(memmove, General)
{
	std::unique_ptr<var::int64[]> l_array(new var::int64[]{ 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1 });

	FE::memmove(l_array.get() + 3, l_array.get() + 1, sizeof(int64) * 6);

	EXPECT_EQ(l_array.get()[0], 1);
	EXPECT_EQ(l_array.get()[1], 0);
	EXPECT_EQ(l_array.get()[2], 1);
	EXPECT_EQ(l_array.get()[3], 0);
	EXPECT_EQ(l_array.get()[4], 1);
	EXPECT_EQ(l_array.get()[5], 2);
	EXPECT_EQ(l_array.get()[6], 3);
	EXPECT_EQ(l_array.get()[7], 4);
	EXPECT_EQ(l_array.get()[8], 5);
}

TEST(memcpy, General)
{
	constexpr auto l_length = 10;
	std::unique_ptr<int[]> l_source(new int [l_length] {1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
	std::unique_ptr<int[]> l_destination(new int [l_length] {0});

	FE::memcpy(l_destination.get(), l_source.get(), l_length * sizeof(int));

	for (var::int32 i = 0; i < l_length; ++i)
	{
		EXPECT_EQ(l_destination.get()[i], 1);
	}
}

TEST(memzero, General)
{
	constexpr auto l_length = 10;
	var::byte l_arr[l_length];

	FE::memzero(l_arr, l_length * sizeof(var::byte));

	for (var::int32 i = 0; i < l_length; ++i)
	{
		EXPECT_EQ(l_arr[i], 0);
	}
}




#define _MAGICAL_SIZE_ 102400

void FE_aligned_memcpy_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);

	for (auto _ : state_p)
	{
		benchmark::ClobberMemory();
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_dest, l_source, _MAGICAL_SIZE_);
		FE::memcpy<FE::Address::_Aligned, FE::Address::_Aligned>(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memcpy_benchmark)->Iterations(100000);

void std_memcpy_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);
	alignas(64) static std::byte l_source[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_source);

	for (auto _ : state_p)
	{
		benchmark::ClobberMemory();
		std::memcpy(l_dest, l_source, _MAGICAL_SIZE_);
		std::memcpy(l_source, l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(std_memcpy_benchmark)->Iterations(100000);


void FE_aligned_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		FE::memmove(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(FE_aligned_memmove_benchmark)->Iterations(100000);

void std_memmove_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_mem[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_mem);

	for (auto _ : state_p)
	{
		std::memmove(l_mem + 64, l_mem + 128, 10240);
	}
}
BENCHMARK(std_memmove_benchmark)->Iterations(100000);


void FE_aligned_memzero_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		FE::memzero<FE::Address::_Aligned>(l_dest, _MAGICAL_SIZE_);
	}
}
BENCHMARK(FE_aligned_memzero_benchmark)->Iterations(100000);

void std_memset_benchmark(benchmark::State& state_p) noexcept
{
	alignas(64) static std::byte l_dest[_MAGICAL_SIZE_];
	benchmark::DoNotOptimize(l_dest);

	for (auto _ : state_p)
	{
		std::memset(l_dest, null, _MAGICAL_SIZE_);
		std::memset(l_dest, 1, _MAGICAL_SIZE_);
	}
}
BENCHMARK(std_memset_benchmark)->Iterations(100000);


#undef _MAGICAL_SIZE_ 




// A simple struct for testing allocation and construction.
struct TestObject
{
	int value;
	TestObject(int v = 0) : value(v) {}
};

// Base and Derived classes for testing polymorphism.
struct Base
{
	virtual ~Base() = default;
	int base_value = 10;
};

struct Derived : public Base
{
	int derived_value = 20;
};

// Test fixture for smart_ptr tests
class SmartPtrTest : public ::testing::Test
{
protected:
	std::pmr::memory_resource* m_resource;

	void SetUp() override
	{
		m_resource = std::pmr::get_default_resource();
	}
};

// Test cases for smart_ptr<T, RefType::_Owner>
TEST_F(SmartPtrTest, Owner_DefaultConstructor)
{
	FE::smart_ptr<TestObject, FE::RefType::_Owner> owner;
	EXPECT_TRUE(owner.is_null());
}

TEST_F(SmartPtrTest, Owner_Creation)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	ASSERT_FALSE(owner.is_null());
	EXPECT_EQ(owner->value, 42);
}

TEST_F(SmartPtrTest, Owner_MoveConstructor)
{
	auto owner1 = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Owner> owner2 = std::move(owner1);

	EXPECT_TRUE(owner1.is_null());
	ASSERT_FALSE(owner2.is_null());
	EXPECT_EQ(owner2->value, 42);
}

TEST_F(SmartPtrTest, Owner_MoveAssignment)
{
	auto owner1 = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Owner> owner2;
	owner2 = std::move(owner1);

	EXPECT_TRUE(owner1.is_null());
	ASSERT_FALSE(owner2.is_null());
	EXPECT_EQ(owner2->value, 42);
}

TEST_F(SmartPtrTest, Owner_Reset)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	ASSERT_FALSE(owner.is_null());
	owner.reset();
	EXPECT_TRUE(owner.is_null());
}

TEST_F(SmartPtrTest, Owner_Swap)
{
	auto owner1 = FE::make_owner<TestObject>(m_resource, 10);
	auto owner2 = FE::make_owner<TestObject>(m_resource, 20);

	owner1.swap(owner2);

	ASSERT_FALSE(owner1.is_null());
	EXPECT_EQ(owner1->value, 20);
	ASSERT_FALSE(owner2.is_null());
	EXPECT_EQ(owner2->value, 10);
}

TEST_F(SmartPtrTest, Owner_Dereference)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 123);
	EXPECT_EQ(owner->value, 123);
	EXPECT_EQ((*owner).value, 123);
}

// Test cases for smart_ptr<T, RefType::_Observer>
TEST_F(SmartPtrTest, Observer_DefaultConstructor)
{
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer;
	EXPECT_FALSE(observer.is_valid());
}

TEST_F(SmartPtrTest, Observer_CreateFromOwner)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer(owner);

	ASSERT_TRUE(observer.is_valid());
	EXPECT_EQ(observer->value, 42);
}

TEST_F(SmartPtrTest, Observer_CopyConstructor)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer1(owner);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer2(observer1);

	ASSERT_TRUE(observer1.is_valid());
	ASSERT_TRUE(observer2.is_valid());
	EXPECT_EQ(observer2->value, 42);
}

TEST_F(SmartPtrTest, Observer_CopyAssignment)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer1(owner);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer2;

	observer2 = observer1;

	ASSERT_TRUE(observer1.is_valid());
	ASSERT_TRUE(observer2.is_valid());
	EXPECT_EQ(observer2->value, 42);
}

TEST_F(SmartPtrTest, Observer_MoveConstructor)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer1(owner);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer2 = std::move(observer1);

	EXPECT_FALSE(observer1.is_valid());
	ASSERT_TRUE(observer2.is_valid());
	EXPECT_EQ(observer2->value, 42);
}

TEST_F(SmartPtrTest, Observer_MoveAssignment)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer1(owner);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer2;
	observer2 = std::move(observer1);

	EXPECT_FALSE(observer1.is_valid());
	ASSERT_TRUE(observer2.is_valid());
	EXPECT_EQ(observer2->value, 42);
}

TEST_F(SmartPtrTest, Observer_Reset)
{
	auto owner = FE::make_owner<TestObject>(m_resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer(owner);
	ASSERT_TRUE(observer.is_valid());

	observer.reset();
	EXPECT_FALSE(observer.is_valid());
}

TEST_F(SmartPtrTest, Observer_IsExpired)
{
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer;
	{
		auto owner = FE::make_owner<TestObject>(m_resource, 42);
		observer = owner;
		ASSERT_TRUE(observer.is_valid());
	} // owner goes out of scope and is destroyed
	EXPECT_FALSE(observer.is_valid());
}

// Polymorphism Tests
TEST_F(SmartPtrTest, Polymorphism_OwnerMove)
{
	auto derived_owner = FE::make_owner<Derived>(m_resource);
	FE::smart_ptr<Base, FE::RefType::_Owner> base_owner = std::move(derived_owner);

	EXPECT_TRUE(derived_owner.is_null());
	ASSERT_FALSE(base_owner.is_null());
	EXPECT_EQ(base_owner->base_value, 10);
}

TEST_F(SmartPtrTest, Polymorphism_ObserverCreate)
{
	auto derived_owner = FE::make_owner<Derived>(m_resource);
	FE::smart_ptr<Base, FE::RefType::_Observer> base_observer(derived_owner);

	ASSERT_TRUE(base_observer.is_valid());
	EXPECT_EQ(base_observer->base_value, 10);
}

TEST_F(SmartPtrTest, Polymorphism_ObserverConversions)
{
	auto derived_owner = FE::make_owner<Derived>(m_resource);

	// Test polymorphic copy assignment
	FE::smart_ptr<Derived, FE::RefType::_Observer> derived_observer1(derived_owner);
	FE::smart_ptr<Base, FE::RefType::_Observer> base_observer1;
	base_observer1 = derived_observer1;
	ASSERT_TRUE(base_observer1.is_valid());
	EXPECT_EQ(base_observer1->base_value, 10);

	// Test polymorphic move constructor
	FE::smart_ptr<Derived, FE::RefType::_Observer> derived_observer2(derived_owner);
	FE::smart_ptr<Base, FE::RefType::_Observer> base_observer2 = std::move(derived_observer2);
	EXPECT_FALSE(derived_observer2.is_valid());
	ASSERT_TRUE(base_observer2.is_valid());
	EXPECT_EQ(base_observer2->base_value, 10);

	// Test polymorphic move assignment
	FE::smart_ptr<Derived, FE::RefType::_Observer> derived_observer3(derived_owner);
	FE::smart_ptr<Base, FE::RefType::_Observer> base_observer3;
	base_observer3 = std::move(derived_observer3);
	EXPECT_FALSE(derived_observer3.is_valid());
	ASSERT_TRUE(base_observer3.is_valid());
	EXPECT_EQ(base_observer3->base_value, 10);
}




// --- Benchmarks for smart_ptr vs std::shared_ptr/weak_ptr ---

// Benchmark for FE::smart_ptr owner creation
static void FESmartPtr_OwnerCreation(benchmark::State& state)
{
	auto resource = std::pmr::get_default_resource();
	for (auto _ : state)
	{
		auto owner = FE::make_owner<TestObject>(resource, 42);
		benchmark::DoNotOptimize(owner);
	}
}
BENCHMARK(FESmartPtr_OwnerCreation);

// Benchmark for std::shared_ptr creation
static void StdSharedPtr_Creation(benchmark::State& state)
{
	for (auto _ : state)
	{
		auto owner = std::make_shared<TestObject>(42);
		benchmark::DoNotOptimize(owner);
	}
}
BENCHMARK(StdSharedPtr_Creation);

// Benchmark for FE::smart_ptr observer creation
static void FESmartPtr_ObserverCreation(benchmark::State& state)
{
	auto resource = std::pmr::get_default_resource();
	auto owner = FE::make_owner<TestObject>(resource, 42);
	for (auto _ : state)
	{
		FE::smart_ptr<TestObject, FE::RefType::_Observer> observer(owner);
		benchmark::DoNotOptimize(observer);
	}
}
BENCHMARK(FESmartPtr_ObserverCreation);

// Benchmark for std::weak_ptr creation
static void StdWeakPtr_Creation(benchmark::State& state)
{
	auto owner = std::make_shared<TestObject>(42);
	for (auto _ : state)
	{
		std::weak_ptr<TestObject> observer(owner);
		benchmark::DoNotOptimize(observer);
	}
}
BENCHMARK(StdWeakPtr_Creation);

// Benchmark for FE::smart_ptr observer copy
static void FESmartPtr_ObserverCopy(benchmark::State& state)
{
	auto resource = std::pmr::get_default_resource();
	auto owner = FE::make_owner<TestObject>(resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer1(owner);
	for (auto _ : state)
	{
		FE::smart_ptr<TestObject, FE::RefType::_Observer> observer2 = observer1;
		benchmark::DoNotOptimize(observer2);
	}
}
BENCHMARK(FESmartPtr_ObserverCopy);

// Benchmark for std::weak_ptr copy
static void StdWeakPtr_Copy(benchmark::State& state)
{
	auto owner = std::make_shared<TestObject>(42);
	std::weak_ptr<TestObject> observer1(owner);
	for (auto _ : state)
	{
		std::weak_ptr<TestObject> observer2 = observer1;
		benchmark::DoNotOptimize(observer2);
	}
}
BENCHMARK(StdWeakPtr_Copy);

// Benchmark for dereferencing FE::smart_ptr owner
static void FESmartPtr_OwnerDereference(benchmark::State& state)
{
	auto resource = std::pmr::get_default_resource();
	auto owner = FE::make_owner<TestObject>(resource, 42);
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(owner->value);
	}
}
BENCHMARK(FESmartPtr_OwnerDereference);

// Benchmark for dereferencing std::shared_ptr
static void StdSharedPtr_Dereference(benchmark::State& state)
{
	auto owner = std::make_shared<TestObject>(42);
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(owner->value);
	}
}
BENCHMARK(StdSharedPtr_Dereference);

// Benchmark for checking validity and dereferencing FE::smart_ptr observer
static void FESmartPtr_ObserverDereference(benchmark::State& state)
{
	auto resource = std::pmr::get_default_resource();
	auto owner = FE::make_owner<TestObject>(resource, 42);
	FE::smart_ptr<TestObject, FE::RefType::_Observer> observer(owner);
	for (auto _ : state)
	{
		if (observer.is_valid())
		{
			benchmark::DoNotOptimize(observer->value);
		}
	}
}
BENCHMARK(FESmartPtr_ObserverDereference);

// Benchmark for locking and dereferencing std::weak_ptr
static void StdWeakPtr_LockAndDereference(benchmark::State& state)
{
	auto owner = std::make_shared<TestObject>(42);
	std::weak_ptr<TestObject> observer(owner);
	for (auto _ : state)
	{
		if (auto locked = observer.lock())
		{
			benchmark::DoNotOptimize(locked->value);
		}
	}
}
BENCHMARK(StdWeakPtr_LockAndDereference);