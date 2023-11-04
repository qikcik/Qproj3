#pragma once
#include "template/macros.hpp"
#include "ptrBlock.hpp"
#include "weakPtr.hpp"
#include "weakFromThis.hpp"
#include <functional>

//similar to shared_ptr, but allow only to have one instance that keep object alive
template<typename TType>
class OwnerPtr
{
public:
    using element_type = TType;


    template <typename... TArgs>
    static OwnerPtr<TType> CreateWithInstance(TArgs&&... args)  {
        OwnerPtr<TType> result{};
        result.createInstance(std::forward<TArgs>(args)...);
        return std::move(result);
    };

    template <typename... TArgs>
    void createInstance(TArgs&&... args)  {
        block = PtrBlock::create_block();
        block->allocate_memory(sizeof(TType));

        if constexpr(std::is_polymorphic<TType>::value and std::is_base_of<WeakFromThis,TType>::value)
        {
            auto weakFromThis = static_cast<WeakFromThis*>((TType*) block->getPtr());
            new(block->getPtr()) TType(WeakPtr<TType>(block), std::forward<TArgs>(args)...);
        }
        else
        {
            new (block->getPtr()) TType(std::forward<TArgs>(args)...);
        }
    };

    void unsafe_createInstance(size_t in_size, std::function<void(WeakPtr<TType>)> in_construct)
    {
        block = PtrBlock::create_block();
        block->allocate_memory(in_size);
        in_construct(WeakPtr<TType>(block));
    }

    explicit OwnerPtr() : block() {}

    ~OwnerPtr() {
        if(block && block->getPtr()) {
            reinterpret_cast<TType*>(block->getPtr())->~TType();
            block->deallocate_memory();
        }
        PtrBlock::remove_ref(block);
    };

    OwnerPtr(const OwnerPtr& other) = delete;
    OwnerPtr& operator=(OwnerPtr& other) = delete;

    OwnerPtr(const OwnerPtr&& other) noexcept {
        DBG_ENSURE_OR_RETURN(this != &other,);

        block = other.block;
        other.block = nullptr;
    };


    template<typename TOther> // down-cast
    requires std::derived_from<TOther,TType>
    OwnerPtr(OwnerPtr<TOther>&& other) {
        DBG_ENSURE_OR_RETURN((void*)this != (void*)&other,);

        block = other.block;
        other.block = nullptr;
    };
    GEN_MOVE_ASSIGMENT_OPERATOR(OwnerPtr)


    WeakPtr<TType> getWeak() const {
        return WeakPtr<TType>(block);
    }

    operator bool() const
    {
        return block and block->getPtr();
    }

    TType* getPtr() {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };

    TType* unsafe_getPtr() const {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };

    TType* operator->() {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };

    const TType* operator->() const {
        if(!block) return nullptr;
        return reinterpret_cast<TType*>(block->getPtr());
    };


protected:
    mutable PtrBlock* block;

    template<typename TOther> // for casting purpose
    friend class OwnerPtr;
};