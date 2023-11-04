#pragma once
#include "memory"
#include <functional>

#include "field.hpp"
#include "template/container/dynamicArray.tpp" //TODO: Split to cpp and hpp file

struct QStruct {};

class QStructDef //TODO: refactor to use function initialer
{
public:
    QStructDef(
            std::string in_name,
            size_t in_size,
            const std::function<void(QStruct* in_addr)>& in_constructInstance,
            const std::function<void(QStruct* in_addr)>& in_destructInstance,
            DynamicArray<OwnerPtr<Field>>&& in_fields
        ):
            name(std::move(in_name)),
            memorySize(in_size),
            constructInstance(in_constructInstance),
            destructInstance(in_destructInstance),
            fields(std::move(in_fields))
        {}

    ~QStructDef() = default;

    const DynamicArray<OwnerPtr<Field>>& getFields() const;
    WeakPtr<Field> getField(const std::string& in_name) const;

    const std::string name;
    const size_t memorySize;

    const std::function<void(QStruct* in_addr)> constructInstance;
    const std::function<void(QStruct* in_addr)> destructInstance;

protected:
    DynamicArray<OwnerPtr<Field>> fields;
};