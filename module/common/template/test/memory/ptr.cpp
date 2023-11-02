#include "gtest/gtest.h"
#include "template/memory/ownerPtr.hpp"
#include "template/memory/weakPtr.hpp"

#include "../testingObject.hpp"
#include <gmock/gmock.h>

TEST(OwnerPtr, createInstance)
{
    std::string result;
    {
        OwnerPtr<TestingObject> obj;
        obj.createInstance(result);
    }
    EXPECT_EQ(result,"(constructor)(destructor)");
}


TEST(OwnerPtr, moveBehaviour)
{
    std::string result;
    OwnerPtr<TestingObject> ptr;
    {
        auto inner = OwnerPtr<TestingObject>::CreateWithInstance(result) ;
        ptr = std::move(inner);
    }
    EXPECT_EQ(result,"(constructor)");
}

TEST(OwnerPtr, weakLifetime)
{
    std::string result;
    WeakPtr<TestingObject> weakPtr{};
    EXPECT_FALSE(weakPtr);
    {
        auto ptr = OwnerPtr<TestingObject>::CreateWithInstance(result) ;
        weakPtr = ptr.getWeak();
        EXPECT_TRUE(weakPtr);
    }
    EXPECT_FALSE(weakPtr);
}

class DeriveredTestingObject : public TestingObject
{
public:
    DeriveredTestingObject(std::string& in_res) : TestingObject(in_res) {};
};

TEST(OwnerPtr, downCast)
{
    std::string result;
    auto ptr = OwnerPtr<DeriveredTestingObject>::CreateWithInstance(result) ;
    WeakPtr<TestingObject> weakPtr = ptr.getWeak();
    EXPECT_TRUE(weakPtr);
}

TEST(OwnerPtr, unsafe_cast)
{
    std::string result;
    auto ptr = OwnerPtr<DeriveredTestingObject>::CreateWithInstance(result) ;
    WeakPtr<TestingObject> weakPtr = ptr.getWeak();
    WeakPtr<DeriveredTestingObject> unsafe_weakPtr = weakPtr.unsafe_cast<DeriveredTestingObject>();
    EXPECT_TRUE(weakPtr);
}

