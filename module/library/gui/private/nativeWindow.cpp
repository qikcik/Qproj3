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
        hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, winClass.lpszClassName, self->id.name.c_str(), WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
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

void NativeWindow::refreshScroll()
{
    std::cout << scrolled.x << " " << scrolled.y << std::endl;

    // Y
    SCROLLINFO si = { sizeof(si) };
    si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;

    GetScrollInfo(hwnd, SB_VERT, &si);

    si.nPage = size.y;
    si.nMin = 0;
    si.nMax = occupiedScreenSize.y;

    if (scrolled.y < 0) scrolled.y = 0;
    if (scrolled.y > occupiedScreenSize.y) scrolled.y = occupiedScreenSize.y;

    ScrollWindow(hwnd, 0, (si.nPos - scrolled.y), NULL, NULL);
    si.nPos = scrolled.y;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    // X
    SCROLLINFO siX = { sizeof(siX) };
    siX.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;

    GetScrollInfo(hwnd, SB_HORZ, &siX);

    siX.nPage = size.x;
    siX.nMin = 0;
    siX.nMax = occupiedScreenSize.x;

    if (scrolled.x < 0) scrolled.x = 0;
    if (scrolled.x > occupiedScreenSize.x) scrolled.x = occupiedScreenSize.x;

    ScrollWindow(hwnd, (siX.nPos - scrolled.x), 0, NULL, NULL);
    siX.nPos = scrolled.x;
    SetScrollInfo(hwnd, SB_HORZ, &siX, TRUE);
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
                selfPtr->handleAppendWidget(widgetIt);
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

        case WM_VSCROLL:
        {
            auto action = LOWORD(wParam);
            HWND hScroll = (HWND)lParam;
            int newPosY = -1;
            if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
                newPosY = HIWORD(wParam);
            } else if (action == SB_LINEDOWN) {
                newPosY = selfPtr->scrolled.y + 30;
            } else if (action == SB_LINEUP) {
                newPosY = selfPtr->scrolled.y - 30;
            }
            if (newPosY == -1)
                break;

            selfPtr->scrolled.y = newPosY;
            selfPtr->refreshScroll();
            break;
        }

        case WM_HSCROLL:
        {
            auto action = LOWORD(wParam);
            HWND hScroll = (HWND)lParam;
            int newPosX = -1;
            if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
                newPosX = HIWORD(wParam);
            } else if (action == SB_LINEDOWN) {
                newPosX = selfPtr->scrolled.x + 30;
            } else if (action == SB_LINEUP) {
                newPosX = selfPtr->scrolled.x - 30;
            }
            if (newPosX == -1)
                break;

            selfPtr->scrolled.x = newPosX;
            selfPtr->refreshScroll();
            break;
        }

        case WM_COMMAND:
            for (auto& widgetIt: selfPtr->getChildsOfClass_singleDeep<NativeWidget>()) {
                if (widgetIt && widgetIt.getPtr()->getHwnd() == (HWND) lParam)
                    widgetIt.getPtr()->handleCommand();
            }
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int NativeWindow::getNewHmenuIdx()
{
    lastHMenuIdx++;
    return lastHMenuIdx;
}

void NativeWindow::handleAppendWidget(WeakPtr<NativeWidget> in_widget)
{
    if(!hwnd) return;
    in_widget.getPtr()->registerInWindow(selfPtr.unsafe_cast<NativeWindow>());

    occupiedScreenSize.x = 0;
    occupiedScreenSize.y = 0;
    for ( auto it: getChildsOfClass_singleDeep<NativeWidget>())
    {
        //LOG_ERR("{}, x {} y {}, w {} , h {}",it->getUniqueName(),it->getScreenRect().x,it->getScreenRect().y,it->getScreenRect().w,it->getScreenRect().h)
        if(it->getScreenRect().x+it->getScreenRect().w > occupiedScreenSize.x)
            occupiedScreenSize.x = it->getScreenRect().x+it->getScreenRect().w;
        if(it->getScreenRect().y+it->getScreenRect().h > occupiedScreenSize.y)
            occupiedScreenSize.y = it->getScreenRect().y+it->getScreenRect().h;
    }

    //LOG_ERR("ocupied screen: {} {}",occupiedScreenSize.x,occupiedScreenSize.y);
}

void NativeWindow::handleRemoveWidget(const WeakPtr<NativeWidget>& in_widget)
{
    occupiedScreenSize.x = 0;
    occupiedScreenSize.y = 0;
    for ( auto it: getChildsOfClass_singleDeep<NativeWidget>())
    {
        //LOG_ERR("{}, x {} y {}, w {} , h {}",it->getUniqueName(),it->getScreenRect().x,it->getScreenRect().y,it->getScreenRect().w,it->getScreenRect().h)
        if(it->getScreenRect().x+it->getScreenRect().w > occupiedScreenSize.x)
            occupiedScreenSize.x = it->getScreenRect().x+it->getScreenRect().w;
        if(it->getScreenRect().y+it->getScreenRect().h > occupiedScreenSize.y)
            occupiedScreenSize.y = it->getScreenRect().y+it->getScreenRect().h;
    }

    scrolled = {{},0,0};
    refreshScroll();
}

void NativeWindow::handleSubWidgetResize(const WeakPtr<NativeWidget>& in_widget)
{
    occupiedScreenSize.x = 0;
    occupiedScreenSize.y = 0;
    for ( auto it: getChildsOfClass_singleDeep<NativeWidget>())
    {
        //LOG_ERR("{}, x {} y {}",it->getUniqueName(),it->getScreenRect().x + it->getScreenRect().w,it->getScreenRect().y + it->getScreenRect().h)
        if(it->getScreenRect().x+it->getScreenRect().w > occupiedScreenSize.x)
            occupiedScreenSize.x = it->getScreenRect().x+it->getScreenRect().w;
        if(it->getScreenRect().y+it->getScreenRect().h > occupiedScreenSize.y)
            occupiedScreenSize.y = it->getScreenRect().y+it->getScreenRect().h;
    }
    //LOG_ERR("ocupied screen: {} {}",occupiedScreenSize.x,occupiedScreenSize.y);
}
