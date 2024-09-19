/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: LRU_Cache_C.c
 * Win10
 * Microsoft Visual Studio
 * Implementing an LRU (Least Recently Used) Cache in C can be a bit involved, as it requires a data structure that allows for efficient access, insertion, and deletion of elements. 
 * A common approach is to use a combination of a hash table and a doubly linked list. 
 * The hash table provides O(1) time complexity for access operations, and the doubly linked list provides O(1) time complexity for insertion and deletion operations.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    int capacity;
    int size;
    Node** table;  // Hash table
    Node* head;    // Doubly linked list head (most recently used)
    Node* tail;    // Doubly linked list tail (least recently used)
} LRUCache;

#define TABLE_SIZE 10000

unsigned int hash(int key) {
    return key % TABLE_SIZE;
}

Node* createNode(int key, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->prev = newNode->next = NULL;
    return newNode;
}

LRUCache* createLRUCache(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->table = (Node**)calloc(TABLE_SIZE, sizeof(Node*));
    cache->head = cache->tail = NULL;
    return cache;
}

void removeNode(LRUCache* cache, Node* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        cache->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        cache->tail = node->prev;
    }
}

void addNodeToHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head) {
        cache->head->prev = node;
    }

    cache->head = node;

    if (!cache->tail) {
        cache->tail = node;
    }
}

int get(LRUCache* cache, int key) {
    unsigned int index = hash(key);
    Node* node = cache->table[index];

    while (node) {
        if (node->key == key) {
            // Move the accessed node to the head of the list
            removeNode(cache, node);
            addNodeToHead(cache, node);
            return node->value;
        }
        node = node->next;
    }

    return -1;  // Key not found
}

void put(LRUCache* cache, int key, int value) {
    unsigned int index = hash(key);
    Node* node = cache->table[index];

    while (node) {
        if (node->key == key) {
            // Update the value and move the node to the head
            node->value = value;
            removeNode(cache, node);
            addNodeToHead(cache, node);
            return;
        }
        node = node->next;
    }

    if (cache->size == cache->capacity) {
        // Remove the least recently used node
        Node* lruNode = cache->tail;
        removeNode(cache, lruNode);

        // Remove from hash table
        unsigned int lruIndex = hash(lruNode->key);
        Node* prev = NULL;
        node = cache->table[lruIndex];
        while (node != lruNode) {
            prev = node;
            node = node->next;
        }
        if (prev) {
            prev->next = lruNode->next;
        } else {
            cache->table[lruIndex] = lruNode->next;
        }

        free(lruNode);
        cache->size--;
    }

    // Add the new node
    Node* newNode = createNode(key, value);
    addNodeToHead(cache, newNode);

    // Add to hash table
    newNode->next = cache->table[index];
    cache->table[index] = newNode;
    cache->size++;
}

void freeLRUCache(LRUCache* cache) {
    Node* current = cache->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(cache->table);
    free(cache);
}

int main() {
    LRUCache* cache = createLRUCache(2);

    put(cache, 1, 1);
    put(cache, 2, 2);
    printf("Get key 1: %d\n", get(cache, 1)); // returns 1

    put(cache, 3, 3); // evicts key 2
    printf("Get key 2: %d\n", get(cache, 2)); // returns -1 (not found)

    put(cache, 4, 4); // evicts key 1
    printf("Get key 1: %d\n", get(cache, 1)); // returns -1 (not found)
    printf("Get key 3: %d\n", get(cache, 3)); // returns 3
    printf("Get key 4: %d\n", get(cache, 4)); // returns 4

    freeLRUCache(cache);

    return 0;
}
