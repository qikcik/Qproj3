#include "typeSystem/fieldType/ownerObjPtrField.hpp"
#include "typeSystem/qObj.hpp"
#include "typeSystem/qObjDef.hpp"
#include "typeSystem/runtimeTypeInformation.hpp"

std::string OwnerObjPtrField::getTypeDesc() const {
    return "OwnerObjPtr<"+type->name+">";
}

//TODO: add checking!
json::innerType OwnerObjPtrField::toJson(void* in_addr) const
{
    json::Object result {};
    ENSURE_OR_RETURN(in_addr,result);

    auto asOwnerPtr = reinterpret_cast<OwnerPtr<QObj>*>(in_addr);
    auto asQObj = reinterpret_cast<QObj*>(asOwnerPtr->getPtr());
    ENSURE_OR_RETURN(asQObj,result);

    result.set("objDef",json::Value(asQObj->getObjDef()->name));

    for(const auto& fieldIt : type->getFields())
    {
        auto field_addr = fieldIt->getValuePtr<void>((QStruct*)in_addr);
        result.set(fieldIt->name,fieldIt->type->toJson(field_addr));
    }
    return result;
}

void OwnerObjPtrField::fromJson(void* in_addr, const json::innerType& in_json,LoadingContext& in_context) const
{
    auto json = std::get<json::Object>(in_json);

    auto& objDefStr = *json.get<json::Value>("objDef")->get<std::string>();

    //TODO MAGIC
    auto objDef = in_context.rti.getDynamicType(objDefStr);
    if(!objDef)
    {
        LOG_ERR("unknown type {}",objDefStr);
        return;
    }

    auto asOwnerPtr = reinterpret_cast<OwnerPtr<QObj>*>(in_addr);
    new (asOwnerPtr) OwnerPtr<QObj>();
    asOwnerPtr->unsafe_createInstance(objDef.getPtr()->memorySize,[objDef](auto in_weak){
        objDef->constructInstance(in_weak);
    });

    in_context.loadedObjects.push_back(asOwnerPtr->getWeak());

    for( auto& entryIt : json.entries)
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
