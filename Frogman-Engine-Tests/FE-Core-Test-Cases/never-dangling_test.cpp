#include <gtest/gtest.h>
#include <FE/core/smart_ptrs.h>
#include <FE/core/smart_refs.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.




TEST(FE_smart_ptr_variants, never_dangling_pointer)
{
	using namespace FE;

	FE::ptr<var::byte[]> l_smart_safe_pointer;
	EXPECT_TRUE(l_smart_safe_pointer.is_expired()); // The safe pointer is pointing to nothing.

	{
		FE::exclusive_ptr<var::byte[]> l_smart_array_pointer = FE::make_exclusive<var::byte[]>(FE::size_t{ 128 });
		l_smart_safe_pointer = l_smart_array_pointer;

		EXPECT_FALSE(l_smart_safe_pointer.is_expired());

		l_smart_array_pointer.reset();  // deletes the memory to reset
		l_smart_array_pointer.reset(FE::resize_to{ 64 }); // The target object address has been changed.

		let boolean L_is_expired = l_smart_safe_pointer.is_expired(); 
		// Checks if the exclusive_ptr is invalidated, and it updates the internal pointer to avoid the dangling pointer issue (a pointer to an invalid address).
		EXPECT_FALSE(L_is_expired);
		if (L_is_expired == false)
		{
			_MAYBE_UNUSED_ auto l_will_do_something_with_this_return_value = l_smart_safe_pointer.get_unchecked();
			// operator->() and operator*() can be expensive in certain circumstances, such as loops and performance-critical contexts.
			// something happens below...
		}
	} // The exclusive pointer dies at this point.

	EXPECT_TRUE(l_smart_safe_pointer.is_expired());  // Cannot safe a dead exclusive pointer to indirect the target object.
}


TEST(std_smart_ptrs, dangling_pointer)
{
	std::shared_ptr<std::byte[]> l_shared = std::make_shared<std::byte[]>(16);
	std::weak_ptr<std::byte[]>l_watcher = l_shared; // The weak pointer is pointing to the sixteen bytes sized memory.

	{
		std::shared_ptr<std::byte[]> l_new_capacity = std::make_shared<std::byte[]>(64);
		l_shared.swap(l_new_capacity); // It swaps to extend the size.
	}

	EXPECT_TRUE(l_watcher.expired()); // the address changed, the weak pointer is dangling now.
}




TEST(FE_smart_refs, never_dangling_reference)
{
	using namespace FE;

	FE::ref<std::string> l_smart_ref;
	EXPECT_TRUE(l_smart_ref.is_expired()); // The reference is pointing to nothing.
	{
		FE::object<std::string> l_object = "Never dangle!";
		
		l_smart_ref = l_object;
		if (l_smart_ref.is_expired() == false)
		{
			_MAYBE_UNUSED_ auto l_something_does_happen_with_this_return_value = l_smart_ref.get_unchecked();
			// something happens below...
		}
	}

	EXPECT_TRUE(l_smart_ref.is_expired()); // The object is dead, the reference is not dangling and is unaccessible.

	if (l_smart_ref.is_expired() == false) _UNLIKELY_
	{
		// never reaches here.
	}
}


TEST(std_ref, dangling_reference)
{
	{
		std::unique_ptr<std::string> l_unique_string = std::make_unique<std::string>("Always dangle!");
		_MAYBE_UNUSED_ std::string& l_reference = *l_unique_string;
		l_unique_string.reset(); // The unique pointer is dead, the reference is dangling now.

		//l_reference = "Segmentation Fault"
	}
}