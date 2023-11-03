#include "gtest/gtest.h"

#include "typeSystem/structMacros.hpp"
#include "typeSystem/qStructDef.hpp"
#include "typeSystem/fieldType/all.hpp"
#include "typeSystem/converter.hpp"
#include "typeSystem/runtimeTypeInformation.hpp"

GEN_QSTRUCT(FooArr)
{
    GEN_QSTRUCT_BODY();

    DynamicArray<float> arr {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(FooArr,{
    GEN_QSTRUCT_FIELD_ENTRY(FooArr,arr),
});




TEST(ConverterTest, ArrayCheck)
{
    FooArr obj {};
    obj.arr.push_back(2);
    obj.arr.push_back(3);
    obj.arr.push_back(6);

    RuntimeTypeInformation rti;
    Converter conv(rti);

    auto json = conv.qstructToJson(&obj,FooArr::staticDef.getWeak());
    auto source = json.stringify();
    FooArr obj2;
    conv.jsonToQStruct(json,FooArr::staticDef.getWeak(),&obj2);

    EXPECT_EQ(obj2.arr[0],2);
    EXPECT_EQ(obj2.arr[1],3);
    EXPECT_EQ(obj2.arr[2],6);
}