#pragma once

#include "typeSystem/fieldType.hpp"

class StdStringField : public FieldType
{
public:
    ~StdStringField() override = default;

    std::string getTypeDesc() const override {
        return "StdString";
    }

    size_t getMemorySize() const override {
        return sizeof(std::string);
    };

    json::innerType toJson(void* in_addr) const  override {
        auto& value = *reinterpret_cast<std::string*>(in_addr);
        return json::Value(value);
    }

    void fromJson(void* in_addr,const json::innerType& in_json,LoadingContext& in_context) const override {
        auto asString = std::get<json::Value>(in_json).get<std::string>();
        new (in_addr) std::string(*asString);
    }
};