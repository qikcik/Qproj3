#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>
#include "typeSystem/structMacros.hpp"
#include "typeSystem/qStructDef.hpp"
#include "typeSystem/fieldType/all.hpp"

GEN_QSTRUCT(Foo)
{
    GEN_QSTRUCT_BODY();

    float x;
    float y;
    float z;

    std::string text;
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(Foo,{
    GEN_QSTRUCT_FIELD_ENTRY(Foo,x),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,y),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,z),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,text),
});

GEN_QSTRUCT(Bar)
{
    GEN_QSTRUCT_BODY();
    Foo inner {};
    DynamicArray<int> arr {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(Bar,{
    GEN_QSTRUCT_FIELD_ENTRY(Bar,inner),
});

TEST(QStructTest, TypeInfoCheck)
{
    EXPECT_EQ(Foo::staticDef->getField("x")->type->getTypeDesc(),"Float");
    EXPECT_EQ(Foo::staticDef->getField("y")->type->getTypeDesc(),"Float");
    EXPECT_EQ(Foo::staticDef->getField("x")->type->getTypeDesc(),"Float");
    EXPECT_EQ(Foo::staticDef->getField("text")->type->getTypeDesc(),"StdString");
}

TEST(QStructTest, GetFieldAndSetField)
{
    Foo obj = {{},2,3,4,"test"};

    EXPECT_EQ(*Foo::staticDef->getField("x")->getValuePtr<float>(&obj),2);
    EXPECT_EQ(*Foo::staticDef->getField("y")->getValuePtr<float>(&obj),3);
    EXPECT_EQ(*Foo::staticDef->getField("z")->getValuePtr<float>(&obj),4);
    EXPECT_EQ(*Foo::staticDef->getField("text")->getValuePtr<std::string>(&obj),"test");

    *Foo::staticDef->getField("text")->getValuePtr<std::string>(&obj) = "lorem ipsum";
    *Foo::staticDef->getField("z")->getValuePtr<float>(&obj) = 10;

    EXPECT_EQ(*Foo::staticDef->getField("x")->getValuePtr<float>(&obj),2);
    EXPECT_EQ(*Foo::staticDef->getField("y")->getValuePtr<float>(&obj),3);
    EXPECT_EQ(*Foo::staticDef->getField("z")->getValuePtr<float>(&obj),10);
    EXPECT_EQ(*Foo::staticDef->getField("text")->getValuePtr<std::string>(&obj),"lorem ipsum");
}

TEST(QStructTest, Array)
{
    Bar obj = {};
    obj.arr.push_back(1);
    obj.arr.push_back(2);
    obj.arr.push_back(3);

    EXPECT_EQ(obj.arr[0],1);
    EXPECT_EQ(obj.arr[1],2);
    EXPECT_EQ(obj.arr[2],3);
}