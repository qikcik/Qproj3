#include "gui/nativeWidget.hpp"

GEN_QSTRUCT_STATIC_DEF(ScreenRect,{
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,x),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,y),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,w),
    GEN_QSTRUCT_FIELD_ENTRY(ScreenRect,h),
});


BEGIN_GEN_QOBJ_STATIC_DEF(NativeWidget,Shred)
DynamicArray<OwnerPtr<Field>>
{
GEN_QOBJ_DEF_FIELD_ENTRY(NativeWidget,screenRect),
},
DynamicArray<OwnerPtr<Method>>
{

}
END_GEN_QOBJ_STATIC_DEF()