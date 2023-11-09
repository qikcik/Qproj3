#pragma once
#include "nativeWidget.hpp"
#include <windows.h>


GEN_QOBJ(NativeButton,NativeWidget)
{
GEN_QOBJ_BODY(NativeButton,NativeWidget);
GEN_QOBJ_DEF_CONSTRUCTOR(NativeButton,NativeWidget);
public:
    void setText(const std::string& in_string);
    bool registerInWindow(WeakPtr<NativeWindow> in_window) override;
    void handleCommand() override;

    ~NativeButton() override;

    std::function<void()> onClick {};
protected:
    FIELD(serializable{instance},description{"display text"});
    std::string text;
};