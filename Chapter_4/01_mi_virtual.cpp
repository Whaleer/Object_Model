#include <iostream>
using namespace std;

class Base1{
public:
    Base1();
    virtual ~Base1();
    virtual void speakClearly();
    virtual Base1 * clone() const;
protected:
    float data_Base1;
};

class Base2{
public:
    Base2();
    virtual ~Base2();
    virtual void mumble();
    virtual Base2 * clone() const;
protected:
    float data_Base2;
};

class Derived:public Base1,public Base2{
public:
    Derived();
    virtual ~Derived();
    virtual Derived * clone() const;
protected:
    char data_Derived;
};