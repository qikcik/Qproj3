#pragma once
#include "shred/shred.hpp"
#include "gui/nativeWidget.hpp"
#include "gui/nativeLabel.hpp"
#include "gui/NativeEditbox.hpp"

GEN_QOBJ(DynamicArrayPropertyView,Shred)
{
GEN_QOBJ_BODY(DynamicArrayPropertyView,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(DynamicArrayPropertyView,Shred);
public:
    Vec2 getSize();
    void setPosition(Vec2 in_pos);

    std::function<void()> onNeedRecreation;
    void set(void* in_ptr, std::string name, WeakPtr<DynamicArrayField> in_type);
protected:

    Vec2 size {};
    Vec2 pos {};
};