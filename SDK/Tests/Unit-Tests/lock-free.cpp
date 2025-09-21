#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "FE/concurrent_vector.hxx"
#include <FE/algorithm/utility.hxx>




// Define a test fixture class
class ConcurrentVectorTest : public ::testing::Test 
{
protected:
   FE::concurrent_vector<std::string, std::allocator<std::string>> vec;
};

// Test the default constructor
TEST_F(ConcurrentVectorTest, DefaultConstructor) 
{
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.is_empty());
}

// Test the allocator constructor
TEST_F(ConcurrentVectorTest, AllocatorConstructor) 
{
    std::allocator<std::string> alloc;
    FE::concurrent_vector<std::string, std::allocator<std::string>> vec_with_alloc(alloc);
    EXPECT_EQ(vec_with_alloc.size(), 0);
    EXPECT_TRUE(vec_with_alloc.is_empty());
}

// Test try_push_back method
TEST_F(ConcurrentVectorTest, TryPushBack) 
{
    vec.try_push_back("FE");
    EXPECT_EQ(vec.size(), 1);

    std::string buffer;
    vec.front(buffer);
    EXPECT_EQ(buffer, "FE");

    vec.back(buffer);
    EXPECT_EQ(buffer, "FE");
}

// Test try_emplace_back method
TEST_F(ConcurrentVectorTest, TryEmplaceBack) 
{
    vec.try_emplace_back("libconcurrency");
    EXPECT_EQ(vec.size(), 1);

    std::string buffer;
    vec.front(buffer);
    EXPECT_EQ(buffer, "libconcurrency");

    vec.back(buffer);
    EXPECT_EQ(buffer, "libconcurrency");
}

// Test try_reserve method
TEST_F(ConcurrentVectorTest, TryReserve) 
{
    EXPECT_TRUE(vec.try_reserve(20));
    EXPECT_GE(vec.capacity(), 20);
}

// Test reserve method
TEST_F(ConcurrentVectorTest, ReserveContention)
{
    std::thread t[256];
    for (int i = 0; i < 64; ++i)
    {
        t[i] = std::thread
        (
            [&]()
            {
                vec.try_reserve(15 * i);
            }
        );
    }

    for (int i = 0; i < 64; ++i)
    {
        t[i].join();
    }

    EXPECT_TRUE(true);
}

// Test push_back method
TEST_F(ConcurrentVectorTest, PushBackContention)
{
    std::thread t[64];
    thread_local static std::array<char, 10> str;

    for (int i = 0; i < 64; ++i)
    {
        t[i] = std::thread
        (
            [&]()
            {
                FE::algorithm::utility::int_to_string(str.data(), str.size(), i);
                vec.push_back(str.data());
                memset(str.data(), 0, sizeof(str));
            }
        );
    }

    for (int i = 0; i < 64; ++i)
    {
        t[i].join();
    }

    EXPECT_TRUE(true);
}

// Test emplace_back method
TEST_F(ConcurrentVectorTest, EmplaceBackContention)
{
    std::thread t[256];
    thread_local static std::array<char, 10> str;
    for (int i = 0; i < 64; ++i)
    {
        t[i] = std::thread
        (
            [&]()
            {
                FE::algorithm::utility::int_to_string(str.data(), str.size(), i);
                vec.emplace_back(str.data());
                memset(str.data(), 0, sizeof(str));
            }
        );
    }

    for (int i = 0; i < 64; ++i)
    {
        t[i].join();
    }

    EXPECT_TRUE(true);
}
// Test emplace_back vs reserve method
TEST_F(ConcurrentVectorTest, EmplaceBack_vs_ReserveContention)
{
    std::thread t[256];
    thread_local static std::array<char, 10> str;
    for (int i = 0; i < 64; ++i)
    {
        if ((i % 2) == 0)
        {
            t[i] = std::thread
            (
                [&]()
                {
                    FE::algorithm::utility::int_to_string(str.data(), str.size(), i);
                    vec.emplace_back(str.data());
                    memset(str.data(), 0, sizeof(str));
                }
            );
        }
        else
        {
            t[i] = std::thread
            (
                [&]()
                {
                    vec.try_reserve(16 * i);
                }
            );
        }
    }

    for (int i = 0; i < 64; ++i)
    {
        t[i].join();
    }

    EXPECT_TRUE(true);
}

