/*
 * Name: Balram Panwar
 * Email ID: balram.panwar@gmail.com
 * Name of the file: LruCache.cpp
 * Win10
 * Microsoft Visual Studio
 * Ref: http://bhrigu.me/blog/2017/01/22/lru-cache-c-plus-plus-implementation/
 */
===============================1st way --i am following=========================
    #include <iostream>
#include <unordered_map>

class Node {
public:
    int key;
    int value; // Added value support to store actual payload data
    Node* next;
    Node* prev;

    Node(int key, int value) {
        this->key = key;
        this->value = value;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class LRUCache {
private:
    std::unordered_map<int, Node*> cache;
    int capacity;
    Node* head;
    Node* tail;

    void addToHead(Node* node) {
        if (head == nullptr) { // Safely checks structural boundary rather than map tracking size
            head = node;
            tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
    }

    void deleteNode(Node* node) {
        if (head == node)
            head = node->next;
        if (tail == node)
            tail = node->prev;
        if (node->prev != nullptr)
            node->prev->next = node->next;
        if (node->next != nullptr)
            node->next->prev = node->prev;
            
        node->next = nullptr;
        node->prev = nullptr;
    }

    // Moves an existing node to the head of the list (marks it as Most Recently Used)
    void moveToHead(Node* node) {
        deleteNode(node);
        addToHead(node);
    }

public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        this->head = nullptr;
        this->tail = nullptr;
    }

    // Clean up remaining allocations on destruction to eliminate memory leaks
    ~LRUCache() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Retrieves a value from the cache. Returns -1 if key does not exist.
    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1; 
        }
        Node* node = cache[key];
        moveToHead(node); // Accessed item becomes Most Recently Used
        return node->value;
    }

    // Inserts or updates a key-value pair in the cache.
    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // Key exists: Update value and bring to head (No new node creation required)
            Node* existingNode = cache[key];
            existingNode->value = value;
            moveToHead(existingNode);
        } else {
            // Key does not exist: Check capacity thresholds
            if (cache.size() == capacity) {
                Node* oldTail = tail;
                int tailKey = oldTail->key;
                
                deleteNode(oldTail);
                cache.erase(tailKey);
                delete oldTail; // Free memory allocated for evicted node
            }
            
            // Allocate fresh node and link it at head
            Node* newNode = new Node(key, value);
            addToHead(newNode);
            cache[key] = newNode;
        }
    }

    void printCache() {
        std::cout << "Cache (MRU -> LRU): ";
        Node* start = head;
        while (start != nullptr) {
            std::cout << "[" << start->key << ":" << start->value << "] ";
            start = start->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    int capacity = 3;
    LRUCache lru(capacity);

    std::cout << "--- Inserting Items ---" << std::endl;
    lru.put(1, 100); lru.printCache();
    lru.put(2, 200); lru.printCache();
    lru.put(3, 300); lru.printCache();

    std::cout << "\n--- Fetching Key 1 (Moves to front) ---" << std::endl;
    std::cout << "Value of Key 1: " << lru.get(1) << std::endl;
    lru.printCache();

    std::cout << "\n--- Eviction Triggered (Key 2 dropped, capacity is 3) ---" << std::endl;
    lru.put(4, 400); lru.printCache();

    std::cout << "\n--- Updating Existing Key 3 ---" << std::endl;
    lru.put(3, 350); lru.printCache();

    return 0;
}

================================================================================

=================================2nd way========================================
#include <iostream>
#include <map>
using namespace std;
class Node {
  public:
  int key, value;
  Node *prev, *next;
  Node(int k, int v): key(k), value(v), prev(NULL), next(NULL) {}
};

class DoublyLinkedList {
  Node *front, *rear;
  
  bool isEmpty() {
      return rear == NULL;
  }

  public:
  DoublyLinkedList(): front(NULL), rear(NULL) {}
  
  Node* add_page_to_head(int key, int value) {
      Node *page = new Node(key, value);
      if(!front && !rear) {
          front = rear = page;
      }
      else {
          page->next = front;
          front->prev = page;
          front = page;
      }
      return page;
  }

  void move_page_to_head(Node *page) {
      if(page==front) {
          return;
      }
      if(page == rear) {
          rear = rear->prev;
          rear->next = NULL;
      }
      else {
          page->prev->next = page->next;
          page->next->prev = page->prev;
      }

      page->next = front;
      page->prev = NULL;
      front->prev = page;
      front = page;
  }

  void remove_rear_page() {
      if(isEmpty()) {
          return;
      }
      if(front == rear) {
          delete rear;
          front = rear = NULL;
      }
      else {
          Node *temp = rear;
          rear = rear->prev;
          rear->next = NULL;
          delete temp;
      }
  }
  Node* get_rear_page() {
      return rear;
  }
  
};

class LRUCache{
  int capacity, size;
  DoublyLinkedList *pageList;
  map<int, Node*> pageMap;

  public:
    LRUCache(int capacity) {
      this->capacity = capacity;
      size = 0;
        pageList = new DoublyLinkedList();
        pageMap = map<int, Node*>();
    }

    int get(int key) {
        if(pageMap.find(key)==pageMap.end()) {
          return -1;
        }
        int val = pageMap[key]->value;

        // move the page to front
        pageList->move_page_to_head(pageMap[key]);
        return val;
    }

    void put(int key, int value) {
      if(pageMap.find(key)!=pageMap.end()) {
          // if key already present, update value and move page to head
          pageMap[key]->value = value;
          pageList->move_page_to_head(pageMap[key]);
          return;
      }

        if(size == capacity) {
          // remove rear page
          int k = pageList->get_rear_page()->key;
          pageMap.erase(k);
          pageList->remove_rear_page();
          size--;
        }

        // add new page to head to Queue
        Node *page = pageList->add_page_to_head(key, value);
        size++;
        pageMap[key] = page;
    }

    ~LRUCache() {
      map<int, Node*>::iterator i1;
      for(i1=pageMap.begin();i1!=pageMap.end();i1++) {
          delete i1->second;
      }
      delete pageList;
    }
};
