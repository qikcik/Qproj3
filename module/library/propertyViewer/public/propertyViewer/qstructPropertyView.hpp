#pragma once
#include "shred/shred.hpp"
#include "gui/nativeWidget.hpp"

GEN_QOBJ(QStructPropertyView,Shred)
{
GEN_QOBJ_BODY(QStructPropertyView,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(QStructPropertyView,Shred);
public:
    Vec2 getSize();
    void setPosition(Vec2 in_pos);

    void set(const std::string& in_name, QStruct* in_ptr, WeakPtr<QStructDef> in_def);
    void recreate();
protected:
    Vec2 size {};
    Vec2 pos {};

    std::string name {""};
    QStruct* ptr {};
    WeakPtr<QStructDef> def {};
};