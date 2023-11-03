#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "typeSystem/structMacros.hpp"
#include "typeSystem/qStructDef.hpp"
#include "typeSystem/fieldType/all.hpp"
#include "typeSystem/converter.hpp"
#include "typeSystem/runtimeTypeInformation.hpp"

GEN_QSTRUCT(FooConv)
{
    GEN_QSTRUCT_BODY();

    float x;
    float y;
    float z;

    std::string text;
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(FooConv,{
    GEN_QSTRUCT_FIELD_ENTRY(FooConv,x),
    GEN_QSTRUCT_FIELD_ENTRY(FooConv,y),
    GEN_QSTRUCT_FIELD_ENTRY(FooConv,z),
    GEN_QSTRUCT_FIELD_ENTRY(FooConv,text),
});

GEN_QSTRUCT(BarConv)
{
    GEN_QSTRUCT_BODY();

    std::string text {};
    FooConv foo {};
    DynamicArray<FooConv> arr {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(BarConv,{
    GEN_QSTRUCT_FIELD_ENTRY(BarConv,text),
    GEN_QSTRUCT_FIELD_ENTRY(BarConv,foo),
    GEN_QSTRUCT_FIELD_ENTRY(BarConv,arr),
});

TEST(ConverterTest, Test)
{
    BarConv bar {{},"lorem",{{},1,2,3,"lorem ipsum"}};
    bar.arr.push_back({{},1,2,3,"lorem ipsum"});
    bar.arr.push_back({{},4,5,6,"ipsum lorem"});

    RuntimeTypeInformation rti;
    Converter conv(rti);
    auto json = conv.qstructToJson(&bar,BarConv::staticDef.getWeak());
    auto source = json.stringify();
    BarConv bar2;
    conv.jsonToQStruct(json,BarConv::staticDef.getWeak(),&bar2);
    EXPECT_EQ(bar2.foo.x,1);
    EXPECT_EQ(bar2.foo.y,2);
    EXPECT_EQ(bar2.foo.z,3);
    EXPECT_EQ(bar2.foo.text,"lorem ipsum");

    EXPECT_EQ(bar2.arr[0].text,"lorem ipsum");
    EXPECT_EQ(bar2.arr[1].text,"ipsum lorem");
}
