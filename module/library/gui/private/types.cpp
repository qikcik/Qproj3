#include "gui/types.hpp"
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
