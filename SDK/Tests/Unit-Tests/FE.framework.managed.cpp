//#include <gtest/gtest.h>
//#include <benchmark/benchmark.h>
//
//#include <FE/framework/managed/trackable.hxx>
//#include <FE/framework/managed/exclusive_ptr.hxx>
//#include <FE/framework/managed/unique_ptr.hxx>
//#include <FE/framework/managed/ptr.hxx>
//#include <FE/framework/string.hxx>
//// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
//
//// std
//#include <vector>
//#include <string>
//#include <memory>
//
//
//// need to be optimized
//
//TEST(exclusive_ptr, construct_by_value_assignment)
//{
//	using namespace FE;
//	
//	{
//		{
//			FE::exclusive_ptr<var::int64> l_exclusive_ptr = 64;
//		}
//
//		FE::exclusive_ptr<var::int64> l_exclusive_ptr;
//		l_exclusive_ptr = 64;
//	}
//
//
//	{
//		{
//			FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = std::vector{ 1, 2, 3, 4, 5 };
//		}
//
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr;
//		l_exclusive_ptr = { 1, 2, 3, 4, 5 };
//	}
//}
//
//
//TEST(exclusive_ptr, release)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = std::vector{ 1, 2, 3, 4, 5 };
//		FE::exclusive_ptr<std::vector<int>>::pointer l_pointer = l_exclusive_ptr.release();
//		EXPECT_EQ(l_exclusive_ptr.get(), nullptr);
//		l_exclusive_ptr.get_allocator().deallocate(l_pointer, 1);
//	}
//}
//
//
//TEST(exclusive_ptr, reset)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = std::vector{ 1, 2, 3, 4, 5 };
//		l_exclusive_ptr.reset();
//		EXPECT_EQ(l_exclusive_ptr.get(), nullptr);
//	}
//}
//
//
//TEST(exclusive_ptr, swap)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr;
//		FE::exclusive_ptr<std::vector<int>> l_another = std::vector{ 1, 2, 3, 4, 5 };
//		
//		l_exclusive_ptr.swap(l_another);
//		EXPECT_EQ((*l_exclusive_ptr)[0], 1);
//	}
//}
//
//
//TEST(exclusive_ptr, make_exclusive)
//{
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = FE::make_exclusive<std::vector<int>>();
//
//		EXPECT_FALSE(l_exclusive_ptr == nullptr);
//	}
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = FE::make_exclusive<std::vector<int>>({ 1, 2, 3, 4, 5 });
//
//		EXPECT_FALSE(l_exclusive_ptr == nullptr);
//
//		EXPECT_EQ((*l_exclusive_ptr)[0], 1);
//	}
//}
//
//
//TEST(smart_ptr_variants, exclusive_ptr_and_safe_ptrs)
//{
//	FE::ptr<std::vector<int>> l_first_safe_ptr;
//	FE::ptr<std::vector<int>> l_second_safe_ptr;
//	FE::ptr<std::vector<int>> l_third_safe_ptr;
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_exclusive_ptr = FE::make_exclusive<std::vector<int>>({ 1, 2, 3, 4, 5 });
//		l_first_safe_ptr = l_exclusive_ptr;
//		l_second_safe_ptr = l_exclusive_ptr;
//		l_third_safe_ptr = l_second_safe_ptr;
//	}
//
//	EXPECT_TRUE(l_first_safe_ptr.is_expired());
//	EXPECT_TRUE(l_second_safe_ptr.is_expired());
//	EXPECT_TRUE(l_third_safe_ptr.is_expired());
//
//	{
//		FE::exclusive_ptr<std::vector<int>> l_second_exclusive_ptr;
//		{
//			FE::exclusive_ptr<std::vector<int>> l_first_exclusive_ptr = FE::make_exclusive<std::vector<int>>({ 1, 2, 3, 4, 5 });
//			l_first_safe_ptr = l_first_exclusive_ptr;
//			l_second_safe_ptr = l_first_exclusive_ptr;
//			l_third_safe_ptr = l_first_exclusive_ptr;
//
//			l_second_exclusive_ptr = std::move(l_first_exclusive_ptr);
//		}
//		l_first_safe_ptr.operator->();
//	}
//}
//
//
//TEST(exclusive_ptr, construct_array_by_value_assignment)
//{
//	using namespace FE;
//
//	{
//		{
//			FE::exclusive_ptr<var::int64[]> l_exclusive_ptr = { 0, 1, 2, 3 };
//		}
//
//		FE::exclusive_ptr<var::int64[]> l_exclusive_ptr;
//		l_exclusive_ptr = { 0, 1, 2, 3 };
//	}
//
//	{
//		{
//			FE::exclusive_ptr<std::string[]> l_exclusive_ptr = { "std::string" };
//		}
//
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr;
//		l_exclusive_ptr = { "std::string" };
//	}
//
//
//
//
//	{
//		FE::exclusive_ptr<var::int64[]> l_exclusive_ptr = FE::reserve{ 10 };
//	}
//}
//
//
//TEST(exclusive_ptr, release_array)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr = { "std::string" };
//		FE::size l_prev_size = l_exclusive_ptr.capacity();
//
//		FE::exclusive_ptr<std::string[]>::pointer l_pointer = l_exclusive_ptr.release();
//		EXPECT_EQ(l_exclusive_ptr.get(), nullptr);
//		l_exclusive_ptr.get_allocator().deallocate(l_pointer, l_prev_size);
//	}
//}
//
//
//TEST(exclusive_ptr, reset_array)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr = { "std::string" };
//		l_exclusive_ptr.reset();
//		EXPECT_EQ(l_exclusive_ptr.get(), nullptr);
//	}
//}
//
//
//TEST(exclusive_ptr, swap_array)
//{
//	using namespace FE;
//
//	{
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr = { "std::string" };
//		FE::exclusive_ptr<std::string[]> l_another = { };
//
//		l_exclusive_ptr.swap(l_another);
//		EXPECT_EQ(strcmp(l_another[0].c_str(), "std::string"), 0);
//	}
//}
//
//
//TEST(exclusive_ptr, make_exclusive_array)
//{
//	{
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr = FE::make_exclusive<std::string[]>(4);
//		EXPECT_FALSE(l_exclusive_ptr == nullptr);
//	}
//
//	{
//		FE::exclusive_ptr<std::string[]> l_exclusive_ptr = FE::make_exclusive<std::string[]>({ "std", "string" });
//
//		EXPECT_FALSE(l_exclusive_ptr == nullptr);
//
//		EXPECT_EQ(strcmp(l_exclusive_ptr[0].c_str(), "std"), 0);
//		EXPECT_EQ(strcmp(l_exclusive_ptr[1].c_str(), "string"), 0);
//	}
//}
//
//
//TEST(smart_ptr_variants, array_exclusive_ptr_and_safe_ptrs)
//{
//	FE::ptr<std::vector<int>[]> l_first_safe_ptr;
//	FE::ptr<std::vector<int>[]> l_second_safe_ptr;
//	FE::ptr<std::vector<int>[]> l_third_safe_ptr;
//
//	{
//		FE::exclusive_ptr<std::vector<int>[]> l_exclusive_ptr = FE::make_exclusive<std::vector<int>[]>(10);
//		l_first_safe_ptr = l_exclusive_ptr;
//		l_second_safe_ptr = l_exclusive_ptr;
//		l_third_safe_ptr = l_second_safe_ptr;
//	}
//
//	EXPECT_TRUE(l_first_safe_ptr.is_expired());
//	EXPECT_TRUE(l_second_safe_ptr.is_expired());
//	EXPECT_TRUE(l_third_safe_ptr.is_expired());
//	
//	{
//		FE::exclusive_ptr<std::vector<int>[]> l_first_exclusive_ptr = FE::make_exclusive<std::vector<int>[]>({ std::vector<int>{ 1, 2, 3 }, std::vector<int>{ 4, 5, 6 } });
//		FE::exclusive_ptr<std::vector<int>[]> l_second_exclusive_ptr = FE::make_exclusive<std::vector<int>[]>({ std::vector<int>{ 1, 2, 3 }, std::vector<int>{ 4, 5, 6 } });
//		l_first_safe_ptr = l_second_exclusive_ptr;
//		l_second_safe_ptr = l_second_exclusive_ptr;
//		l_third_safe_ptr = l_second_safe_ptr;
//
//		EXPECT_FALSE(l_first_safe_ptr.is_expired());
//		EXPECT_FALSE(l_second_safe_ptr.is_expired());
//		EXPECT_FALSE(l_third_safe_ptr.is_expired());
//
//		FE::exclusive_ptr<std::vector<int>[]> l_third_exclusive_ptr = std::move(l_first_exclusive_ptr);
//
//		EXPECT_FALSE(l_first_safe_ptr.is_expired());
//		EXPECT_FALSE(l_second_safe_ptr.is_expired());
//		EXPECT_FALSE(l_third_safe_ptr.is_expired());
//
//		EXPECT_TRUE(l_first_exclusive_ptr == nullptr);
//
//		EXPECT_EQ((*l_first_safe_ptr)[0], 1);
//		EXPECT_EQ((*l_second_safe_ptr)[0], 1);
//		EXPECT_EQ((*l_third_safe_ptr)[0], 1);
//	}
//
//	EXPECT_TRUE(l_first_safe_ptr.is_expired());
//	EXPECT_TRUE(l_second_safe_ptr.is_expired());
//	EXPECT_TRUE(l_third_safe_ptr.is_expired());
//}
//
//
//TEST(FE_smart_ptr_variants, never_dangling_pointer)
//{
//	using namespace FE;
//
//	FE::ptr<var::byte[]> l_smart_safe_pointer;
//	EXPECT_TRUE(l_smart_safe_pointer.is_expired()); // The safe pointer is pointing to nothing.
//
//	{
//		FE::exclusive_ptr<var::byte[]> l_smart_array_pointer = FE::make_exclusive<var::byte[]>(FE::size{ 128 });
//		l_smart_safe_pointer = l_smart_array_pointer;
//
//		EXPECT_FALSE(l_smart_safe_pointer.is_expired());
//
//		l_smart_array_pointer.reset();  // deletes the memory to reset
//		l_smart_array_pointer.reset(FE::resize_to{ 64 }); // The target object address has been changed.
//
//		boolean l_is_expired = l_smart_safe_pointer.is_expired(); 
//		// Checks if the exclusive_ptr is invalidated to avoid the dangling pointer issue (a pointer to an invalid address).
//		EXPECT_FALSE(l_is_expired);
//		if (l_is_expired == false)
//		{
//			auto* l_raw = l_smart_safe_pointer.get();
//			(void)l_raw;
//		}
//	} // The exclusive pointer dies at this point.
//
//	EXPECT_TRUE(l_smart_safe_pointer.is_expired()); 
//}
//
//
//TEST(std_smart_ptrs, dangling_pointer)
//{
//	using namespace FE;
//	std::shared_ptr<var::int16> l_shared = std::make_shared<var::int16>(std::size_t(16));
//	std::weak_ptr<var::int16>l_watcher = l_shared; // The weak pointer is pointing to the sixteen bytes sized memory.
//
//	{
//		std::shared_ptr<var::int16> l_new_capacity = std::make_shared<var::int16>(std::size_t(64));
//		l_shared.swap(l_new_capacity); // It swaps to extend the size.
//	}
//
//	EXPECT_TRUE(l_watcher.expired()); // the address changed, the weak pointer is dangling now.
//}
//
//
//TEST(FE_smart_refs, never_dangling_reference)
//{
//	using namespace FE;
//
//	FE::ptr<std::string> l_smart_ref;
//	EXPECT_TRUE(l_smart_ref.is_expired()); // The reference is pointing to nothing.
//	{
//		FE::trackable<std::string> l_object = "Never dangle!";
//		
//		l_smart_ref = l_object;
//		if (l_smart_ref.is_expired() == false)
//		{
//			auto& l_raw = *l_smart_ref.get();
//			(void)l_raw;
//		}
//	}
//
//	EXPECT_TRUE(l_smart_ref.is_expired()); // The object is dead, the reference is not dangling and is unaccessible.
//}
//
//
//TEST(std_ref, dangling_reference)
//{
//	{
//		std::unique_ptr<std::string> l_unique_string = std::make_unique<std::string>("Always dangle!");
//		_FE_MAYBE_UNUSED_ std::string& l_reference = *l_unique_string;
//		l_unique_string.reset(); // The unique pointer is dead, the reference is dangling now.
//
//		//l_reference = "Segmentation Fault"
//	}
//}
//
//
//TEST(unique_ptr, construct_by_value_assignment)
//{
//	using namespace FE;
//
//	{
//		{
//			FE::unique_ptr<var::int64> l_unique_ptr = 64;
//		}
//
//		FE::unique_ptr<var::int64> l_unique_ptr;
//		l_unique_ptr = 64;
//	}
//
//
//	{
//		{
//			FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
//		}
//
//		FE::unique_ptr<std::string> l_unique_ptr;
//		l_unique_ptr = std::string("std::string");
//	}
//}
//
//
//TEST(unique_ptr, release)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
//		FE::unique_ptr<std::string>::pointer l_pointer = l_unique_ptr.release();
//		EXPECT_EQ(l_unique_ptr.get(), nullptr);
//		l_unique_ptr.get_allocator().deallocate(l_pointer, 1);
//	}
//}
//
//
//TEST(unique_ptr, reset)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
//		l_unique_ptr.reset();
//		EXPECT_EQ(l_unique_ptr.get(), nullptr);
//	}
//}
//
//
//TEST(unique_ptr, swap)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string> l_unique_ptr = std::string("std::string");
//		FE::unique_ptr<std::string> l_another = std::string();
//
//		l_unique_ptr.swap(l_another);
//		EXPECT_EQ(strcmp(l_another->c_str(), "std::string"), 0);
//	}
//}
//
//
//TEST(unique_ptr, make_unique)
//{
//	{
//		FE::unique_ptr<std::string> l_unique_ptr = FE::make_unique<std::string>();
//
//		EXPECT_FALSE(l_unique_ptr == nullptr);
//	}
//
//	{
//		FE::unique_ptr<std::string> l_unique_ptr = FE::make_unique<std::string>("std::string");
//
//		EXPECT_FALSE(l_unique_ptr == nullptr);
//
//		EXPECT_EQ(strcmp(l_unique_ptr->c_str(), "std::string"), 0);
//	}
//}
//
//
//TEST(unique_ptr, construct_array_by_value_assignment)
//{
//	using namespace FE;
//
//	{
//		{
//			FE::unique_ptr<var::int64[]> l_unique_ptr = {0, 1, 2, 3};
//		}
//
//		FE::unique_ptr<var::int64[]> l_unique_ptr;
//		l_unique_ptr = { 0, 1, 2, 3 };
//	}
//
//	{
//		{
//			FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
//		}
//
//		FE::unique_ptr<std::string[]> l_unique_ptr;
//		l_unique_ptr = { "std::string" };
//	}
//
//
//
//
//	{
//		FE::unique_ptr<var::int64[]> l_unique_ptr = FE::reserve{ 10 };
//	}
//}
//
//
//TEST(unique_ptr, release_array)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
//		FE::size l_prev_size = l_unique_ptr.capacity();
//
//		FE::unique_ptr<std::string[]>::pointer l_pointer = l_unique_ptr.release();
//		EXPECT_EQ(l_unique_ptr.get(), nullptr);
//		l_unique_ptr.get_allocator().deallocate(l_pointer, l_prev_size);
//	}
//}
//
//
//TEST(unique_ptr, reset_array)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
//		l_unique_ptr.reset();
//		EXPECT_EQ(l_unique_ptr.get(), nullptr);
//	}
//}
//
//
//TEST(unique_ptr, swap_array)
//{
//	using namespace FE;
//
//	{
//		FE::unique_ptr<std::string[]> l_unique_ptr = { "std::string" };
//		FE::unique_ptr<std::string[]> l_another = { };
//
//		l_unique_ptr.swap(l_another);
//		EXPECT_EQ(strcmp(l_another[0].c_str(), "std::string"), 0);
//	}
//}
//
//
//TEST(unique_ptr, make_unique_array)
//{
//	{
//		FE::unique_ptr<std::string[]> l_unique_ptr = FE::make_unique<std::string[]>(4);
//		EXPECT_FALSE(l_unique_ptr == nullptr);
//	}
//
//	{
//		FE::unique_ptr<std::string[]> l_unique_ptr = FE::make_unique<std::string[]>({ "std", "string" });
//
//		EXPECT_FALSE(l_unique_ptr == nullptr);
//
//		EXPECT_EQ(strcmp(l_unique_ptr[0].c_str(), "std"), 0);
//		EXPECT_EQ(strcmp(l_unique_ptr[1].c_str(), "string"), 0);
//	}
//}
//
//
//
//
//void FE_unique_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
//{
//	for (auto _ : state_p)
//	{
//		FE::unique_ptr<std::byte[]> l_unique_ptr = FE::make_unique<std::byte[]>(1024);
//
//	}
//}
//BENCHMARK(FE_unique_ptr_RAII_construction_and_destruction);
//
//
//void std_unique_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
//{
//	for (auto _ : state_p)
//	{
//		std::unique_ptr<std::byte[]> l_unique_ptr = std::make_unique<std::byte[]>(1024);
//
//	}
//}
//BENCHMARK(std_unique_ptr_RAII_construction_and_destruction);
//
//
//void FE_exclusive_ptr_RAII_construction_and_destruction(benchmark::State& state_p) noexcept
//{
//	for (auto _ : state_p)
//	{
//		FE::exclusive_ptr<std::byte[]> l_exclusive_ptr = FE::make_exclusive<std::byte[]>(1024);
//
//	}
//}
//BENCHMARK(FE_exclusive_ptr_RAII_construction_and_destruction);
//
//
//#define _MAGICAL_SIZE_ 8
//void FE_smart_ptrs(benchmark::State& state_p) noexcept
//{
//	using namespace FE;
//	FE::exclusive_ptr<var::uint32[]> l_exclusive_ptr = FE::make_exclusive<var::uint32[]>(_MAGICAL_SIZE_);
//
//	for (auto _ : state_p)
//	{
//		FE::ptr<var::uint32[]> l_safe_ptr = l_exclusive_ptr;
//		if (l_safe_ptr.is_expired() == false)
//		{
//			auto* l_ptr = l_safe_ptr.get();
//			l_ptr = l_ptr + 1;
//		}
//	}
//
//}
//BENCHMARK(FE_smart_ptrs);
//void FE_smart_refs(benchmark::State& state_p) noexcept
//{
//	using namespace FE;
//	FE::trackable<var::uint32> l_trackable_uint32 = 7;
//
//	for (auto _ : state_p)
//	{
//		FE::ptr<var::uint32> l_safe_ref = l_trackable_uint32;
//		if (l_safe_ref.is_expired() == false)
//		{
//			auto& l_ref = *l_safe_ref.get();
//			(void)l_ref;
//		}
//	}
//
//}
//BENCHMARK(FE_smart_refs);
//
//void std_smart_ptrs(benchmark::State& state_p) noexcept
//{
//	using namespace FE;
//	std::shared_ptr<var::uint32> l_shared_ptr = std::make_shared<var::uint32>(_MAGICAL_SIZE_);
//
//	for (auto _ : state_p)
//	{
//		std::weak_ptr<var::uint32> l_weak_ptr = l_shared_ptr;
//		if (l_weak_ptr.expired() == false)
//		{
//			auto* l_ptr = l_weak_ptr.lock().get();
//			l_ptr = l_ptr + 1;
//		}
//	}
//}
//BENCHMARK(std_smart_ptrs);
//#undef _MAGICAL_SIZE_
