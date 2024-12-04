#include <iostream>
using namespace std;

class Base1{
private:
    char a;
    int b;
    double c;
};

class Base2{
private:
    char a;
    double b;
    int c;
};

int main(){
    cout << sizeof(Base1) << endl;       // 16
    cout << sizeof(Base2) << endl;       // 24
    return 0;
}