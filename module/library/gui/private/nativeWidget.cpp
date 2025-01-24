#include "gui/nativeWidget.hpp"
#include "gui/nativeWindow.hpp"
#include "gui/types.hpp"


BEGIN_GEN_QOBJ_STATIC_DEF(NativeWidget, Shred)
    selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{
GEN_QOBJ_DEF_FIELD_ENTRY(NativeWidget,screenRect),
});
END_GEN_QOBJ_STATIC_DEF()

void NativeWidget::postAppended()
{
    auto window = getNearestParentOfClass<NativeWindow>();
    if(window) window->handleAppendWidget(selfPtr.unsafe_cast<NativeWidget>());
}

void NativeWidget::preRemove()
{
    DestroyWindow(hwnd);

    auto window = getNearestParentOfClass<NativeWindow>();
    if(window) window->handleRemoveWidget(selfPtr.unsafe_cast<NativeWidget>());
}

void NativeWidget::setScreenRect(ScreenRect in_rect)
{
    screenRect = in_rect;
    if(hwnd)
        SetWindowPos(hwnd, nullptr,screenRect.x,screenRect.y,screenRect.w,screenRect.h,0);

    auto window = getNearestParentOfClass<NativeWindow>();
    if(window) window->handleSubWidgetResize(selfPtr.unsafe_cast<NativeWidget>());
}

ScreenRect NativeWidget::getScreenRect()
{
    RECT Rect;
    GetWindowRect(hwnd, &Rect);
    MapWindowPoints(HWND_DESKTOP,GetParent(hwnd), (LPPOINT) &Rect, 2);
    return ScreenRect{
        static_cast<int>(Rect.top),
        static_cast<int>(Rect.left),
        static_cast<int>(Rect.right-Rect.left),
        static_cast<int>(Rect.bottom-Rect.top)
    };
}
