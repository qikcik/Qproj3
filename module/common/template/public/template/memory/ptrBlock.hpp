#pragma once
#include <memory>
#include <unordered_map>
#include "log/log.hpp"

// internal class used by weakPtr and ownerPtr
class PtrBlock
{
public:
    inline static PtrBlock* create_block() {
        auto block = new PtrBlock();
        block->refCount = 1;

        return block;
    }

    inline static void remove_ref(PtrBlock* in_block) {
        if(!in_block) return;

        in_block->refCount--;
        if(in_block->refCount <= 0)
            delete in_block;
    }

    inline static PtrBlock* add_ref(PtrBlock* in_block) {
        if(in_block)
            in_block->refCount++;
        return in_block;
    }

    inline void allocate_memory(size_t in_memorySize) {
        if(ptr) deallocate_memory();
        ptr = new uint8_t[in_memorySize];
    }

    inline void deallocate_memory() {
        ENSURE_OR_RETURN(ptr,);
        delete[] (uint8_t*)ptr;
        ptr = nullptr;
    }

    inline void* getPtr() {
        return ptr;
    }

protected:
    PtrBlock() = default;

protected:
    int refCount {0};
    void* ptr {nullptr};
};