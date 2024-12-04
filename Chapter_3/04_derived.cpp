#include <iostream>
using namespace std;

class Base {
private:
    char a;
public:
    virtual void f();
    virtual void g();
};

class Derived0 : public Base {
private:
    char b;
};

class Derived1 : public Base {
private:
    int b;
public:
    void f();
};

class Derived2 : public Base {
private:
    double b;
public:
    void g();
    virtual void h();
};
int main(){
    cout << sizeof(Base) << endl;     // 16
    cout << sizeof(Derived0) << endl; // 16
    cout << sizeof(Derived1) << endl; // 16
    cout << sizeof(Derived2) << endl; // 24
    return 0;
}
