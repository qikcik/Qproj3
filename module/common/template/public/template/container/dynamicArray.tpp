#pragma once
#include "dynamicArray.hpp"
#include "log/log.hpp"

// ATTENTION: keep in mind, that in this piece of code you have to manually call constructor, and destructors.

template<typename TType>
DynamicArray<TType>::DynamicArray()
{
}

template<typename TType>
DynamicArray<TType>::DynamicArray(const DynamicArray& other)
{
    reserve(other.capacity);
    for(auto& it : other)
        push_back(it);
}

template<typename TType>
DynamicArray<TType>::DynamicArray(DynamicArray&& other) noexcept
{
    array = other.array;
    capacity = other.capacity;
    length = other.length;

    other.array = nullptr;
    other.capacity = 0;
    other.length = 0;
}

template<typename TType>
template<typename... TTypeArg>
DynamicArray<TType>::DynamicArray(TTypeArg&&... args)
{
    reserve(sizeof...(args));
    ((push_back(std::forward<TTypeArg>(args))), ...);
}

template<typename TType>
DynamicArray<TType>::~DynamicArray()
{
    clear();
}

template<typename TType>
size_t DynamicArray<TType>::get_length() const
{
    return length;
}

template<typename TType>
size_t DynamicArray<TType>::get_capacity() const
{
    return capacity;
}

template<typename TType>
const DynamicArray<TType>::element_type&
DynamicArray<TType>::operator[](size_t in_position) const
{
    DBG_ENSURE(in_position < length);
    return array[in_position];
}

template<typename TType>
DynamicArray<TType>::element_type&
DynamicArray<TType>::operator[](size_t in_position)
{
    DBG_ENSURE(in_position < length);
    return array[in_position];
}

template<typename TType>
void DynamicArray<TType>::push_back(element_type in_element) noexcept
{
    if (length == capacity || capacity == 0)
        reserve(capacity + 8);

    if constexpr ( std::is_same_v<float, TType>) // for some reason float doesn't have default constructor, TODO: make this more generic
        array[length] = in_element;
    else
        new (array + length) TType( std::move(in_element) );
    length++;
}

template<typename TType>
void DynamicArray<TType>::reserve(size_t in_capacity) noexcept
{
    ENSURE_OR_RETURN(in_capacity > capacity,);

    capacity = in_capacity;
    auto newArr = allocate_memory(capacity,sizeof(TType));
    for(int idx = 0; idx != length; idx++)
        new (newArr + idx) TType(std::move(array[idx]));

    if(array) deallocate_memory(array);
    array = newArr;
}


template<typename TType>
void DynamicArray<TType>::clear() noexcept
{
    if(!array) return;
    for(auto& it : *this)
        it.~TType();

    deallocate_memory(array);
    array = nullptr;
    capacity = 0;
    length = 0;
}


template<typename TType>
TType* DynamicArray<TType>::allocate_memory(size_t in_capacity,size_t in_elementSize) noexcept
{
    auto* bytePtr = new uint8_t[in_capacity * in_elementSize];
    auto memberPtr = reinterpret_cast<TType*>(bytePtr);
    LOG_DBG("allocated on addr:({})", PTR_TO_STR(memberPtr));
    return memberPtr;
}

template<typename TType>
void DynamicArray<TType>::deallocate_memory(element_type* in_addr) noexcept
{
    DBG_ENSURE_OR_RETURN(in_addr,);
    LOG_DBG("deallocating on addr:({})", PTR_TO_STR(in_addr));

    delete[] (uint8_t*)in_addr;
}
