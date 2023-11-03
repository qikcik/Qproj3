#include "gtest/gtest.h"
#include "typeSystem/macros.hpp"
#include "typeSystem/qObj.hpp"
#include "typeSystem/qObjDef.hpp"

GEN_QOBJ(FooObj,QObj)
{
    GEN_QOBJ_BODY(FooObj,QObj);
    GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(FooObj, QObj);
public:
    WeakPtr<FooObj> getSelfPtr() {return  selfPtr.unsafe_cast<FooObj>();}
    float test {};
};

BEGIN_GEN_QOBJ_STATIC_DEF(FooObj,QObj)
    DynamicArray<OwnerPtr<Field>>
    {
        GEN_QOBJ_DEF_FIELD_ENTRY(FooObj,test)
    },
    DynamicArray<OwnerPtr<Method>>
    {

    }
END_GEN_QOBJ_STATIC_DEF()

TEST(Logger, BasicUsage)
{
    auto obj = OwnerPtr<FooObj>::CreateWithInstance(FooObj::staticDef.getWeak());
    obj->test = 2;
    EXPECT_EQ(obj->getSelfPtr()->test,2);
}