#include "gui/nativeLabel.hpp"
#include "gui/nativeWindow.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(NativeLabel,NativeWidget)
DynamicArray<OwnerPtr<Field>>
{
GEN_QOBJ_DEF_FIELD_ENTRY(NativeLabel,text),
},
DynamicArray<OwnerPtr<Method>>
{

}
END_GEN_QOBJ_STATIC_DEF()

NativeLabel::~NativeLabel()
{
    if(hwnd) DestroyWindow(hwnd);
}

void NativeLabel::setText(const std::string& in_string)
{
    text = in_string;
    if(hwnd)
        SetWindowText(hwnd, in_string.c_str());
}

bool NativeLabel::registerInWindow(WeakPtr<NativeWindow> in_window)
{
    hmenu = 0;
    hwnd = CreateWindowEx( 0, "EDIT", text.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT| ES_READONLY,
                           screenRect.x, screenRect.y, screenRect.w, screenRect.h,
                           in_window->getHwnd(), nullptr, in_window->getHInstance(), nullptr );
    return true;
}
