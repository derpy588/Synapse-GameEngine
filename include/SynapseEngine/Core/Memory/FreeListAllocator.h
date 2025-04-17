#pragma once

#include <cstddef>
#include <mutex>

namespace Synapse {

    class FreeListAllocator {
        public:
            FreeListAllocator(size_t totalSize);
            ~FreeListAllocator();

            void* allocate(size_t size, size_t alignment = 8);
            void deallocate(void* ptr);

            void reset();

        private:
            struct AllocationHeader {
                size_t size;
                size_t adjustment;
            };

            struct FreeBlock {
                size_t size;
                FreeBlock* next;
            };

            size_t align(size_t address, size_t alignment);
            void coalesceBlocks();

        private:
            void* memoryBlock;
            size_t memorySize;

            FreeBlock* freeList;

            std::mutex allocMutex;

    };
}