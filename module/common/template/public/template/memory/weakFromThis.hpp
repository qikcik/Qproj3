#pragma once
#include "weakPtr.hpp"

class WeakFromThis
{
    // required to have first argument in constructor as WeakPtr<void>
    //WeakFromThis(WeakPtr<void> in_self);
};