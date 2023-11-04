#pragma once

#include <concepts>
#include "runtimeTypeInformation.hpp"
#include "qStructDef.hpp"
#include <stack>


class Converter
{
public:
    explicit Converter(const RuntimeTypeInformation& in_rti) : rti(in_rti) {};

    json::Object qstructToJson(QStruct* in_addr, WeakPtr<QStructDef> in_class);

    void jsonToQStruct(const json::Object& json,WeakPtr<QStructDef> in_class, QStruct* in_addr);

    template<typename TType>
    TType jsonToQStruct(const json::Object& json) {
        LOG_WRN("inside",nullptr);
        TType obj;
        jsonToQStruct(json,TType::staticDef.getWeak(), &obj);
        LOG_WRN("outside",nullptr);
        return std::move(obj);
    }

protected:

    //void jsonArrayToQstruct(json::Array& arrayJson,const FieldType::DynamicArray& dynamicArray, void* addr);
    const RuntimeTypeInformation& rti;
};