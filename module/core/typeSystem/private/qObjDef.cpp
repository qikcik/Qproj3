#include "typeSystem/qObjDef.hpp"

DynamicArray<WeakPtr<Field>> QObjDef::getFields() const
{
    DynamicArray<WeakPtr<Field>> result;
    if(baseObjDef) result = baseObjDef->getFields();

    if(fields.get_length() != 0)
        result.reserve(fields.get_length()+result.get_length());
    for(auto& FieldIt : fields)
    {
        result.push_back(FieldIt.getWeak());
    }
    return result;
}

WeakPtr<Field> QObjDef::getField(const std::string& in_name) const
{
    for(auto& fieldIt : getFields())
        if(fieldIt->name == in_name) return fieldIt;
    return {nullptr};
}

bool QObjDef::isBaseOrSame(WeakPtr<QObjDef> in_other) const
{
    if(in_other.getPtr() == this) return true;
    if(baseObjDef)
        return baseObjDef->isBaseOrSame(std::move(in_other));
    return false;
}

