/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Memory_pool_C.c
 * Win10
 * Microsoft Visual Studio
 * Thread-Safe Memory Pool using Semaphore + Mutex:
 * A thread-safe memory pool uses a mutex to protect the free list and a semaphore to track available blocks, ensuring constant-time, 
 * deterministic, and race-free allocation.
 *🔹When to use 
 * Producer–Consumer systems
 * Blocking allocation (wait if pool empty)
 * RTOS-friendly design
 *🔹Design
 * Counting semaphore → number of free blocks
 * Mutex → protect free list structure
 * Prevents overflow/underflow automatically
 */

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

#define POOL_SIZE   10
#define BLOCK_SIZE  32

typedef struct FreeBlock {
    struct FreeBlock *next;
} FreeBlock;

static uint8_t memory_pool[POOL_SIZE][BLOCK_SIZE];
static FreeBlock *free_list = NULL;

static pthread_mutex_t pool_mutex;
static sem_t free_blocks;

/* Initialize pool
pool_init() initializes a memory pool by linking all fixed-size blocks into a singly linked free list,
allowing constant-time allocation and deallocation without heap fragmentation.
*/
void pool_init(void) {
    for (int i = 0; i < POOL_SIZE - 1; i++) {
        //Because each block points to the next block
        ((FreeBlock *)memory_pool[i])->next =
            (FreeBlock *)memory_pool[i + 1];
    }
    //The last block will be handled separately
    ((FreeBlock *)memory_pool[POOL_SIZE - 1])->next = NULL;

    free_list = (FreeBlock *)memory_pool[0];

    pthread_mutex_init(&pool_mutex, NULL);
    sem_init(&free_blocks, 0, POOL_SIZE); // count = available blocks
}

/* Allocate (blocks if empty) */
void *pool_alloc(void) {
    sem_wait(&free_blocks);   // wait until block available

    pthread_mutex_lock(&pool_mutex);

    void *block = free_list;
    free_list = free_list->next;

    pthread_mutex_unlock(&pool_mutex);
    return block;
}

/* Free memory
When freeing a block, the allocator inserts it at the head of the free list by storing the old head 
pointer inside the freed block and updating the free-list head, achieving constant-time deallocation.
*/
void pool_free(void *ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&pool_mutex);

    ((FreeBlock *)ptr)->next = free_list;
    free_list = (FreeBlock *)ptr;

    pthread_mutex_unlock(&pool_mutex);
    sem_post(&free_blocks);   // signal availability
}

/* Example usage */
int main(void) {
    pool_init();

    void *p1 = pool_alloc();
    void *p2 = pool_alloc();

    printf("Allocated: %p %p\n", p1, p2);

    pool_free(p1);
    pool_free(p2);

    printf("Memory returned to pool\n");
    return 0;
}
