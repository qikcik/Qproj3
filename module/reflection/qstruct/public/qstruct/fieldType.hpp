#pragma once
#include <memory>
#include "json/object.hpp"
#include "template/memory/ownerPtr.hpp"

class FieldType
{
public:
    virtual ~FieldType() = default;

    virtual std::string getTypeDesc() const = 0;
    virtual size_t getMemorySize() const = 0;
    virtual json::innerType toJson(void* in_addr) const = 0;
    virtual void fromJson(void* in_addr,const json::innerType& in_json) const = 0;
};

template<typename T>
OwnerPtr<FieldType> getFieldType();

