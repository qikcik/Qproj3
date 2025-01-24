#pragma once
#include "shred/shred.hpp"
#include <windows.h>

#include "types.hpp"


class NativeWidget;
GEN_QOBJ(NativeWindow,Shred)
{
GEN_QOBJ_BODY(NativeWindow,Shred);
GEN_QOBJ_DEF_CONSTRUCTOR(NativeWindow,Shred);
public:
    bool initializeNative(HINSTANCE in_hInstance, int nCmdShow);

    ~NativeWindow() override;

    HWND getHwnd()  {return hwnd;}
    HINSTANCE getHInstance()  {return hInstance;}
    int getNewHmenuIdx();

    bool isKeyPressed(uint8_t in_id) {return keys[in_id];};

    void handleAppendWidget(WeakPtr<NativeWidget> in_widget);
    void handleRemoveWidget(const WeakPtr<NativeWidget>& in_widget);
    void handleSubWidgetResize(const WeakPtr<NativeWidget>& in_widget);

protected:
    FIELDS_BEGIN(description{"win32 api internal"})
        WNDCLASSEX winClass {};
        HWND hwnd {};
        HINSTANCE hInstance {};
        int lastHMenuIdx {0};

        bool focused {false};
        bool keys[256] = {};

        Vec2 scrolled {};
        Vec2 size {};
        Vec2 occupiedScreenSize {};
    FIELDS_END()

    void refreshScroll();
    virtual bool postInitializeImpl() {return true;};
    virtual void onResizeImpl(int in_x, int in_y) { size = {{},in_x,in_y}; scrolled = {}; refreshScroll(); };

protected:
    static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static std::unordered_map<HWND,WeakPtr<NativeWindow>> registered_windows;
};

