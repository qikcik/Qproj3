#pragma once
#include "ptrBlock.hpp"
#include "template/macros.hpp"

template<typename TType>
class WeakPtr
{
public:
    using element_type = TType;

    WeakPtr() = default;
    ~WeakPtr() {
        PtrBlock::remove_ref(block);
    };

    WeakPtr(const WeakPtr& other) {
        block = PtrBlock::add_ref(other.block);
    };

    template<std::derived_from<TType> TOther> // down-casting
    WeakPtr(const WeakPtr<TOther>& other) {
        block = PtrBlock::add_ref(other.block);
    };
    GEN_COPY_ASSIGMENT_OPERATOR(WeakPtr)

    WeakPtr(WeakPtr&& other) noexcept {
        DBG_ENSURE_OR_RETURN(this != &other,);

        block = other.block;
        other.block = nullptr;
    };

    template<std::derived_from<TType> TOther> // down-casting
    WeakPtr(WeakPtr<TOther>&& other)  noexcept {
        if(this == reinterpret_cast<WeakPtr<TType>*>(&other)) return;

        block = other.block;
        other.block = nullptr;
    };
    GEN_MOVE_ASSIGMENT_OPERATOR(WeakPtr)

    operator bool() const
    {
        return block and block->getPtr();
    }

    TType* operator->() {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };

    const TType* operator->() const {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };


    WeakPtr(PtrBlock* in_block) {
        block = PtrBlock::add_ref(in_block);
    };


    template<typename TOther>
    WeakPtr<TOther> unsafe_cast() {
        return  WeakPtr<TOther>(block);
    };


protected:
    PtrBlock* block {nullptr};

    template<typename TOther> // for casting purpose
    friend class WeakPtr;
};