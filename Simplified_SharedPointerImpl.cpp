/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Simplified_SharedPointerImpl.cpp
 * A shared pointer is a smart pointer in C++ that helps manage memory for dynamically allocated objects. 
 * It uses reference counting to track how many shared pointers are pointing to the same dynamically allocated object.
 * When the last shared pointer pointing to the object is destroyed or reset, the object's memory is automatically deallocated.
 */

template <typename T>
class SharedPointer {
private:
    T* ptr;
    size_t* refCount;

public:
    SharedPointer() : ptr(nullptr), refCount(nullptr) {}

    SharedPointer(T* rawPtr) : ptr(rawPtr), refCount(new size_t(1)) {}

    SharedPointer(const SharedPointer& other) : ptr(other.ptr), refCount(other.refCount) {
        if (refCount) {
            ++(*refCount);
        }
    }

    SharedPointer& operator=(const SharedPointer& other) {
        if (this != &other) {
            // Decrement the old reference count
            release();

            ptr = other.ptr;
            refCount = other.refCount;

            if (refCount) {
                ++(*refCount);
            }
        }
        return *this;
    }

    ~SharedPointer() {
        release();
    }

    T* get() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    size_t useCount() const {
        return (refCount ? *refCount : 0);
    }

    void reset() {
        release();
        ptr = nullptr;
        refCount = nullptr;
    }

private:
    void release() {
        if (refCount) {
            --(*refCount);
            if (*refCount == 0) {
                delete ptr;
                delete refCount;
            }
        }
    }
};

int main() {
    SharedPointer<int> sp1(new int(42));
    SharedPointer<int> sp2 = sp1;
    SharedPointer<int> sp3;
    sp3 = sp1;

    std::cout << "Use count: " << sp1.useCount() << std::endl;

    sp1.reset();
    std::cout << "Use count after reset: " << sp2.useCount() << std::endl;

    return 0;
}
