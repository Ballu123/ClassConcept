/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Unique_Pointer_Impl.cpp
 * Win10
 * Microsoft Visual Studio
 * Ref: https://www.vishalchovatiya.com/unique_ptr-with-example-in-c/
 * Ref: https://blog.devgenius.io/unique-pointer-and-implementation-in-c-ec6599a518e5
 * Advantages of Unique Pointers:
 * 1.	Memory safety     2. Single ownership    3. Move semantics    4. Custom deleters   5. Standard library support: 
 * Disadvantages of Unique Pointers:
 * 1.	Limited sharing   2. Syntax complexity   3.	Custom deleter management    4.	Performance overhead 
 */

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<memory>
#include<algorithm>
#include<bits/stdc++.h> 

template <typename T>
class unique_ptr {
private:
    T* ptr_;
public:
    // Constructor
    explicit unique_ptr(T* ptr = nullptr) : ptr_(ptr) {}

    // Destructor
    ~unique_ptr() {
        delete ptr_;
    }

    // Copy constructor and assignment operator deleted
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    / Move constructor and assignment operator
    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_; // Delete the current resource
            ptr_ = other.ptr_;  // Transfer ownership
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // Get the raw pointer
    T* get() const noexcept {
        return ptr_;
    }

    // Release ownership of the managed object
    T* release() noexcept {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

   // Reset the managed object with a new pointer (optional)
    void reset(T* ptr = nullptr) noexcept {
        delete ptr_;
        ptr_ = ptr;
    }
    
    // Overload * operator and Dereference operator
    T& operator*() const noexcept {
        return *ptr_;
    }

    // arrow operator
    T* operator->() const noexcept {
        return ptr_;
    }
    //This method is for to check the unique ptr is null or not
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
};

int main (){

  unique_ptr<int>ptr1(new int(2)); // Create a UniquePtr for an integer
  std::cout << "Value: " << *ptr1 << std::endl;
    
  unique_ptr<int>ptr2(new int(500));
  ptr2 = std:move(ptr1);
  std::cout << "Value after move: " << *ptr2 << std::endl;  
    
 // p1 is now empty
 if (!ptr1.get()) {
    std::cout << "ptr1 is now empty" << std::endl;
 }

 // Reset ptr1 with a new value
 ptr1.reset(new int(30));
 std::cout << "New value in ptr1: " << *ptr1 << std::endl;

 return 0;
}
