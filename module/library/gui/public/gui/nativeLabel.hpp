#pragma once
#include "nativeWidget.hpp"
#include <windows.h>


GEN_QOBJ(NativeLabel,NativeWidget)
{
GEN_QOBJ_BODY(NativeWidget,NativeWidget);
GEN_QOBJ_DEF_CONSTRUCTOR(NativeLabel,NativeWidget);
public:
    void setText(const std::string& in_string);
    bool registerInWindow(WeakPtr<NativeWindow> in_window) override;

    ~NativeLabel() override;
protected:
    FIELD(serializable{instance},description{"display text"});
    std::string text;

};