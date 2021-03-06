/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: MoveConstAssignoperator.cpp
 * Win10
 * Microsoft Visual Studio
 * ref : https://www.bogotobogo.com/cplusplus/C11/5_C11_Move_Semantics_Rvalue_Reference.php
 * ref : https://mbevin.wordpress.com/2012/11/20/move-semantics/
 * The example above adds two elements to a vector object and then inserts a new 
 * element between the two existing elements.
 */
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<memory>
#include<algorithm>
#include<bits/stdc++.h> 
using namespace std;

class A {
private:
    size_t mLength; // The length of the resource.
    int* mData;     // The resource
public:

    // Simple constructor that initializes the resource.
    explicit A(size_t length)
        : mLength(length), mData(new int[length]) {
        std::cout << "A(size_t). length = "
		<< mLength << "." << std::endl;
    }

    // Destructor.
    ~A() {
	std::cout << "~A(). length = " << mLength << ".";

	if (mData != NULL) {
            std::cout << " Deleting resource.";
	    delete[] mData;  // Delete the resource.
	}
	std::cout << std::endl;
    }

    // Copy constructor.
    A(const A& other)
	    : mLength(other.mLength), mData(new int[other.mLength]) {
	std::cout << "A(const A&). length = "
		<< other.mLength << ". Copying resource." << std::endl;

	std::copy(other.mData, other.mData + mLength, mData);
    }

    // Copy assignment operator.
    A& operator=(const A& other) {
	std::cout << "operator=(const A&). length = "
	         << other.mLength << ". Copying resource." << std::endl;

	if (this != &other) {
	    delete[] mData;  // Free the existing resource.
	    mLength = other.mLength;
            mData = new int[mLength];
            std::copy(other.mData, other.mData + mLength, mData);
	}
	return *this;
    }

    // Move constructor.
    A(A&& other) : mData(NULL), mLength(0) {
        std::cout << "A(A&&). length = " 
             << other.mLength << ". Moving resource.\n";

        // Copy the data pointer and its length from the 
        // source object.
        mData = other.mData;
        mLength = other.mLength;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other.mData = NULL;
        other.mLength = 0;
    }

    // Move assignment operator.
    A& operator=(A&& other) {
        std::cout << "operator=(A&&). length = " 
             << other.mLength << "." << std::endl;

        if (this != &other) {
          // Free the existing resource.
          delete[] mData;

          // Copy the data pointer and its length from the 
          // source object.
          mData = other.mData;
          mLength = other.mLength;

          // Release the data pointer from the source object so that
          // the destructor does not free the memory multiple times.
          other.mData = NULL;
          other.mLength = 0;
       }
       return *this;
    }

    // Retrieves the length of the data resource.
    size_t Length() const {
        return mLength;
    }
};

int main()
{
   // Create a vector object and add a few elements to it.
   std::vector<A> v;
   v.push_back(A(25));
   v.push_back(A(75));

   // Insert a new element into the second position of the vector.
   v.insert(v.begin() + 1, A(50));
   return 0;
}
/*
A(size_t). length = 25.
A(A&&). length = 25. Moving resource.
~A(). length = 0.
A(size_t). length = 75.
A(A&&). length = 75. Moving resource.
A(const A&). length = 25. Copying resource.
~A(). length = 25. Deleting resource.
~A(). length = 0.
A(size_t). length = 50.
A(A&&). length = 50. Moving resource.
A(const A&). length = 25. Copying resource.
A(const A&). length = 75. Copying resource.
~A(). length = 25. Deleting resource.
~A(). length = 75. Deleting resource.
~A(). length = 0.
~A(). length = 25. Deleting resource.
~A(). length = 50. Deleting resource.
~A(). length = 75. Deleting resource.
*/
