#include "gui/nativeWindow.hpp"
#include "gui/nativeGroupbox.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(NativeGroupbox,NativeWidget)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{
    GEN_QOBJ_DEF_FIELD_ENTRY(NativeGroupbox,text),
});
END_GEN_QOBJ_STATIC_DEF()

NativeGroupbox::~NativeGroupbox()
{
    if(hwnd) DestroyWindow(hwnd);
}

void NativeGroupbox::setText(const std::string& in_string)
{
    text = in_string;
    if(hwnd)
        SetWindowText(hwnd, in_string.c_str());
}

bool NativeGroupbox::registerInWindow(WeakPtr<NativeWindow> in_window)
{
    hmenu = 0;
    hwnd = CreateWindowEx( 0, "BUTTON", text.c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                           screenRect.x, screenRect.y, screenRect.w, screenRect.h,
                           in_window->getHwnd(), nullptr, in_window->getHInstance(), nullptr );
    return true;
}
