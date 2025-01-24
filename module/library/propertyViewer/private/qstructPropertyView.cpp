#include "propertyViewer/qstructPropertyView.hpp"
#include "propertyViewer/stringPropertyView.hpp"
#include "propertyViewer/floatPropertyView.hpp"
#include "gui/nativeGroupbox.hpp"
#include "propertyViewer/dynamicArrayPropertyView.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(QStructPropertyView,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

void QStructPropertyView::set(const std::string& in_name, QStruct* in_ptr, WeakPtr<QStructDef> in_def)
{
    name = in_name;
    ptr = in_ptr;
    def = in_def;
    //TODO: Assumed that is called only once
    Vec2 nextPos = pos;
    nextPos.x += 5;
    nextPos.y += 25;

    int maxX {0};
    for(auto& fieldIt : in_def->getFields())
    {
        LOG_ERR("Constructing {}",fieldIt->name);
        if(dynamic_cast<QStructField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asQStruct = fieldIt->type.getWeak().unsafe_cast<QStructField>();
            auto qstruct = appendChildren<QStructPropertyView>(fieldIt->name);
            qstruct->setPosition(nextPos);
            qstruct->set( fieldIt->name, fieldIt->getValuePtr<QStruct>(in_ptr), asQStruct->type );
            nextPos = {{}, nextPos.x, nextPos.y + qstruct->getSize().y};
            if(qstruct->getSize().x > maxX) maxX = qstruct->getSize().x;
        }

        if(dynamic_cast<StdStringField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asString = fieldIt->type.getWeak().unsafe_cast<StdStringField>();

            auto prop = appendChildren<StringPropertyView>(fieldIt->name);
            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asString );
            prop->setPosition(nextPos);
            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
            if(prop->getSize().x > maxX) maxX = prop->getSize().x;
        }
        if(dynamic_cast<FloatField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asFloat = fieldIt->type.getWeak().unsafe_cast<FloatField>();

            auto prop = appendChildren<FloatPropertyView>(fieldIt->name);
            prop->setPosition(nextPos);
            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asFloat );
            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
            if(prop->getSize().x > maxX) maxX = prop->getSize().x;
        }
        if(dynamic_cast<DynamicArrayField*>(fieldIt->type.unsafe_getPtr()))
        {
            auto asDynamicArrayField = fieldIt->type.getWeak().unsafe_cast<DynamicArrayField>();

            auto prop = appendChildren<DynamicArrayPropertyView>(fieldIt->name);
            prop->setPosition(nextPos);
            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asDynamicArrayField );
            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
            if(prop->getSize().x > maxX) maxX = prop->getSize().x;

            prop->onNeedRecreation = [&](){
                recreate();
            };
        }
    }

    auto group = appendChildren<NativeGroupbox>("groupbox");
    group->setText(name + " [" + in_def->name+"]");
    group->setScreenRect({pos.x,pos.y,maxX+10,nextPos.y-pos.y+10});

    size = {{}, maxX+15, nextPos.y-pos.y+10};
}

Vec2 QStructPropertyView::getSize()
{
    return size;
}

void QStructPropertyView::setPosition(Vec2 in_pos)
{
    pos = in_pos;
}

void QStructPropertyView::recreate()
{
    for(auto& it : children)
    {
        it->preRemove();
    }
    children.clear();
    set(name, ptr, def);
}
