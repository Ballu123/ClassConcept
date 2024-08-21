/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: SharedPointerImpl.cpp
 * Win10
 * Microsoft Visual Studio
 * ref : https://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c
 * ref : https://www.acodersjourney.com/implementing-smart-pointer-using-reference-counting/
 */
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<memory>
#include<algorithm>
#include<bits/stdc++.h> 
using namespace std;
/* Reference Count: A simple class for maanging the number of active smart pointers*/
class ReferenceCount
{
private:
  int m_RefCount{ 0 };
public:
  void Increment()
  {
    ++m_RefCount;
  }
  int Decrement()
  {
    return --m_RefCount;
  }
  int GetCount() const
  {
    return m_RefCount;
  }
};
template <typename T>
class smart_ptr
{
private:
  T* m_Object{ nullptr };
  ReferenceCount* m_ReferenceCount{ nullptr };
public:
  smart_ptr()
  {
  }
  //Constructor
  smart_ptr(T* object) : m_Object{ object }, m_ReferenceCount{ new ReferenceCount() }
  {
    m_ReferenceCount->Increment();
    cout << "Created smart_ptr! Ref count is " << m_ReferenceCount->GetCount() << endl;
  }

  //Destructor
  virtual ~smart_ptr()
  {
    if (m_ReferenceCount)
    {
      int decrementedCount = m_ReferenceCount->Decrement();
      cout << "Destroyed smart_ptr! Ref count is " << decrementedCount << endl;
      if (decrementedCount <= 0)
      {
        delete m_ReferenceCount;
        delete m_Object;
        m_ReferenceCount = nullptr;
        m_Object = nullptr;
      }
    }
  }

  // Copy Constructor
  smart_ptr(const smart_ptr<T>& other) : m_Object{ other.m_Object }, m_ReferenceCount{ other.m_ReferenceCount }
  {
    m_ReferenceCount->Increment();
    cout << "Copied smart_ptr! Ref count is "
      << m_ReferenceCount->GetCount() << endl;
  }

  // Overloaded Assignment Operator
  smart_ptr<T>& operator=(const smart_ptr<T>& other)
  {
    if (this != &other)
    {
      if (m_ReferenceCount && m_ReferenceCount->Decrement() == 0)
      {
        delete m_ReferenceCount;
        delete m_Object;
      }
      m_Object = other.m_Object;
      m_ReferenceCount = other.m_ReferenceCount;
      m_ReferenceCount->Increment();
    }
    cout << "Assigning smart_ptr! Ref count is " << m_ReferenceCount->GetCount() << endl;
    return *this;
  }

  //Dereference operator
  T& operator*()
  {
    return *m_Object;
  }

  //Member Access operator
  T* operator->()
  {
    return m_Object;
  }

};
class AirCraft
{
private:
  std::string m_Model;
public:
  AirCraft() :m_Model("Generic Model")
  {
    cout << "Generic model aircraft created" << endl;
  }
  AirCraft(const string& modelName) :m_Model(modelName)
  {
    cout << "Aircraft type" << m_Model << "is created!" << endl;
  }
  void SetAirCraftModel(const string& modelName)
  {
    cout << "Aircraft model changed from  " << m_Model << " to " << modelName << endl;
    m_Model = modelName;
  }
  ~AirCraft()
  {
    cout << "Destroying Aircraft of model:" << m_Model << "!" << endl;
  }
};

int main()
{
  // Create two aircraft objects. 
  smart_ptr<AirCraft> raptorPointer(new AirCraft("F-22 Raptor")); // Ref Count for raptorPointer = 1
  raptorPointer->SetAirCraftModel("B2 Bomber");   // rename the model using pointer access operator
  (*raptorPointer).SetAirCraftModel("B2 Spirit"); // rename the model using the pointer dereference operator
  smart_ptr<AirCraft> hornettPointer(new AirCraft("F-14 Hornett")); // Ref count for hornettPointer = 1
  raptorPointer = hornettPointer; // raptorPointer now points to "F14-Hornett".Ref count for hornett is 2. "F-22 Raptor" is destroyed. Ref count for hornett is 2
  return 0;
}
/*
Aircraft typeF-22 Raptoris created!
Created smart_ptr! Ref count is 1
Aircraft model changed from  F-22 Raptor to B2 Bomber
Aircraft model changed from  B2 Bomber to B2 Spirit
Aircraft typeF-14 Hornettis created!
Created smart_ptr! Ref count is 1
Destroying Aircraft of model:B2 Spirit!
Assigning smart_ptr! Ref count is 2
Destroyed smart_ptr! Ref count is 1
Destroyed smart_ptr! Ref count is 0
Destroying Aircraft of model:F-14 Hornett!
*/
