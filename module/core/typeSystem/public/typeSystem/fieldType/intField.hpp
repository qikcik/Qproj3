#pragma once
#include "typeSystem/fieldType.hpp"

class IntField : public FieldType
{
public:
    ~IntField() override = default;

    std::string getTypeDesc() const override {
        return "Int";
    }

    size_t getMemorySize() const override {
        return sizeof(int);
    };

    json::innerType toJson(void* in_addr) const  override {
        auto& value = *reinterpret_cast<int*>(in_addr);
        return json::Value((float)value);
    }

    void fromJson(void* in_addr,const json::innerType& in_json,LoadingContext& in_context) const override {
        auto asFloat = *std::get<json::Value>(in_json).get<float>();
        *static_cast<int*>(in_addr) = (int)asFloat;
    }
};
