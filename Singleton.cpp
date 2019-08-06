/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Singleton.cpp
 * Win10
 * Microsoft Visual Studio
 */
 
#include <iostream>

class Singleton {
private:
  /* Here will be the instance stored. */
  static Singleton* instance;
  /* Private constructor to prevent instancing. */ //imp
  Singleton();
public:
  /* Static access method. */
  static Singleton* getinstance();
};

/* Null, because instance will be initialized on demand. */
Singleton* Singleton::instance = 0;

Singleton::Singleton() {}

Singleton* Singleton:: getinstance() {
  if(!instance) {
    instance = new Singleton();
  }
  return instance;
}

int main() {
  std::cout << "Implementing Singleton Class!\n";
  //new Singleton(); // Won't work
  Singleton *r = Singleton::getinstance();
  Singleton *s = Singleton::getinstance();
  
  /* The addresses will be the same. */
  std::cout << s << std::endl;
  std::cout << r << std::endl;
}
