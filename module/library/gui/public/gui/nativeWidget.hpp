#pragma once
#include "shred/shred.hpp"
#include <windows.h>

#include "types.hpp"


class NativeWindow;
GEN_QOBJ(NativeWidget,Shred)
{
GEN_QOBJ_BODY(NativeWidget,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(NativeWidget,Shred);
public:

    virtual bool registerInWindow(WeakPtr<NativeWindow> in_window) { return false; };
    virtual void setScreenRect(ScreenRect in_rect);
    virtual ScreenRect getScreenRect();

    virtual void handleCommand() {};
    HWND getHwnd()  {return hwnd;};

    void postAppended() override;
    void preRemove() override;


protected:
    FIELD(serializable{instance},description{"cord of widget on the widget"});
    ScreenRect screenRect {};

    FIELDS_BEGIN(description{"win32 api internal"})
        HMENU hmenu {};
        HWND hwnd {};
    FIELDS_END()

protected:

};