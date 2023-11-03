#include "typeSystem/converter.hpp"
#include "typeSystem/fieldType/qStructField.hpp"
#include "typeSystem/qObj.hpp"

json::Object Converter::qstructToJson(QStruct* in_addr, WeakPtr<QStructDef> in_class)
{
    QStructField field(in_class);
    return std::get<json::Object>(field.toJson(in_addr));
}


void Converter::jsonToQStruct(const json::Object& json,WeakPtr<QStructDef> in_class, QStruct* in_addr)
{
    in_class.getPtr()->destructInstance(in_addr);

    LoadingContext context{rti};
    QStructField field(in_class);
    field.fromJson(in_addr,json,context);

    for(auto it : context.loadedObjects)
    {
        it->postLoad();
    }

}