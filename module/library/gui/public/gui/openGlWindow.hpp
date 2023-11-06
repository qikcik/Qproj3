#pragma once
#include "shred/shred.hpp"
#include "gui/nativeWindow.hpp"
#include "tickable.hpp"
#include <windows.h>

GEN_QOBJ(OpenGlWindow,NativeWindow), public Tickable
{
GEN_QOBJ_BODY(OpenGlWindow,NativeWindow);
GEN_QOBJ_DEF_CONSTRUCTOR(OpenGlWindow,NativeWindow);
public:
    void handleTick(float delta) override;

    std::function<void()> onInit {};
    std::function<void(float in_delta)> onUpdate {};

    ~OpenGlWindow() override;
protected:
    FIELDS_BEGIN(description{"win32 api internal"})
        HGLRC hRC {nullptr};
        HDC hDC {nullptr};
    FIELDS_END()

    bool postInitializeImpl() override;
    void onResizeImpl(int in_x, int in_y) override;

    void InitGL();
};