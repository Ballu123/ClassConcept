/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: HashMap.cpp
 * Win10
 * Microsoft Visual Studio
 * The HashMap class implements a basic hash map using separate chaining for collision resolution. It supports insertion, retrieval, 
 * and removal of key-value pairs. The hash function is used to determine the bucket index for each key. Each bucket is a linked list that stores pairs of keys and values.
 */

#include <iostream>
#include <vector>
#include <list>

template<typename K, typename V>
class HashMap {
private:
    size_t size;
    std::vector<std::list<std::pair<K, V>>> buckets;

    size_t hashFunction(const K& key) const {
        // Hash function logic here
        // You can use std::hash or custom hash functions
        return std::hash<K>{}(key) % buckets.size();
    }

public:
    HashMap(size_t initialSize = 10) : size(initialSize), buckets(initialSize) {}

    void insert(const K& key, const V& value) {
        size_t bucketIndex = hashFunction(key);
        auto& bucket = buckets[bucketIndex];

        for (auto& entry : bucket) {
            if (entry.first == key) {
                entry.second = value;
                return;
            }
        }

        bucket.emplace_back(key, value);
    }

    bool get(const K& key, V& value) const {
        size_t bucketIndex = hashFunction(key);
        const auto& bucket = buckets[bucketIndex];

        for (const auto& entry : bucket) {
            if (entry.first == key) {
                value = entry.second;
                return true;
            }
        }

        return false;
    }

    void remove(const K& key) {
        size_t bucketIndex = hashFunction(key);
        auto& bucket = buckets[bucketIndex];

        auto it = bucket.begin();
        while (it != bucket.end()) {
            if (it->first == key) {
                it = bucket.erase(it);
                return;
            }
            ++it;
        }
    }
};

int main() {
    HashMap<std::string, int> myHashMap;
    
    myHashMap.insert("apple", 5);
    myHashMap.insert("banana", 10);
    myHashMap.insert("orange", 8);

    int value;
    if (myHashMap.get("banana", value)) {
        std::cout << "Value of banana: " << value << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }

    myHashMap.remove("apple");

    if (myHashMap.get("apple", value)) {
        std::cout << "Value of apple: " << value << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }

    return 0;
}
