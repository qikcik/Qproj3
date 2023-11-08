#include "gui/nativeEditbox.hpp"
#include "gui/nativeWindow.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(NativeEditbox,NativeWidget)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{
GEN_QOBJ_DEF_FIELD_ENTRY(NativeEditbox,text),
});
END_GEN_QOBJ_STATIC_DEF()

void NativeEditbox::setText(const std::string& in_string)
{
    text = in_string;
    if(hwnd) SetWindowText(hwnd, text.c_str());
}

const std::string& NativeEditbox::getText() const
{
    return text;
}

static WNDPROC OriginalEditCtrlProc = nullptr;
LRESULT CALLBACK FloatOnlyEditCtrlProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    if(uMsg == WM_CHAR)
    {
        // Make sure we only allow specific characters
        if(!    ((wParam >= '0' && wParam <= '9')
                 || wParam == '.'
                 || wParam == VK_RETURN
                 || wParam == VK_DELETE
                 || wParam == VK_BACK))
        {
            return 0;
        }
    }

    return CallWindowProc(OriginalEditCtrlProc, hwnd, uMsg, wParam, lParam);
}

bool NativeEditbox::registerInWindow(WeakPtr<NativeWindow> in_window)
{
    hmenu = (HMENU)in_window->getNewHmenuIdx();
    hwnd = CreateWindowEx( 0, "EDIT", text.c_str(), WS_BORDER | WS_CHILD | WS_VISIBLE,
                           screenRect.x, screenRect.y, screenRect.w, screenRect.h,
                           in_window->getHwnd(), hmenu, in_window->getHInstance(), nullptr );

    setType(type);

    return true;
}

NativeEditbox::~NativeEditbox()
{
    if(hwnd) DestroyWindow(hwnd);
}

void NativeEditbox::handleCommand()
{
    static char buf[4096];
    GetWindowText(hwnd, buf, 1024);
    std::string str = buf;
    if(str != text)
    {
        text = str;
        if(onChange) onChange(selfPtr.unsafe_cast<NativeEditbox>());
    }
}

void NativeEditbox::setType(NativeEditbox::Type in_type)
{
    if(!hwnd) return;

    if(type == Type::Float) {
        auto oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
                hwnd,
                GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(FloatOnlyEditCtrlProc)));
        if(OriginalEditCtrlProc == nullptr) OriginalEditCtrlProc = oldProc;
    }
    else if(OriginalEditCtrlProc != nullptr) {
        SetWindowLongPtr(hwnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(OriginalEditCtrlProc));
    }
}

