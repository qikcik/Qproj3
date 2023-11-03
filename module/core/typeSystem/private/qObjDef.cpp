#include "typeSystem/qObjDef.hpp"

DynamicArray<WeakPtr<Field>> QObjDef::getFields() const
{
    DynamicArray<WeakPtr<Field>> result;
    if(baseObjDef) result = baseObjDef->getFields();

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

