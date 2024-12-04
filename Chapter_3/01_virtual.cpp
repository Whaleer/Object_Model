#include <iostream>
using namespace std;

class X{};
class Y:public virtual X{};
class Z:public virtual X{};
class A:public Y,public Z{};

int main(){

    cout << sizeof(X) << endl; // 1
    cout << sizeof(Y) << endl; // 8
    cout << sizeof(Z) << endl; // 8
    cout << sizeof(A) << endl; // 16
    return 0;
}