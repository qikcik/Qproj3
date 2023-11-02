#pragma once
#include "qstruct/fieldType.hpp"

class FloatField : public FieldType
{
public:
    ~FloatField() override = default;

    std::string getTypeDesc() const override {
        return "Float";
    }

    size_t getMemorySize() const override {
        return sizeof(float);
    };

    json::innerType toJson(void* in_addr) const  override {
        auto& value = *reinterpret_cast<float*>(in_addr);
        return json::Value(value);
    }

    void fromJson(void* in_addr,const json::innerType& in_json) const override {
        auto asFloat = std::get<json::Value>(in_json).get<float>();
        *static_cast<float*>(in_addr) = *asFloat;
    }
};