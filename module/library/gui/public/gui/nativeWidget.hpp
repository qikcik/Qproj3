#pragma once
#include "shred/shred.hpp"
#include <windows.h>

GEN_QSTRUCT(ScreenRect)
{
    GEN_QSTRUCT_BODY(ScreenRect)
public:
    FIELDS_BEGIN(serializable{instance})
        int x {0};
        int y {0};
        int w {0};
        int h {0};
    FIELDS_END()
};

class NativeWindow;
GEN_QOBJ(NativeWidget,Shred)
{
GEN_QOBJ_BODY(NativeWidget,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(NativeWidget,Shred);
public:
    virtual bool registerInWindow(WeakPtr<NativeWindow> in_window) { return false; };
    virtual void setScreenRect(ScreenRect in_rect) { screenRect = in_rect; };
protected:

    FIELD(serializable{instance},description{"cord of of widget on the widget"});
    ScreenRect screenRect {};

    FIELDS_BEGIN(description{"win32 api internal"})
        HMENU hmenu {};
        HWND hwnd {};
    FIELDS_END()

protected:

};