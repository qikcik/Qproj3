#include "gui/nativeWindow.hpp"
#include "../public/gui/nativeWindow.hpp"
#include "../public/gui/nativeWidget.hpp"


BEGIN_GEN_QOBJ_STATIC_DEF(NativeWindow,Shred)
    DynamicArray<OwnerPtr<Field>>
    {

    },
    DynamicArray<OwnerPtr<Method>>
    {

    }
END_GEN_QOBJ_STATIC_DEF()

std::unordered_map<HWND,WeakPtr<NativeWindow>> NativeWindow::registered_windows{};

NativeWindow::~NativeWindow()
{
    if (hwnd)
    {
        DestroyWindow(hwnd);
        registered_windows.erase(hwnd);
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

    hInstance = in_hInstance;
    currentRegisteringWin = self;
        hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, winClass.lpszClassName, self->id.name.c_str(), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, nullptr, nullptr, hInstance,
                              nullptr);
    currentRegisteringWin = nullptr;

    ENSURE_OR_RETURN(hwnd,false);

    registered_windows[hwnd] = self;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

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
