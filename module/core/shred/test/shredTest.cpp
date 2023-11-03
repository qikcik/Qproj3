#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "shred/shred.hpp"

TEST(Shred, ChildMustHaveUniqueName)
{
    auto root = Shred::ConstructRoot(Shred::staticDef.getWeak());
    auto a = root->appendChildren(Shred::staticDef.getWeak(),"child");
    auto b = root->appendChildren(Shred::staticDef.getWeak(),"child");

    EXPECT_EQ(a->getUniqueName(),"child_0");
    EXPECT_EQ(b->getUniqueName(),"child_1");
}