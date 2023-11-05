#pragma once
#include "shred/shred.hpp"
#include "gui/nativeWidget.hpp"
#include "gui/nativeLabel.hpp"
#include "gui/NativeEditbox.hpp"

GEN_QOBJ(FloatPropertyView,Shred)
{
GEN_QOBJ_BODY(FloatPropertyView,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(FloatPropertyView,Shred);
public:

    Vec2 getSize();
    void setPosition(Vec2 in_pos);

    void set(void* in_ptr, std::string name, WeakPtr<FloatField> in_type);
protected:
    WeakPtr<NativeLabel> label {};
    WeakPtr<NativeEditbox> edit {};
};