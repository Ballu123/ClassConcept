/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: New_Operator_Impl.cpp
 * Platform: Linux 
 * To implement a custom memory allocation function using the new operator in C++, we need to override the new and delete operators. 
 * This allows us to control how memory is allocated and deallocated for objects in our program. 
 * This is useful when you want to implement custom memory management techniques, such as pooling or tracking allocation
 */

#include <iostream>
#include <cstdlib>  // For malloc and free

// Overloading the global new operator
void* operator new(size_t size) {
    std::cout << "Custom new called. Allocating " << size << " bytes.\n";
    void* p = std::malloc(size);  // Allocate memory using malloc
    if (!p) {
        throw std::bad_alloc();  // Throw an exception if allocation fails
    }
    return p;
}

// Overloading the global delete operator
void operator delete(void* p) noexcept {
    std::cout << "Custom delete called. Freeing memory.\n";
    std::free(p);  // Free the memory using free
}

class Test {
public:
    int x;
    Test(int val) : x(val) {
        std::cout << "Constructor called for Test object.\n";
    }

    ~Test() {
        std::cout << "Destructor called for Test object.\n";
    }
};

int main() {
    // Using the custom new and delete operators
    std::cout << "Creating Test object with custom new.\n";
    Test* obj = new Test(42);  // This will use the custom new operator
    std::cout << "Test object created with value: " << obj->x << "\n";
    
    // Deleting the object using the custom delete operator
    std::cout << "Deleting Test object with custom delete.\n";
    delete obj;  // This will use the custom delete operator

    return 0;
}
