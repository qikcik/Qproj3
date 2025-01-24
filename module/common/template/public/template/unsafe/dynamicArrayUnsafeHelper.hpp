#pragma once
#include <functional>
#include <memory>

//TODO: make UT
struct DynamicArrayUnsafeHelper
{
    void* fixedArrayPtr;
    const size_t value_size;

    size_t get_length();
    size_t get_capacity();
    void set_length(size_t in_length);

    void reserve_withoutFreeingOldMemory(size_t in_capacity);
    void reserve(size_t in_capacity, std::function<void(void* in_fromAddr, void* in_toAddr)> in_moveElement);

    void* get_elementPtr(size_t in_index);
};