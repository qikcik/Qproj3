#include "typeSystem/fieldType/qStructField.hpp"
#include "typeSystem/qStructDef.hpp"
#include "log/log.hpp"

size_t QStructField::getMemorySize() const {
    DBG_ENSURE(type);
    return type->memorySize;
}
//TODO: improve this code, mainly safety
json::innerType QStructField::toJson(void* in_addr) const
{
    json::Object result {};

    for(const auto& fieldIt : type->getFields())
    {
        auto field_addr = fieldIt->getValuePtr<void>((QStruct*)in_addr);
        result.set(fieldIt->name,fieldIt->type->toJson(field_addr));
    }

    return result;
}
//TODO: improve this code, mainly safety
void QStructField::fromJson(void* in_addr, const json::innerType& in_json,LoadingContext& in_context) const
{
    type->constructInstance(reinterpret_cast<QStruct*>(in_addr));
    auto json = std::get<json::Object>(in_json);
    for( auto& entryIt : json.entries )
    {
        auto field = type->getField(entryIt.first);
        auto entryIt_addr = field->getValuePtr<void>(in_addr);
        if(!field) {
            LOG_WRN("tried to set not existed property:({})",entryIt.first)
            continue;
        }
        field->type->fromJson(entryIt_addr,entryIt.second,in_context);
    }
}

std::string QStructField::getTypeDesc() const
{
    return "QStruct:"+type->name;
};
