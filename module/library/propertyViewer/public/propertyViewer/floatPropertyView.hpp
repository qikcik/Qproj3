#pragma once
#include "shred/shred.hpp"
#include "gui/nativeWidget.hpp"
#include "gui/nativeLabel.hpp"
#include "gui/NativeEditbox.hpp"
#include "gui/tickable.hpp"

GEN_QOBJ(FloatPropertyView,Shred), public Tickable
{
GEN_QOBJ_BODY(FloatPropertyView,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(FloatPropertyView,Shred);
public:

    Vec2 getSize();
    void setPosition(Vec2 in_pos);

    void set(void* in_ptr, std::string name, WeakPtr<FloatField> in_type);
    void handleTick(float delta) override;
protected:
    WeakPtr<NativeLabel> label {};
    WeakPtr<NativeEditbox> edit {};

    Vec2 pos {};

    float* ptr {nullptr};
    bool blocked {false};
};