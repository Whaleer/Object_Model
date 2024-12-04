#include <iostream>
#include <cstddef>
using namespace std;

class Concrete1{
public:
    int val;
    char bit1;
};

class Concrete2: public Concrete1{
public:
    char bit2, bit3, bit4;
};

class Con1{
private:
    int val;
    char bit1;
};

class Con2: public Con1{
private:
    char bit2, bit3, bit4;
};

int main(){
    Concrete2 tmp;
    Con2 tmp2;
    cout << sizeof(tmp) << endl; // 8
    cout << sizeof(tmp2) << endl; // 8

    cout << offsetof(Concrete1, val) << endl;
    cout << offsetof(Concrete2, bit1) << endl;
    cout << offsetof(Concrete2, bit2) << endl;
    cout << offsetof(Concrete2, bit3) << endl;
    cout << offsetof(Concrete2, bit4) << endl;
    return 0;
}


