/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Simplified_SharedPointerImpl.cpp
 * A shared pointer is a smart pointer in C++ that helps manage memory for dynamically allocated objects. 
 * It uses reference counting to track how many shared pointers are pointing to the same dynamically allocated object.
 * When the last shared pointer pointing to the object is destroyed or reset, the object's memory is automatically deallocated.
 * Ref : https://medium.com/analytics-vidhya/c-shared-ptr-and-how-to-write-your-own-d0d385c118ad
 * Ref : https://www.vishalchovatiya.com/move-constructor-assignment-operator-with-shared-ptr/#l-value_reference_r-value_reference
 */

#include <iostream>

template<typename T>
class SharedPtr {
private:
    T* ptr;                 // Raw pointer to the object
    unsigned* ref_count;    // Pointer to the reference count

public:
    // Constructor
    explicit SharedPtr(T* p = nullptr) : ptr(p), ref_count(new unsigned(1)) {
        if (!p) {
            *ref_count = 0;
        }
    }

    // Copy constructor
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        if (ptr) {
            (*ref_count)++;
        }
    }

    // Move constructor
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Copy assignment operator
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            // Decrement the old reference count and delete if necessary
            if (ptr && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }

            // Copy the data from the other `SharedPtr`
            ptr = other.ptr;
            ref_count = other.ref_count;

            // Increment the reference count
            if (ptr) {
                (*ref_count)++;
            }
        }
        return *this;
    }

    // Move assignment operator
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            // Decrement the old reference count and delete if necessary
            if (ptr && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }

            // Transfer ownership
            ptr = other.ptr;
            ref_count = other.ref_count;

            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // Destructor
    ~SharedPtr() {
        if (ptr && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const {
        return ptr;
    }

    // Get the raw pointer
    T* get() const {
        return ptr;
    }

    // Get the reference count
    unsigned use_count() const {
        return *ref_count;
    }
};

int main() {
    SharedPtr<int> sp1(new int(42));
    std::cout << "sp1 count: " << sp1.use_count() << ", value: " << *sp1 << std::endl;

    {
        SharedPtr<int> sp2 = sp1;
        std::cout << "sp2 count: " << sp2.use_count() << ", value: " << *sp2 << std::endl;
    }

    std::cout << "sp1 count after sp2 goes out of scope: " << sp1.use_count() << std::endl;

    SharedPtr<int> sp3(new int(100));
    sp3 = sp1;
    std::cout << "sp3 count after assignment: " << sp3.use_count() << ", value: " << *sp3 << std::endl;

    return 0;
}
