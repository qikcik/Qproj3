#pragma once
#include "weakPtr.hpp"

class WeakFromThis
{
public:
    virtual void inject_selfWeak(WeakPtr<void> in_selfPtr) = 0;
};