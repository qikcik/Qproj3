#pragma once
#include <string>
#include <functional>

class QObj;
class Method
{
public:
    const std::string name {};
    const std::function<void(QObj* in_addr)> invoke;
};