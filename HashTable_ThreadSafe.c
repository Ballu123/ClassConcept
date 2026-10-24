/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: HashTable_ThreadSafe.c
 * Win10
 * Microsoft Visual Studio
 * The Hashtable
 */
========================================================================
//To make this highly relevant for an embedded or systems programming interview, 
//this design uses Open Addressing with Linear Probing rather than linked lists.

Why this design fits an Embedded Interview:

1. Zero Heap Overhead during operations: It allocates a contiguous block of memory upfront.
2. CPU Cache-Friendly: Linear probing keeps elements adjacent in memory, minimizing cache misses.
3. No Pointer Chasing: It avoids the pointer overhead and fragmentation caused by linked lists (Separate Chaining).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h> // Required for mutex primitives
======================================================================
#define TABLE_SIZE 16
#define DELETED_KEY -99999

typedef struct {
    int key;
    int value;
    bool is_occupied;
} HashEntry;

typedef struct {
    HashEntry entries[TABLE_SIZE];
    int size;
    pthread_mutex_t lock; // Object lock protecting table contents
} ThreadSafeHashTable;

int hash_function(int key) {
    int hash = key % TABLE_SIZE;
    if (hash < 0) hash += TABLE_SIZE;
    return hash;
}

// 1. Initialize the Table and its Mutex
bool ht_init(ThreadSafeHashTable* ht) {
    ht->size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->entries[i].key = 0;
        ht->entries[i].value = 0;
        ht->entries[i].is_occupied = false;
    }
    
    // Initialize the mutex. Returns 0 on success.
    if (pthread_mutex_init(&ht->lock, NULL) != 0) {
        printf("Mutex initialization failed!\n");
        return false;
    }
    return true;
}

// Destroy table components at shutdown
void ht_destroy(ThreadSafeHashTable* ht) {
    pthread_mutex_destroy(&ht->lock);
}

// 2. Thread-Safe Insert / Update
bool ht_insert(ThreadSafeHashTable* ht, int key, int value) {
    pthread_mutex_lock(&ht->lock); // Acquire exclusive lock

    if (ht->size >= TABLE_SIZE) {
        pthread_mutex_unlock(&ht->lock); // CRITICAL: Unlock before returning early!
        return false;
    }

    int index = hash_function(key);
    int original_index = index;

    while (ht->entries[index].is_occupied) {
        if (ht->entries[index].key == key) {
            ht->entries[index].value = value;
            pthread_mutex_unlock(&ht->lock); // Release lock
            return true;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == original_index) break;
    }

    ht->entries[index].key = key;
    ht->entries[index].value = value;
    ht->entries[index].is_occupied = true;
    ht->size++;

    pthread_mutex_unlock(&ht->lock); // Release lock
    return true;
}

// 3. Thread-Safe Search / Lookup
bool ht_search(ThreadSafeHashTable* ht, int key, int* out_value) {
    pthread_mutex_lock(&ht->lock); // Acquire lock

    int index = hash_function(key);
    int original_index = index;

    while (ht->entries[index].is_occupied || ht->entries[index].key == DELETED_KEY) {
        if (ht->entries[index].key == key) {
            *out_value = ht->entries[index].value;
            pthread_mutex_unlock(&ht->lock); // Release lock
            return true;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == original_index) break;
    }

    pthread_mutex_unlock(&ht->lock); // Release lock
    return false;
}

=================Bare Metal=========================================
Your interviewer says: "This is a bare-metal microcontroller with no OS or threads, but the main loop reads from the table
while an Interrupt (ISR) inserts into it. How do you make it safe?"

You cannot use a mutex here because an ISR cannot block or wait for a lock. 
Instead, you must protect the main loop operations by temporarily disabling interrupts using compiler-specific intrinsics (like ARM CMSIS primitives):

bool ht_search_bare_metal(HashTable* ht, int key, int* out_value) {
    bool found = false;

    // 1. Disable interrupts globally (Enter Critical Section)
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); 

    // 2. Perform the lookup operation safely without fear of preemption
    int index = hash_function(key);
    // ... (standard lookup loop) ...
    if (found_key) {
        *out_value = ht->entries[index].value;
        found = true;
    }

    // 3. Restore original interrupt state (Exit Critical Section)
    __set_PRIMASK(primask); 
    
    return found;
}


===================================================================
