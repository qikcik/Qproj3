#pragma once
#include "typeSystem/macros.hpp"
#include "typeSystem/qStructDef.hpp"
#include "typeSystem/structMacros.hpp"


GEN_QSTRUCT(ScreenRect)
{
    GEN_QSTRUCT_BODY(ScreenRect)
public:
    ScreenRect() = default;
    ScreenRect(int in_x, int in_y,int in_w,int in_h) : x(in_x),y(in_y),w(in_w),h(in_h)  {}

    FIELDS_BEGIN(serializable{instance})
        int x {0};
        int y {0};
        int w {0};
        int h {0};
    FIELDS_END()
};

GEN_QSTRUCT(Vec2)
{
    GEN_QSTRUCT_BODY(Vec2)
public:
    FIELDS_BEGIN(serializable{instance})
        int x {0};
        int y {0};
    FIELDS_END()
};