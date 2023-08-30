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

    /*** Copy Semantics ***/
    // Copy constructor deleted
    unique_ptr(const unique_ptr&) = delete;

    // Move constructor
    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    /*** Move Semantics ***/
    // Assignment operator deleted
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Assignment operator
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // Member functions Get the pointer
    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* ptr = nullptr) noexcept {
        delete ptr_;
        ptr_ = ptr;
    }
    
    // Overload * operator
    T& operator*() const noexcept {
        return *ptr_;
    }

    // Overload -> operator
    T* operator->() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
};

int main (){
  unique_ptr<int>ptr1(new int(2));
  unique_ptr<int>ptr2(new int(500));
  ptr2 = std:move(ptr1);
  ptr1.get();
  ptr1.reset(new int(30));
}
