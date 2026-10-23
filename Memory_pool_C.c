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
===============================================================================
General allocator:  variable-size blocks → fragmentation
Memory Pool:        fixed size blocks   → zero fragmentation,  O(1) alloc and free

Pool of 32-byte blocks:
┌────┬────┬────┬────┬────┬────┬────┬────┐
│ 32 │ 32 │ 32 │ 32 │ 32 │ 32 │ 32 │ 32 │
│FREE│USED│FREE│FREE│USED│FREE│FREE│USED│
└────┴────┴────┴────┴────┴────┴────┴────┘
         ↑              ↑
      allocated      allocated

Free list: FREE→FREE→FREE→FREE→FREE→NULL
--------------------------------------------------------------
Free List — How It Works Inside
BEFORE any allocation:

pool memory:
[block0][block1][block2][block3]...

free_list → [block3] → [block2] → [block1] → [block0] → NULL

Each FREE block stores the "next" pointer
INSIDE its own memory (no extra overhead!)


AFTER pool_alloc() called:

free_list → [block2] → [block1] → [block0] → NULL

block3 returned to user — its memory now holds user data


AFTER pool_free(block3):

free_list → [block3] → [block2] → [block1] → [block0] → NULL

block3 back on list — its memory now holds next pointer again

=====================================================================================================
-------------------------------1stway-------------------------------------------------------------------
/*─────────────────────────────────────────────
  Memory Pool — Fixed Size Block Allocator
  Zero fragmentation, O(1) alloc and free
─────────────────────────────────────────────*/
#define POOL_BLOCK_SIZE   32    // each block = 32 bytes
#define POOL_BLOCK_COUNT  16    // 16 blocks total

// Free list node — stored INSIDE the free block itself
typedef struct FreeNode {
    struct FreeNode *next;
} FreeNode;

typedef struct {
    uint8_t   pool[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
    FreeNode *free_list;        // head of free list
    uint32_t  free_count;       // blocks available
} MemPool;

/*──── Pool Init ───────────────────────────*/
void pool_init(MemPool *mp) {
    mp->free_list  = NULL;
    mp->free_count = POOL_BLOCK_COUNT;

    // Link all blocks into free list
    // Free list pointer lives INSIDE the free block
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        FreeNode *node = (FreeNode *)
            (mp->pool + i * POOL_BLOCK_SIZE);
        node->next    = mp->free_list;
        mp->free_list = node;
    }

    printf("[POOL] Init: %d blocks x %d bytes = %d bytes\n",
           POOL_BLOCK_COUNT,
           POOL_BLOCK_SIZE,
           POOL_BLOCK_SIZE * POOL_BLOCK_COUNT);
}

/*──── Pool Alloc — O(1) ───────────────────*/
void *pool_alloc(MemPool *mp) {
    if (!mp->free_list) {
        printf("[POOL] Out of blocks!\n");
        return NULL;
    }

    // Pop from free list head
    FreeNode *node = mp->free_list;
    mp->free_list  = node->next;
    mp->free_count--;

    printf("[POOL] Alloc block @ %p (free=%u)\n",
           (void*)node, mp->free_count);

    memset(node, 0, POOL_BLOCK_SIZE);  // clear block
    return (void *)node;
}

/*──── Pool Free — O(1) ────────────────────*/
void pool_free(MemPool *mp, void *ptr) {
    if (!ptr) return;

    // Push back onto free list head
    FreeNode *node = (FreeNode *)ptr;
    node->next     = mp->free_list;
    mp->free_list  = node;
    mp->free_count++;

    printf("[POOL] Free block @ %p (free=%u)\n",
           ptr, mp->free_count);
}

/*──── Pool Status ─────────────────────────*/
void pool_dump(MemPool *mp) {
    printf("\n── Pool Status ─────────────────────\n");
    printf("  Free blocks : %u / %u\n",
           mp->free_count, POOL_BLOCK_COUNT);
    printf("  Used blocks : %u / %u\n",
           POOL_BLOCK_COUNT - mp->free_count,
           POOL_BLOCK_COUNT);
    printf("────────────────────────────────────\n\n");
}
----------------------------------------------------------------------------------------------
---------------------------2nd way-------------------------------------------------------------
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
    //The last block will be handled separately. The last block has no next block:This marks the end of the free list.
    ((FreeBlock *)memory_pool[POOL_SIZE - 1])->next = NULL;
    
    //The first free block is block 0. Allocation will start from here
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
