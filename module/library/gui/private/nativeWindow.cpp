#include "gui/nativeWindow.hpp"
#include "../public/gui/nativeWindow.hpp"
#include "../public/gui/nativeWidget.hpp"


BEGIN_GEN_QOBJ_STATIC_DEF(NativeWindow,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

std::unordered_map<HWND,WeakPtr<NativeWindow>> NativeWindow::registered_windows{};

NativeWindow::~NativeWindow()
{
    if (hwnd && hInstance)
    {
        DestroyWindow(hwnd);
        registered_windows.erase(hwnd);
        UnregisterClass(getUniqueName().c_str(),hInstance);
    }
}

WeakPtr<NativeWindow> currentRegisteringWin {};
bool NativeWindow::initializeNative(HINSTANCE in_hInstance, int nCmdShow)
{
    auto self = selfPtr.unsafe_cast<NativeWindow>();

    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = 0;
    winClass.lpfnWndProc = NativeWindow::WndProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    winClass.lpszMenuName = nullptr;
    winClass.lpszClassName = getUniqueName().c_str();
    winClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    ENSURE_OR_RETURN(RegisterClassEx(&winClass),false);

    int width = 300;
    int height = 300;

    hInstance = in_hInstance;
    currentRegisteringWin = self;
        hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, winClass.lpszClassName, self->id.name.c_str(), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance,
                              nullptr);
    currentRegisteringWin = nullptr;

    ENSURE_OR_RETURN(hwnd,false);

    registered_windows[hwnd] = self;

    if(!postInitializeImpl())
        return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    SetFocus(hwnd);
    onResizeImpl(width, height);

    return true;
}

LRESULT CALLBACK NativeWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto selfPtr = registered_windows[hwnd];

    if (currentRegisteringWin) {
        selfPtr = currentRegisteringWin;
        selfPtr->hwnd = hwnd;
    }

    switch (msg) {
        case WM_CREATE:
            for (auto& widgetIt: selfPtr->getChildsOfClass_singleDeep<NativeWidget>()) {
                widgetIt.getPtr()->registerInWindow(selfPtr);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_ACTIVATE:
            selfPtr->focused = !HIWORD(wParam);
            break;

        case WM_SIZE:
            selfPtr->onResizeImpl(LOWORD(lParam),HIWORD(lParam));
            return 0;

        case WM_KEYDOWN:
            selfPtr->keys[wParam] = true;
            break;

        case WM_KEYUP:
            selfPtr->keys[wParam] = false;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            //for (auto& widgetIt: selfPtr->widgets) {
            //    if (widgetIt.getPtr()->hwnd == (HWND) lParam)
            //        widgetIt.getPtr()->receivedCommand();
            //}
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
