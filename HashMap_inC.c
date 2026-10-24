
/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: HashMap_inC.c
 * Win10
 * Microsoft Visual Studio
 * The HashMap class implements a basic hash map using separate chaining for collision resolution. It supports insertion, retrieval, 
 * and removal of key-value pairs. The hash function is used to determine the bucket index for each key. Each bucket is a linked list that stores pairs of keys and values.
 */
============================================================================
To keep it as easy to read and understand as possible, this version uses Separate Chaining (a basic linked list for each bucket) to handle collisions. 
It does not contain any complex thread-locking or optimization math.
When you add elements, they drop straight into their corresponding modulo index. When a collision occurs (like key 1 and key 11 both mapping to index 1), 
the newest node attaches directly to the front of that specific list bucket:

HashMap Table Array
 ┌───────────┐
 │ Bucket 0  ├─► NULL
 ├───────────┤
 │ Bucket 1  ├─► [ Key: 11 | Val: 1100 ] ──► [ Key: 1 | Val: 150 ] ──► NULL
 ├───────────┤
 │ Bucket 2  ├─► NULL
 ├───────────┤
 │ Bucket 5  ├─► [ Key: 5  | Val: 500  ] ──► NULL
 └───────────┘

Delete
Here is the code for the map_delete function, along with a complete clean-up function to free the entire structure at shutdown.
When deleting from a linked list bucket, you have to track the previous node in the chain. 
This allows you to stitch the "before" and "after" nodes together smoothly when the target node is removed.
  
When you drop key 11 out of the chain, the bucket array drops its old direct link and grabs the target's next pointer instead:
Before Deleting Key 11:
Bucket [1] ──► [ Key: 11 | Val: 1100 ] ──► [ Key: 1 | Val: 100 ] ──► NULL

After map_delete(&map, 11):
               ┌───────────────────────────┐
               │                           ▼
Bucket [1] ───┴   [ Key: 11 (FREED) ]     [ Key: 1 | Val: 100 ] ──► NULL
  
============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUCKETS 10 // Size of our fixed array of buckets

// 1. The structural building block (Linked List Node)
typedef struct Node {
    int key;
    int value;
    struct Node* next; // Points to the next item if a hash collision occurs
} Node;

// 2. The HashMap container tracking our array of buckets
typedef struct {
    Node* table[BUCKETS];
} HashMap;

// 3. A basic Hash Function (Modulo operator)
int hash_function(int key) {
    int index = key % BUCKETS;
    if (index < 0) index += BUCKETS; // Ensure negative keys generate valid indices
    return index;
}

// Initialize all buckets to NULL
void map_init(HashMap* map) {
    for (int i = 0; i < BUCKETS; i++) {
        map->table[i] = NULL;
    }
}

// 4. Insert or Update a Key-Value pair
void map_put(HashMap* map, int key, int value) {
    int index = hash_function(key);
    Node* current = map->table[index];

    // Check if the key already exists in this bucket chain
    while (current != NULL) {
        if (current->key == key) {
            current->value = value; // Key found! Update value and exit
            return;
        }
        current = current->next;
    }

    // Key doesn't exist: Create a brand new node allocation
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    
    // Insert at the head of the linked list for this bucket slot
    new_node->next = map->table[index];
    map->table[index] = new_node;
}

// 5. Lookup a Value
bool map_get(HashMap* map, int key, int* out_value) {
    int index = hash_function(key);
    Node* current = map->table[index];

    // Traverse the linked list inside this bucket slot
    while (current != NULL) {
        if (current->key == key) {
            *out_value = current->value; // Found it! Populate return reference
            return true;
        }
        current = current->next;
    }
    return false; // Key does not exist
}

// 1. Delete a specific key-value pair and free its memory slot
bool map_delete(HashMap* map, int key) {
    int index = hash_function(key);
    Node* current = map->table[index];
    Node* prev = NULL;

    // Search through the linked list at this bucket slot
    while (current != NULL) {
        if (current->key == key) {
            // Node found! Now update the pointer links to bypass it
            if (prev == NULL) {
                // Scenario A: The node to delete is the first item in the bucket
                map->table[index] = current->next;
            } else {
                // Scenario B: The node to delete is in the middle or end of the chain
                prev->next = current->next;
            }

            free(current); // Reclaim the allocated heap RAM
            return true;   // Deletion complete
        }
        
        // Move forward down the chain, keeping track of the previous node
        prev = current;
        current = current->next;
    }
    
    return false; // Key was not found in the map
}

// 2. Clear all allocations to prevent memory leaks when closing the application
void map_destroy(HashMap* map) {
    for (int i = 0; i < BUCKETS; i++) {
        Node* current = map->table[i];
        while (current != NULL) {
            Node* temp = current->next;
            free(current); // Free current node slot
            current = temp; // Move to next un-freed node
        }
        map->table[i] = NULL; // Reset bucket head
    }
}

// Print function to visually check how items map into memory slots
void map_print(HashMap* map) {
    printf("\n--- HashMap Internal State ---\n");
    for (int i = 0; i < BUCKETS; i++) {
        printf("Bucket [%d]: ", i);
        Node* current = map->table[i];
        while (current != NULL) {
            printf("[%d -> %d] -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main() {
    HashMap map;
    map_init(&map);

    // Setup a bucket chain with a collision at Bucket 1 (11 % 10 = 1, 1 % 10 = 1)
    map_put(&map, 1, 100);
    map_put(&map, 11, 1100);
    map_put(&map, 5, 500);

    printf("--- Before Deletion ---");
    map_print(&map);

    // Test Deletion
    printf("\nDeleting Key 11 (Head of Bucket 1 chain)...");
    map_delete(&map, 11);
    map_print(&map);

    // Clean up remaining allocations completely
    map_destroy(&map);
    printf("\nHashMap safely destroyed. Memory cleared.\n");

    return 0;
}

