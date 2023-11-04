#pragma once
#include "shred/shred.hpp"
#include <windows.h>



GEN_QOBJ(NativeController,Shred)
{
GEN_QOBJ_BODY(NativeController,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(NativeController,Shred);
public:
    void loop(HINSTANCE in_hInstance, int nCmdShow);
protected:
    FIELDS_BEGIN(description{"win32 api internal"})

    FIELDS_END()


protected:

};