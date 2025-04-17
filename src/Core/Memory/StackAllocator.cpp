#include "SynapseEngine/Core/Memory/StackAllocator.h"

#include <cstdlib>

namespace Synapse {
    StackAllocator::StackAllocator(size_t size) {
        memoryBlock = malloc(size);
        memorySize = size;
        currentOffset = 0;
    }

    StackAllocator::~StackAllocator() {
        free(memoryBlock);
    }

    void* StackAllocator::allocate(size_t size, size_t alignment) {
        size_t alignedOffset = align(currentOffset, alignment);

        if (alignedOffset + size > memorySize) {
            return nullptr;
        }

        currentOffset = alignedOffset + size;
        return static_cast<char*>(memoryBlock) + alignedOffset;
    }

    void StackAllocator::reset() {
        currentOffset = 0;
    }

    size_t StackAllocator::align(size_t address, size_t alignment) {
        return (address + alignment - 1) & ~(alignment - 1);
    }
}