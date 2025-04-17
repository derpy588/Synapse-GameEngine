#pragma once

#include <cstddef>
#include <mutex>

namespace Synapse {
    class StackAllocator {
    public:
        StackAllocator(size_t size);
        ~StackAllocator();

        void* allocate(size_t size, size_t alignment = 8);
        void reset();

    private:
        size_t align(size_t address, size_t alignment);

    private:
        void* memoryBlock;
        size_t memorySize;
        size_t currentOffset;

        std::mutex allocMutex;
    };
}