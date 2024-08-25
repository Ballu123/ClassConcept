    /*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: static_virtual.cpp
 * Win10
 * Microsoft Visual Studio
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include <iostream>
#include <string>
#include <memory>
using namespace std;

class A {
public:
  static A sa; //instance of class
  static int id;
  int localId;
  A() { 
    localId = id++;
    cout<< "constructor of A with id:" << localId << endl;
  }
  virtual ~A() {
    cout << "Destructor of A id: " << localId << endl;
  }
  virtual void show() { 
    cout << "virtual of A\n"; 
  }
};

A A::sa;
int A::id = 0;

class B : public A {
public:
  B() {
    cout << "constructor of B " << localId << endl; 
  }
   void show() {
     cout << "virtual of B "<< localId << endl;
   }
   ~B(){
     cout << "Destructor of B "<< localId << endl;
   }
};

int main(int argc, char ** argv) {
#if 0
  A* a;     //Base class pointer
  B b;      //Derived class object
  a = &b;
  a->show(); //Late Binding Ocuurs
#endif
  unique_ptr<A> a(new B());
  //A* a = new B();
  a->show();
  cout << "=========NEW=======" <<endl;
  A a1 = *a;
  a1.show();
  /*
  constructor of A with id:0
  constructor of A with id:1
  constructor of B 1
  virtual of B 1
  =========NEW=======
  virtual of A
  Destructor of A id: 1
  Destructor of B 1
  Destructor of A id: 1
  Destructor of A id: 0 
  */
  return 0;
}
 
