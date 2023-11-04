#pragma once
#include <functional>

#include "template/container/dynamicArray.tpp" //TODO: Split to cpp and hpp file
#include "typeSystem/field.hpp"
#include "method.hpp"

class QObj;

class QObjDef
{
public:
    QObjDef(
            std::string in_name,
            size_t in_size,
            const std::function<void(WeakPtr<QObj> in_addr)>& in_constructInstance,
            const std::function<void(WeakPtr<QObj> in_addr)>& in_destructInstance,
            const std::function<void()>& in_initializeQObjDef
    )
    :
        name(std::move(in_name)),
        memorySize(in_size),
        initializeQObjDef(in_initializeQObjDef),
        constructInstance(in_constructInstance),
        destructInstance(in_destructInstance)
    {}

    ~QObjDef() = default;

    DynamicArray<WeakPtr<Field>> getFields() const;
    WeakPtr<Field> getField(const std::string& in_name) const;
    bool isBaseOrSame(WeakPtr<QObjDef> in_other) const;

    WeakPtr<QObjDef> baseObjDef {};
    const std::string name;
    const size_t memorySize;

    const std::function<void()> initializeQObjDef;
    const std::function<void(WeakPtr<QObj> in_addr)> constructInstance;
    const std::function<void(WeakPtr<QObj> in_addr)> destructInstance;

    DynamicArray<OwnerPtr<Field>> fields;
    DynamicArray<OwnerPtr<Method>> methods;
};