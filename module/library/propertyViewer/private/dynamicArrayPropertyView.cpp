#include "propertyViewer/dynamicArrayPropertyView.hpp"
#include "propertyViewer/stringPropertyView.hpp"
#include "propertyViewer/floatPropertyView.hpp"
#include "gui/nativeGroupbox.hpp"
#include "template/unsafe/dynamicArrayUnsafeHelper.hpp"
#include "gui/nativeButton.hpp"
#include "propertyViewer/qstructPropertyView.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(DynamicArrayPropertyView,Shred)
            selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
                                                {

                                                });
END_GEN_QOBJ_STATIC_DEF()

void DynamicArrayPropertyView::set(void* in_ptr, std::string name, WeakPtr<DynamicArrayField> in_type)
{
    //TODO: Assumed that is called only once
    Vec2 nextPos = pos;
    nextPos.x += 5;
    nextPos.y += 25;
    int maxX {200};

    auto* valueTypePtr = in_type->valueType.unsafe_getPtr();
    DynamicArrayUnsafeHelper helper(in_ptr,in_type->valueType->getMemorySize());

    for(size_t el_idx=0; el_idx != helper.get_length(); el_idx++)
    {
        void* el_addr = helper.get_elementPtr(el_idx);

        if(dynamic_cast<FloatField*>(valueTypePtr))
        {
            auto asFloat = in_type->valueType.getWeak().unsafe_cast<FloatField>();

            auto prop = appendChildren<FloatPropertyView>("element");
            prop->setPosition(nextPos);
            prop->set( el_addr, std::to_string(el_idx), asFloat );
            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
            if(prop->getSize().x > maxX) maxX = prop->getSize().x;
        }

        if(dynamic_cast<QStructField*>(valueTypePtr))
        {
            auto asQStruct = in_type->valueType.getWeak().unsafe_cast<QStructField>();
            auto qstruct = appendChildren<QStructPropertyView>("element");
            qstruct->setPosition(nextPos);
            qstruct->set( std::to_string(el_idx), (QStruct*)el_addr, asQStruct->type );
            nextPos = {{}, nextPos.x, nextPos.y + qstruct->getSize().y};
            if(qstruct->getSize().x > maxX) maxX = qstruct->getSize().x;
        }
    }

//    for(auto& fieldIt : in_def->getFields())
//    {
////        if(dynamic_cast<QStructField*>(fieldIt->type.unsafe_getPtr()))
////        {
////            auto asQStruct = fieldIt->type.getWeak().unsafe_cast<QStructField>();
////            auto qstruct = appendChildren<QStructPropertyView>(fieldIt->name);
////            qstruct->setPosition(nextPos);
////            qstruct->set( fieldIt->name, fieldIt->getValuePtr<QStruct>(in_ptr), asQStruct->type );
////            nextPos = {{}, nextPos.x, nextPos.y + qstruct->getSize().y};
////            if(qstruct->getSize().x > maxX) maxX = qstruct->getSize().x;
////        }
////
////        if(dynamic_cast<StdStringField*>(fieldIt->type.unsafe_getPtr()))
////        {
////            auto asString = fieldIt->type.getWeak().unsafe_cast<StdStringField>();
////
////            auto prop = appendChildren<StringPropertyView>(fieldIt->name);
////            prop->setPosition(nextPos);
////            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asString );
////            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
////            if(prop->getSize().x > maxX) maxX = prop->getSize().x;
////        }
////        if(dynamic_cast<FloatField*>(fieldIt->type.unsafe_getPtr()))
////        {
////            auto asFloat = fieldIt->type.getWeak().unsafe_cast<FloatField>();
////
////            auto prop = appendChildren<FloatPropertyView>(fieldIt->name);
////            prop->setPosition(nextPos);
////            prop->set( fieldIt->getValuePtr<void>(in_ptr), fieldIt->name, asFloat );
////            nextPos = {{}, nextPos.x, nextPos.y + prop->getSize().y};
////            if(prop->getSize().x > maxX) maxX = prop->getSize().x;
////        }
//    }

    nextPos = {{}, nextPos.x, nextPos.y + 5};
    auto addBtn = appendChildren<NativeButton>("add_btn");
    addBtn->setText("add new");
    addBtn->setScreenRect({{},nextPos.x,nextPos.y,maxX,20});
    nextPos = {{}, nextPos.x, nextPos.y + 20};

    auto selfWeak = selfPtr.unsafe_cast<DynamicArrayPropertyView>();
    addBtn->onClick = [selfWeak,in_type,in_ptr](){
        auto* valueTypePtr = in_type->valueType.unsafe_getPtr();
        DynamicArrayUnsafeHelper helper(in_ptr,in_type->valueType->getMemorySize());

        helper.get_length();
        LOG_INF("{}/{}",helper.get_length(),helper.get_capacity());
        if(helper.get_capacity() > helper.get_length()+1)
        {
            auto idx = helper.get_length();
            helper.set_length(helper.get_length()+1);
            auto ptr = helper.get_elementPtr(idx);
            *(float*)ptr = 0;

            if(dynamic_cast<QStructField*>(valueTypePtr)) {
                auto asQStruct = in_type->valueType.getWeak().unsafe_cast<QStructField>();
                asQStruct->type->constructInstance((QStruct*)ptr);
            }

            if(selfWeak->onNeedRecreation) selfWeak->onNeedRecreation();
        }
    };

    auto group = appendChildren<NativeGroupbox>("groupbox");
    group->setText(name + " [" + in_type->getTypeDesc() +"]");
    group->setScreenRect({{},pos.x,pos.y,maxX+10,nextPos.y-pos.y+10});

    size = {{}, maxX+15, nextPos.y-pos.y+10};

}

Vec2 DynamicArrayPropertyView::getSize()
{
    return size;
}

void DynamicArrayPropertyView::setPosition(Vec2 in_pos)
{
    pos = in_pos;
}