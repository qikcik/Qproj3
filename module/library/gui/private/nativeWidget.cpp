#include "gui/nativeWidget.hpp"
#include "gui/nativeWindow.hpp"

GEN_QSTRUCT_STATIC_DEF(ScreenRect,{
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,x),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,y),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,w),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,h),
});

GEN_QSTRUCT_STATIC_DEF(Vec2,{
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,x),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,y),
});


BEGIN_GEN_QOBJ_STATIC_DEF(NativeWidget,Shred)
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

void NativeWidget::setScreenRect(ScreenRect in_rect)
{
    screenRect = in_rect;
    if(hwnd)
        SetWindowPos(hwnd, nullptr,screenRect.x,screenRect.y,screenRect.w,screenRect.h,0);
}
