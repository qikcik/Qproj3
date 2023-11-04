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
        LOG_DBG("allocated block on addr:({})", PTR_TO_STR(block));

        return block;
    }

    inline static void remove_ref(PtrBlock* in_block) {
        if(!in_block) return;

        in_block->refCount--;
        if(in_block->refCount <= 0)
        {
            LOG_DBG("deallocating block on addr:({})", PTR_TO_STR(in_block));
            delete in_block;
        }
    }

    inline static PtrBlock* add_ref(PtrBlock* in_block) {
        if(in_block)
            in_block->refCount++;
        return in_block;
    }

    inline void allocate_memory(size_t in_memorySize) {
        if(ptr) deallocate_memory();
        ptr = malloc(in_memorySize);

        LOG_DBG("allocated on addr:({})", PTR_TO_STR(ptr));
    }

    inline void deallocate_memory() {
        ENSURE_OR_RETURN(ptr,);
        LOG_DBG("deallocating on addr:({})", PTR_TO_STR(ptr));

        free(ptr);
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