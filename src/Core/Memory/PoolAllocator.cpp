#include "SynapseEngine/Core/Memory/PoolAllocator.h"

#include <cstdlib>

namespace Synapse {

    PoolAllocator::PoolAllocator(size_t objectSize, size_t objectCount) {
        size_t totalSize = objectSize * objectCount;
        memoryBlock = malloc(totalSize);

        freeList = nullptr;
        char* block = static_cast<char*>(memoryBlock);

        for (size_t i = 0; i < objectCount; i++) {
            void* ptr = memoryBlock + (i * objectSize);
            *static_cast<void**>(ptr) = freeList;
            freeList = ptr;
        }
    }

    PoolAllocator::~PoolAllocator() {
        free(memoryBlock);
    }

    void* PoolAllocator::allocate() {
        if (!freeList) return nullptr;

        void* result = freeList;

        freeList = *static_cast<void**>(freeList);

        return result;
    }

    void PoolAllocator::deallocate(void* ptr) {
        *static_cast<void**>(ptr) = freeList;

        freeList = ptr;
    }
}