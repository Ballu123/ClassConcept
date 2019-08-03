#include <iostream>
using namespace std;

class B
{
  public:
    virtual void func() {
      cout << "This is class B" << endl;
    }
};
 
class D: public B
{
  public:
    virtual void func() {
      cout << "This is class D" <<endl;
    }
};
 
int main() {
B b;
b.func(); //This is class B

B b1;
B &b2 = b1;
b2.func();	//This is class B

B *b3 = new B;
b3->func(); //This is class B

D d;
d.func(); //This is class D
	
D d1;
D &d2 = d1;
d2.func(); //This is class D
	
D *d3 = new D;
d3->func(); //This is class D

D d4;
B bnew = d4;
bnew.func();	//This is class B

D d5;
B &b_1 = d5;
b_1.func(); //This is class D

B *b7 = new D;
b7->func(); //This is class D
return 0;
}