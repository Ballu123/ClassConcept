#include <iostream>

using namespace std;
class Base{
  public:
    Base(){
        cout << "Base constructor was called" << endl;
    }
    virtual void printMe(){
        cout << "printMe was called from Base" << endl;
    }
    virtual ~Base(){  //V.imp base class destructor must be virtual
        cout << "Base destructor was called" << endl;
    }
};

class Derived : public Base{
  private:
    string* m_string;
  public:
    Derived(){
      cout << "Derived constructor was called" << endl;
      m_string = new string;
      *m_string = "Hello World";
    }
    virtual void printMe(){
      cout << "printMe was called from Derived" <<endl;
      cout << *m_string << endl;
    }
    ~Derived(){
      cout << "Derived destructor was called" << endl;
      delete m_string;
    }
};
int main(void){
  Base * b = new Derived;
  b->printMe();
  delete b;
/*Base constructor was called
  Derived constructor was called
  printMe was called from Derived
  Hello World
  Derived destructor was called
  Base destructor was called*/
 return 0;
}
