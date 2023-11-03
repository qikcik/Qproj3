#include "typeSystem/qObj.hpp"
#include "typeSystem/qObjDef.hpp"
#include "typeSystem/field.hpp"
#include "typeSystem/method.hpp"
#include "template/container/dynamicArray.hpp"

const OwnerPtr<QObjDef> QObj::staticDef = std::move(OwnerPtr<QObjDef>::CreateWithInstance(
        WeakPtr<QObjDef>(),
        "QObj",
        sizeof(QObj),
        [](WeakPtr<QObj> in_addr){ new (in_addr.getPtr()) QObj(in_addr,QObj::staticDef.getWeak()); },
        [](WeakPtr<QObj> in_addr){ reinterpret_cast<QObj*>(in_addr.getPtr())->~QObj(); },
        DynamicArray<OwnerPtr<Field>>{},
        DynamicArray<OwnerPtr<Method>>{}
));