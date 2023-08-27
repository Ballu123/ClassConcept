/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Memory_pool.cpp
 * Details
 * A memory pool, also known as a buffer pool, is a preallocated pool of memory chunks that can be used and reused 
 * for efficient memory management in situations where memory allocations and deallocations are frequent. 
 * In this example, the MemoryPool class allocates and manages memory blocks of a fixed size (blockSize) from the system. 
 * The allocate function retrieves a memory block from the pool, and the deallocate function returns the memory block to the pool.
 */

#include <iostream>
#include <vector>

class MemoryPool {
private:
    std::vector<void*> memoryBlocks;
    size_t blockSize;
    size_t poolSize;

public:
    MemoryPool(size_t block_size, size_t num_blocks) : blockSize(block_size), poolSize(block_size * num_blocks) {
        memoryBlocks.reserve(num_blocks);
        for (size_t i = 0; i < num_blocks; ++i) {
            void* block = std::malloc(block_size);
            memoryBlocks.push_back(block);
        }
    }

    ~MemoryPool() {
        for (void* block : memoryBlocks) {
            std::free(block);
        }
    }

    void* allocate() {
        if (!memoryBlocks.empty()) {
            void* block = memoryBlocks.back();
            memoryBlocks.pop_back();
            return block;
        }
        return nullptr; // No available blocks
    }

    void deallocate(void* block) {
        memoryBlocks.push_back(block);
    }
};

int main() {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    MemoryPool pool(block_size, num_blocks);

    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();
    
    if (ptr1) {
        std::cout << "Allocated block 1" << std::endl;
    }

    if (ptr2) {
        std::cout << "Allocated block 2" << std::endl;
    }

    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    return 0;
}
