#pragma once
#include "template/macros.hpp"
#include <memory>
#include <concepts>

template<typename TType>
class DynamicArray
{
public:
    using element_type = TType;

    DynamicArray();
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    GEN_COPY_ASSIGMENT_OPERATOR(DynamicArray)
    GEN_MOVE_ASSIGMENT_OPERATOR(DynamicArray)

    template<typename... TTypeArg>
    explicit DynamicArray(TTypeArg&&... args);

    inline size_t get_length() const;
    inline size_t get_capacity() const;

    void reserve(size_t in_capacity) noexcept;
    void push_back(element_type in_element) noexcept;
    void clear() noexcept;

    element_type& operator[](size_t in_position);
    const element_type& operator[](size_t in_position) const;

    class Iterator;
    Iterator begin() const { return Iterator(array); }
    Iterator end()   const { return Iterator(array+length); }

protected:
    element_type* allocate_memory(size_t in_capacity,size_t in_elementSize = sizeof(TType)) noexcept;
    void deallocate_memory(element_type* in_addr) noexcept;

    size_t length = 0;
    size_t capacity = 0;
    element_type* array = nullptr;

    friend class DynamicArrayUnsafeHelper;
};

template<typename TType>
struct DynamicArray<TType>::Iterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = TType;
    using pointer           = TType*;
    using reference         = TType&;

    Iterator(pointer in) : ptr(in) {}

    reference operator*() const { return *ptr; }
    pointer operator->() { return ptr; }
    Iterator& operator++() { ptr++; return *this; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };
private:
    pointer ptr;
};