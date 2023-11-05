#include "propertyViewer/qstructPropertyView.hpp"
#include "propertyViewer/stringPropertyView.hpp"
#include "propertyViewer/floatPropertyView.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(QStructPropertyView,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

void QStructPropertyView::set(QStruct* in_ptr, WeakPtr<QStructDef> in_def)
{
    //TODO: Assumed that is called only once
    Vec2 pos = {{},0,0};
    for(auto& fieldIt : in_def->getFields())
    {
        if(dynamic_cast<StdStringField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asQStruct = fieldIt->type.getWeak().unsafe_cast<QStructField>();

            //TODO:
        }

        if(dynamic_cast<StdStringField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asString = fieldIt->type.getWeak().unsafe_cast<StdStringField>();

            auto prop = appendChildren<StringPropertyView>(fieldIt->name);
            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asString );
            prop->setPosition(pos);
            pos = {{},0,pos.y+prop->getSize().y};
        }
        if(dynamic_cast<FloatField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asFloat = fieldIt->type.getWeak().unsafe_cast<FloatField>();

            auto prop = appendChildren<FloatPropertyView>(fieldIt->name);
            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asFloat );
            prop->setPosition(pos);
            pos = {{},0,pos.y+prop->getSize().y};
        }
    }
}
