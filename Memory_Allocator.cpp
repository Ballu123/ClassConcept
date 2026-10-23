/*
 * Name: Balram Panwar
 * Email ID: balram.panwar@gmail.com
 * Name of the file: Memory_Allocator.cpp
 * Win10
 * Microsoft Visual Studio
*/
===============================================================================
What is a Memory Allocator?
Problem:
  Programs need memory at runtime
  but don't know HOW MUCH or WHEN ahead of time

Solution:
  A memory allocator manages a large chunk of RAM
  and hands out pieces on request

  malloc(100) → give me 100 bytes
  free(ptr)   → take it back
The Big Picture
Your Program
    │
    │  malloc(64)
    ▼
┌─────────────────────────────────┐
│        Memory Allocator         │
│   (tracks free/used blocks)     │
└─────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────┐
│            HEAP                 │
│  [used][free][used][free][free] │
└─────────────────────────────────┘
    │
    ▼
  Physical RAM
Structure of a Memory Allocator
The Heap Block — Core Unit
Every allocated chunk has a HEADER before user data:

┌──────────────────────────────────────────┐
│              BLOCK HEADER                │
│  size     : how big is this block        │
│  is_free  : 0=used  1=free               │
│  next     : pointer to next block        │
│  prev     : pointer to prev block        │
│  magic    : 0xDEADBEEF (corruption check)│
├──────────────────────────────────────────┤
│                                          │
│           USER DATA                      │  ← malloc() returns
│           (size bytes)                   │     pointer HERE
│                                          │
├──────────────────────────────────────────┤
│           FOOTER (optional)              │
│  size     : copy of size (for coalescing)│
└──────────────────────────────────────────┘
Heap Layout in Memory
HEAP START
┌────────────┬──────────────────────────────┐
│  HEADER    │         DATA (32 bytes)       │  Block 1 (used)
├────────────┼──────────────────────────────┤
│  HEADER    │         DATA (64 bytes)       │  Block 2 (free)
├────────────┼──────────────────────────────┤
│  HEADER    │         DATA (16 bytes)       │  Block 3 (used)
├────────────┼──────────────────────────────┤
│  HEADER    │         DATA (128 bytes)      │  Block 4 (free)
└────────────┴──────────────────────────────┘
HEAP END

--------------------------------------------
Allocator Strategies Compared
┌──────────────┬────────────────────────────────────┐
│ Strategy     │ How it finds a free block           │
├──────────────┼────────────────────────────────────┤
│ First Fit    │ First block that is big enough      │
│              │ Fast but leaves small fragments     │
├──────────────┼────────────────────────────────────┤
│ Best Fit     │ Smallest block that fits            │
│              │ Less waste but slower to find       │
├──────────────┼────────────────────────────────────┤
│ Worst Fit    │ Largest block available             │
│              │ Leaves largest remainder            │
├──────────────┼────────────────────────────────────┤
│ Memory Pool  │ Any block from fixed-size pool      │
│              │ O(1), no fragmentation              │
└──────────────┴────────────────────────────────────┘
=================================================================================
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*─────────────────────────────────────────────
  Configuration
─────────────────────────────────────────────*/
#define HEAP_SIZE      1024        // total heap bytes
#define ALIGN          8           // 8-byte alignment
#define MAGIC_FREE     0xDEADBEEF  // detect corruption
#define MAGIC_USED     0xBEEFDEAD

// Round up to alignment boundary
#define ALIGN_UP(x)  (((x) + (ALIGN-1)) & ~(ALIGN-1))

/*─────────────────────────────────────────────
  Block Header Structure
─────────────────────────────────────────────*/
typedef struct Block {
    uint32_t     magic;      // corruption detection
    uint32_t     size;       // size of DATA (not header)
    bool         is_free;    // free or used?
    struct Block *next;      // next block in heap
    struct Block *prev;      // previous block in heap
} Block;

#define HEADER_SIZE  sizeof(Block)

/*─────────────────────────────────────────────
  Heap Storage
─────────────────────────────────────────────*/
static uint8_t heap[HEAP_SIZE];   // raw heap memory
static Block  *heap_start = NULL; // first block

/*─────────────────────────────────────────────
  Init — set up entire heap as one free block
─────────────────────────────────────────────*/
void mem_init(void) {
    heap_start = (Block *)heap;

    heap_start->magic   = MAGIC_FREE;
    heap_start->size    = HEAP_SIZE - HEADER_SIZE;
    heap_start->is_free = true;
    heap_start->next    = NULL;
    heap_start->prev    = NULL;

    printf("[INIT] Heap: %d bytes, Header: %zu bytes\n",
           HEAP_SIZE, HEADER_SIZE);
    printf("[INIT] Usable: %zu bytes\n",
           HEAP_SIZE - HEADER_SIZE);
}

/*─────────────────────────────────────────────
  Split a block into two
  [ BLOCK (size)       ] →
  [ BLOCK (need) ][ BLOCK (rest) ]
─────────────────────────────────────────────*/
static void split_block(Block *b, uint32_t need) {
    // Only split if remainder is large enough
    if (b->size <= need + HEADER_SIZE + ALIGN)
        return;

    // Create new free block after the used portion
    Block *new = (Block *)((uint8_t *)b
                          + HEADER_SIZE + need);
    new->magic   = MAGIC_FREE;
    new->size    = b->size - need - HEADER_SIZE;
    new->is_free = true;
    new->next    = b->next;
    new->prev    = b;

    if (b->next)
        b->next->prev = new;

    b->next = new;
    b->size = need;
}

/*─────────────────────────────────────────────
  Coalesce — merge adjacent free blocks
  [ FREE ][ FREE ] → [ FREE (merged) ]
─────────────────────────────────────────────*/
static void coalesce(Block *b) {
    // Merge with NEXT block if free
    if (b->next && b->next->is_free) {
        b->size += HEADER_SIZE + b->next->size;
        b->next  = b->next->next;
        if (b->next)
            b->next->prev = b;
    }

    // Merge with PREVIOUS block if free
    if (b->prev && b->prev->is_free) {
        b->prev->size += HEADER_SIZE + b->size;
        b->prev->next  = b->next;
        if (b->next)
            b->next->prev = b->prev;
    }
}

/*─────────────────────────────────────────────
  malloc — First Fit Strategy
─────────────────────────────────────────────*/
void *mem_alloc(uint32_t size) {
    if (size == 0) return NULL;

    // Round up to alignment
    size = ALIGN_UP(size);

    // Walk the block list — find first free block
    Block *curr = heap_start;
    while (curr) {
        if (curr->magic != MAGIC_FREE &&
            curr->magic != MAGIC_USED) {
            printf("[ERROR] Heap corruption detected!\n");
            return NULL;
        }

        if (curr->is_free && curr->size >= size) {
            // Found a fit — split if too large
            split_block(curr, size);

            curr->is_free = false;
            curr->magic   = MAGIC_USED;

            printf("[ALLOC] %u bytes at %p\n",
                   size, (void*)(curr + 1));

            // Return pointer AFTER header
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }

    printf("[ALLOC] FAILED — out of memory!\n");
    return NULL;
}

/*─────────────────────────────────────────────
  free — release memory back to heap
─────────────────────────────────────────────*/
void mem_free(void *ptr) {
    if (!ptr) return;

    // Get block header (sits just BEFORE user data)
    Block *b = (Block *)ptr - 1;

    // Validate magic number
    if (b->magic != MAGIC_USED) {
        printf("[ERROR] Double-free or invalid pointer!\n");
        return;
    }

    b->is_free = true;
    b->magic   = MAGIC_FREE;

    printf("[FREE] %u bytes at %p\n", b->size, ptr);

    // Merge adjacent free blocks
    coalesce(b);
}

/*─────────────────────────────────────────────
  Heap visualizer
─────────────────────────────────────────────*/
void mem_dump(void) {
    printf("\n── Heap Dump ──────────────────────\n");
    Block *curr = heap_start;
    int    idx  = 0;
    uint32_t used = 0, free = 0;

    while (curr) {
        printf("  Block[%d] | %s | size=%4u | addr=%p\n",
               idx,
               curr->is_free ? "FREE" : "USED",
               curr->size,
               (void *)(curr + 1));

        if (curr->is_free) free += curr->size;
        else               used += curr->size;

        curr = curr->next;
        idx++;
    }
    printf("  Total used: %u | Total free: %u\n",
           used, free);
    printf("───────────────────────────────────\n\n");
}
