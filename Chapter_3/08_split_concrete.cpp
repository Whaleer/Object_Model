#include <iostream>
#include <cstddef>
using namespace std;

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
    Con2 tmp;
    cout << sizeof(tmp) << endl; // 8

    return 0;
}


