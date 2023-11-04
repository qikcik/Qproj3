#include "typeSystem/qObj.hpp"
#include "typeSystem/qObjDef.hpp"
#include "typeSystem/field.hpp"
#include "typeSystem/method.hpp"
#include "template/container/dynamicArray.hpp"

const OwnerPtr<QObjDef> QObj::staticDef = std::move(OwnerPtr<QObjDef>::CreateWithInstance(
        "QObj",
        sizeof(QObj),
        [](WeakPtr<QObj> in_addr){ new (in_addr.getPtr()) QObj(in_addr,QObj::staticDef.getWeak()); },
        [](WeakPtr<QObj> in_addr){ reinterpret_cast<QObj*>(in_addr.getPtr())->~QObj(); },
        [](){
            QObj::staticDef.unsafe_getPtr()->baseObjDef = std::move(WeakPtr<QObjDef>());
        }
));