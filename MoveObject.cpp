/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: MoveObject.cpp
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

class A
{
  public:
    // constructor
    explicit A(size_t length)
        : mLength(length), mData(new int[length]) {}

    // move constructor
    A(A&& other)
    {
      mData = other.mData;
      mLength = other.mLength;
      other.mData = nullptr;
      other.mLength = 0;
    }

    // move assignment
    A& operator=(A&& other) noexcept
    {
      mData =  other.mData;
      mLength = other.mLength;
      other.mData = nullptr;
      other.mLength = 0;
      return *this;
    }

    size_t getLength() { return mLength; }

    
    void swap(A& other)
    {
      A temp = move(other);
      other = move(*this);
      *this = move(temp);
    }

    int* get_mData() { return mData; }

  private:
    int *mData;
    size_t mLength;
};

int main()
{
  A a(11), b(22);
  cout << a.getLength() << ' ' << b.getLength() << endl;
  cout << a.get_mData() << ' ' << b.get_mData() << endl;
  swap(a,b);
  cout << a.getLength() << ' ' << b.getLength() << endl;
  cout << a.get_mData() << ' ' << b.get_mData() << endl;
  return 0;
}
/*
11 22
0x23d2c20 0x23d2c60
22 11
0x23d2c60 0x23d2c20
*/
