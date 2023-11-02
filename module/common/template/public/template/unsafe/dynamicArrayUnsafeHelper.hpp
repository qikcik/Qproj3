#pragma once
#include <memory>

//TODO: make UT
struct DynamicArrayUnsafeHelper
{
    void* fixedArrayPtr;
    const size_t value_size;

    size_t get_length();
    void set_length(size_t in_length);
    void reserve_withoutFreeingOldMemory(size_t in_capacity);

    void* get_elementPtr(size_t in_index);
};