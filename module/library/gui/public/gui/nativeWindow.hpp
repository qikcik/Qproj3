#pragma once
#include "shred/shred.hpp"
#include <windows.h>

GEN_QOBJ(NativeWindow,Shred)
{
GEN_QOBJ_BODY(NativeWindow,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR(NativeWindow,Shred);
public:
    bool initializeNative(HINSTANCE in_hInstance, int nCmdShow);

    ~NativeWindow() override;

    HWND getHwnd() {return hwnd;};
    HINSTANCE getHInstance() {return hInstance;};
protected:
    FIELDS_BEGIN(description{"win32 api internal"})
        WNDCLASSEX winClass {};
        HWND hwnd {};
        HINSTANCE hInstance {};
        int lastHMENUIdx = 0;
    FIELDS_END()

protected:
    static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static std::unordered_map<HWND,WeakPtr<NativeWindow>> registered_windows;
};