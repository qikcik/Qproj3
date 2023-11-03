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
            WeakPtr<QObjDef> in_baseObjDef,
            std::string in_name,
            size_t in_size,
            const std::function<void(WeakPtr<QObj> in_addr)>& in_constructInstance,
            const std::function<void(WeakPtr<QObj> in_addr)>& in_destructInstance,
            DynamicArray<OwnerPtr<Field>>&& in_fields,
            DynamicArray<OwnerPtr<Method>>&& in_methods
    ):
        baseObjDef(std::move(in_baseObjDef)),
        name(std::move(in_name)),
        memorySize(in_size),
        constructInstance(in_constructInstance),
        destructInstance(in_destructInstance),
        fields(std::move(in_fields)),
        methods(std::move(in_methods))
    {}

    ~QObjDef() = default;

    DynamicArray<WeakPtr<Field>> getFields() const;
    WeakPtr<Field> getField(const std::string& in_name) const;

    const WeakPtr<QObjDef> baseObjDef {};
    const std::string name;
    const size_t memorySize;

    const std::function<void(WeakPtr<QObj> in_addr)> constructInstance;
    const std::function<void(WeakPtr<QObj> in_addr)> destructInstance;

protected:
    DynamicArray<OwnerPtr<Field>> fields;
    DynamicArray<OwnerPtr<Method>> methods;
};