#pragma once
#include "template/memory/weakFromThis.hpp"
#include "template/memory/ownerPtr.hpp"

class QObjDef;
class QObj : public WeakFromThis
{
public:
    QObj(WeakPtr<QObj> in_self,WeakPtr<QObjDef> in_def) : selfPtr(std::move(in_self)), objDef(std::move(in_def)) {};
    virtual ~QObj() = default;

    virtual void postLoad() {}; // called post loading object, first parent then object from it field. deep first

    WeakPtr<QObjDef> getObjDef() const {return objDef;};

protected:
    WeakPtr<QObj> selfPtr {};
    WeakPtr<QObjDef> objDef {};
public:
    static const OwnerPtr<QObjDef> staticDef;
};
