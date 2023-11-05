#pragma once
#include "shred/shred.hpp"

GEN_QOBJ(QStructPropertyView,Shred)
{
GEN_QOBJ_BODY(QStructPropertyView,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(QStructPropertyView,Shred);
public:
    void set(QStruct* in_ptr, WeakPtr<QStructDef> in_def);
};