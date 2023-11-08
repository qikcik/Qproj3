#pragma once
#include "nativeWidget.hpp"
#include <windows.h>
#include <functional>


GEN_QOBJ(NativeEditbox,NativeWidget)
{
GEN_QOBJ_BODY(NativeEditbox,NativeWidget);
GEN_QOBJ_DEF_CONSTRUCTOR(NativeEditbox,NativeWidget);
public:
    void setText(const std::string& in_string);
    const std::string& getText() const;

    enum class Type {
        String,Float
    };
    void setType(Type in_type);

    bool registerInWindow(WeakPtr<NativeWindow> in_window) override;
    void handleCommand() override;

    ~NativeEditbox() override;

    std::function<void(WeakPtr<NativeEditbox> inEditbox)> onChange {};
protected:
    Type type {Type::String};
    FIELD(serializable{instance},description{"display text"});
    std::string text;
};