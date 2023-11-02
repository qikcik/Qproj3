
#include "qstruct/field.hpp"
#include "qstruct/qStructDef.hpp"


WeakPtr<Field> QStructDef::getField(const std::string& in_name) const
{
    for(auto& fieldIt : fields)
        if(fieldIt->name == in_name) return fieldIt.getWeak();
    return {nullptr};
}

const DynamicArray<OwnerPtr<Field>>& QStructDef::getFields() const
{
    return fields;
}
