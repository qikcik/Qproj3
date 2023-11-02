#include "gtest/gtest.h"
#include "template/container/dynamicArray.hpp"
#include "template/container/dynamicArray.tpp"
#include "../testingObject.hpp"
#include <gmock/gmock.h>

TEST(DynamicArray, push_back)
{
    DynamicArray<float> arr{};
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    EXPECT_EQ(arr[0],1);
    EXPECT_EQ(arr[1],2);
    EXPECT_EQ(arr[2],3);
}

TEST(DynamicArray, push_back_inConstructo)
{
    DynamicArray<float> arr{1,2,3};

    EXPECT_EQ(arr[0],1);
    EXPECT_EQ(arr[1],2);
    EXPECT_EQ(arr[2],3);
}

TEST(DynamicArray, realocatingMemory)
{
    std::string result;

    DynamicArray<TestingObject> arr;
    arr.reserve(1);

    arr.push_back(TestingObject(result));
    EXPECT_EQ(result,"(constructor)(move constructor)(destructor)"); result = "";

    arr.reserve(2);
    EXPECT_EQ(result,"(move constructor)"); result = "";
}

TEST(DynamicArray, clear)
{
    std::string result;

    DynamicArray<TestingObject> arr;
    arr.reserve(8);
    arr.push_back(TestingObject(result));
    arr.push_back(TestingObject(result));

    result = "";
    arr.clear();
    EXPECT_EQ(result,"(destructor)(destructor)"); result = "";
}