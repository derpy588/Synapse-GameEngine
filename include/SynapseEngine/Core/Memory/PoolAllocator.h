#pragma once

#include <cstddef>
#include <mutex>

namespace Synapse {

    class PoolAllocator {
        public:
            PoolAllocator(size_t objectSize, size_t objectCount);
            ~PoolAllocator();

            void* allocate();
            void deallocate(void* ptr);

        private:
            void* memoryBlock;
            void* freeList;

            std::mutex allocMutex;
    };
}