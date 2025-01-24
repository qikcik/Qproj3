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

void DynamicArrayUnsafeHelper::reserve(size_t in_capacity,
    std::function<void(void* in_fromAddr, void* in_toAddr)> in_moveElement)
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    uint8_t* newArray = ptr->allocate_memory(in_capacity,value_size);


    for(size_t el_idx=0; el_idx != get_length(); el_idx++)
    {
        void* el_oldAddr = get_elementPtr(el_idx);
        void* el_newAddr = (newArray + (el_idx * value_size));
        in_moveElement(el_oldAddr,el_newAddr);
    }
    ptr->deallocate_memory(ptr->array);
    ptr->capacity = in_capacity;
    ptr->array = newArray;

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
