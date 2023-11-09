#include "template/unsafe/dynamicArrayUnsafeHelper.hpp"
#include "template/container/dynamicArray.tpp"

size_t DynamicArrayUnsafeHelper::get_length()
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    return ptr->get_length();
}


void* DynamicArrayUnsafeHelper::get_elementPtr(size_t in)
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    auto* arr = ptr->array;

    return arr + (in * value_size);
}

void DynamicArrayUnsafeHelper::reserve_withoutFreeingOldMemory(size_t in_capacity)
{
    new (fixedArrayPtr) DynamicArray<uint8_t>();
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);

    ptr->array = ptr->allocate_memory(in_capacity,value_size);
    ptr->capacity = in_capacity;
    ptr->length = 0;
}

void DynamicArrayUnsafeHelper::set_length(size_t in_length)
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    ptr->length = in_length;
}

size_t DynamicArrayUnsafeHelper::get_capacity()
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    return ptr->get_capacity();
}
