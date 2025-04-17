#include "SynapseEngine/Core/Memory/FreeListAllocator.h"

#include <cstdlib>
#include <new>

/*
Changes to Improve Allocator:
- More robust coalesce function
- thread safety with mutex
- Logging (once i implement the memory tracker)

Possible tests for this:
- Basic Alloc test
- Alignment tests
- Out of memory tests
- Fragmentation and Coalescing tests
- Reset test
- Stress test
*/

namespace Synapse {

    FreeListAllocator::FreeListAllocator(size_t totalSize) {
        memoryBlock = malloc(totalSize);

        if (!memoryBlock) throw std::bad_alloc();

        memorySize = totalSize;

        FreeBlock* initBlock = reinterpret_cast<FreeBlock*>(memoryBlock);
        initBlock->size = totalSize;
        initBlock->next = nullptr;

        freeList = initBlock;

    }

    FreeListAllocator::~FreeListAllocator() {
        free(memoryBlock);
    }

    void* FreeListAllocator::allocate(size_t size, size_t alignment) {
        // Check if size is at least the size of the freeblock struct
        size_t minSize = sizeof(FreeBlock);

        if (size < minSize) size = minSize;

        // Calculate the full size which is size of block + header
        size_t fullSize = size + sizeof(AllocationHeader);

        FreeBlock** current = &freeList;
        FreeBlock* block = freeList;

        while (block != nullptr) {

            uintptr_t blockAddr = reinterpret_cast<uintptr_t>(block);
            uintptr_t dataAddr = blockAddr + sizeof(AllocationHeader);
            uintptr_t alignedDataAddr = align(dataAddr, alignment);
            uintptr_t headerAddr = dataAddr - sizeof(AllocationHeader);

            size_t padding = alignedDataAddr - dataAddr;
            size_t reqSize = fullSize + padding;

            if (block->size >= reqSize) {

                *current = block->next;

                size_t remainingSize = block->size - reqSize;

                if (remainingSize > sizeof(FreeBlock)) {
                    FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(block) + reqSize);
                    newBlock->size = remainingSize;
                    newBlock->next = *current;
                    *current = newBlock;
                }

                AllocationHeader* header = reinterpret_cast<AllocationHeader*>(headerAddr);
                header->size = reqSize;
                header->adjustment = static_cast<uint8_t>(padding);

                return reinterpret_cast<void*>(alignedDataAddr);
            }

            current = &block->next;
            block = *current;

        }

        return nullptr;

    }

    void FreeListAllocator::deallocate(void* ptr) {
        if (!ptr) return;

        AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<char*>(ptr) - sizeof(AllocationHeader));

        uintptr_t ogAddr = reinterpret_cast<uintptr_t>(ptr) - sizeof(AllocationHeader) - header->adjustment;

        uintptr_t memoryBlockAddr = reinterpret_cast<uintptr_t>(memoryBlock);

        if (ogAddr < memoryBlockAddr || ogAddr >= memoryBlockAddr+memorySize) return;

        FreeBlock* block = reinterpret_cast<FreeBlock*>(ogAddr);

        block->size = header->size;

        FreeBlock** current = &freeList;

        while (*current && *current < block) {
            current = &(*current)->next;
        }

        block->next = *current;

        *current = block;

        coalesceBlocks();
    }

    void FreeListAllocator::reset() {
        FreeBlock* initBlock = reinterpret_cast<FreeBlock*>(memoryBlock);
        initBlock->size = memorySize;
        initBlock->next = nullptr;

        freeList = initBlock;
    }

    size_t FreeListAllocator::align(size_t address, size_t alignment) {
        return (address + alignment - 1) & ~(alignment - 1);
    }

    void FreeListAllocator::coalesceBlocks() {
        FreeBlock* current = freeList;

        while (current && current->next) {
            char* endAddr = reinterpret_cast<char*>(current) + current->size;

            if (endAddr == reinterpret_cast<char*>(current->next)) {
                current->size += current->next->size;
                current->next = current->next->next;
            } else {
                current = current->next;
            }
        }
    }

}