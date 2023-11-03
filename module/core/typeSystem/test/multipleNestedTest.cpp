#include "gtest/gtest.h"

#include "typeSystem/structMacros.hpp"
#include "typeSystem/qStructDef.hpp"
#include "typeSystem/fieldType/all.hpp"
#include "typeSystem/converter.hpp"
#include "typeSystem/runtimeTypeInformation.hpp"


GEN_QSTRUCT(Foo2)
{
    GEN_QSTRUCT_BODY();

    DynamicArray<float> nums {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(Foo2,{
    GEN_QSTRUCT_FIELD_ENTRY(Foo2,nums),
});


GEN_QSTRUCT(Bar2)
{
    GEN_QSTRUCT_BODY();

    DynamicArray<DynamicArray<Foo2>> arr {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_STATIC_DEF(Bar2,{
    GEN_QSTRUCT_FIELD_ENTRY(Bar2,arr),
});
TEST(ConverterTest2, Test)
{
    Bar2 bar {};
    bar.arr.push_back( DynamicArray<Foo2>() );
    bar.arr[0].push_back(Foo2());

    for(int i = 0 ; i != 18 ; i++)
    {
        bar.arr[0][0].nums.push_back(i*2);
    }

    RuntimeTypeInformation rti;
    Converter conv(rti);
    auto json = conv.qstructToJson(&bar,Bar2::staticDef.getWeak());
    auto source = json.stringify();

    Bar2 bar2 = conv.jsonToQStruct<Bar2>(json);

    for(int i = 0 ; i != 18 ; i++)
    {
        EXPECT_EQ(bar.arr[0][0].nums[i], i*2);
    }
}
