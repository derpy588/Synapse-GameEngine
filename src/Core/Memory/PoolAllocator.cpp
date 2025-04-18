#include "SynapseEngine/Core/Memory/PoolAllocator.h"

#include <cstdlib>

namespace Synapse {

    PoolAllocator::PoolAllocator(size_t objectSize, size_t objectCount) {
        size_t totalSize = objectSize * objectCount;
        memoryBlock = malloc(totalSize);

        freeList = nullptr;
        char* block = static_cast<char*>(memoryBlock);

        for (size_t i = 0; i < objectCount; i++) {
            uintptr_t memBlockAddr = reinterpret_cast<uintptr_t>(memoryBlock);
            void* ptr = reinterpret_cast<void*>(memBlockAddr + (i * objectSize));
            *static_cast<void**>(ptr) = freeList;
            freeList = ptr;
        }
    }

    PoolAllocator::~PoolAllocator() {
        free(memoryBlock);
    }

    void* PoolAllocator::allocate() {
        if (!freeList) return nullptr;

        std::lock_guard<std::mutex> lock(allocMutex);

        void* result = freeList;

        freeList = *static_cast<void**>(freeList);

        return result;
    }

    void PoolAllocator::deallocate(void* ptr) {
        std::lock_guard<std::mutex> lock(allocMutex);
        *static_cast<void**>(ptr) = freeList;

        freeList = ptr;
    }
}