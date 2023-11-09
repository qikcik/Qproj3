#include "gui/nativeButton.hpp"
#include "gui/nativeWindow.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(NativeButton,NativeWidget)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{
    GEN_QOBJ_DEF_FIELD_ENTRY(NativeButton,text),
});
END_GEN_QOBJ_STATIC_DEF()

NativeButton::~NativeButton()
{
    if(hwnd) DestroyWindow(hwnd);
}

void NativeButton::setText(const std::string& in_string)
{
    text = in_string;
    if(hwnd)
        SetWindowText(hwnd, in_string.c_str());
}

bool NativeButton::registerInWindow(WeakPtr<NativeWindow> in_window)
{
    hmenu = (HMENU)in_window->getNewHmenuIdx();
    hwnd = CreateWindowEx( 0, "BUTTON", text.c_str(), WS_CHILD | WS_VISIBLE,
                           screenRect.x, screenRect.y, screenRect.w, screenRect.h,
                           in_window->getHwnd(), hmenu, in_window->getHInstance(), nullptr );
    return true;
}

void NativeButton::handleCommand()
{
    if(onClick) onClick();
}
