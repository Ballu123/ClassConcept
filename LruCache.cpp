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
    Node* next;
    Node* prev;

    Node(int key) {
        this->key = key;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class Main {
private:
    std::unordered_map<int, Node*> cache;
    int capacity;
    Node* head;
    Node* tail;

public:
    Main(int capacity) {
        this->capacity = capacity;
        this->head = nullptr;
        this->tail = nullptr;
    }

    // Destructor to clean up allocated memory and prevent memory leaks
    ~Main() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void addToHead(Node* node) {
        if (cache.size() == 0) {
            head = node;
            tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
    }

    void deleteNode(Node* node) {
        // if node to be deleted is the first node
        if (head == node)
            head = node->next;
        // if node to be deleted is the last node
        if (tail == node)
            tail = node->prev;
        // boundary condition for start
        if (node->prev != nullptr)
            node->prev->next = node->next;
        // boundary condition for end
        if (node->next != nullptr)
            node->next->prev = node->prev;
    }

    void look(int key) {
        if (cache.find(key) != cache.end()) {
            Node* keyNode = cache[key];
            deleteNode(keyNode);
            // Free the memory of the unlinked node before creating a replacement
            delete keyNode; 
        } else {
            if (cache.size() == capacity) {
                // cache is full, delete the last entry
                int tailKey = tail->key;
                Node* oldTail = tail;
                deleteNode(oldTail);
                cache.erase(tailKey);
                delete oldTail; // Free memory of the evicted node
            }
        }
        // add it to cache
        Node* newNode = new Node(key);
        addToHead(newNode);
        cache[key] = newNode;
        std::cout << "Item accessed: " << key << std::endl;
        printCache();
    }

    void printCache() {
        std::cout << "Cache : ";
        Node* start = head;
        while (start != nullptr) {
            std::cout << start->key << " ";
            start = start->next;
        }
        std::endl(std::cout);
    }
};

int main() {
    int capacity = 4;
    Main lru(capacity);
    lru.look(1);
    lru.look(2);
    lru.look(1);
    lru.look(3);
    lru.look(4);
    lru.look(3);
    lru.look(5);
    lru.look(4);
    lru.look(6);
    return 0;
}

=================================2nd way=======================
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
