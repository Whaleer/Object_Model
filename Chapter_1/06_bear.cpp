#include <iostream>
using namespace std;

class ZooAnimal {
public:
    ZooAnimal(string name, int loc) : name(name), loc(0) {}
    virtual ~ZooAnimal() {}
    virtual void rotate() {
        cout << "ZooAnimal rotate" << endl;
    };
protected:
    int loc;
    string name;
};

class Bear :public ZooAnimal {
public:
    Bear() : ZooAnimal("Bear", 1){}
    ~Bear(){};
    void rotate(){
        cout << "Bear rotate" << endl;
    };
    virtual void dance(){};
protected:
    enum Dances{waltz, tango, foxtrot};
    Dances dances_known;
    int cell_block;
};

int main(){
    Bear b;
    ZooAnimal *pz= &b;
    Bear *pb = &b;
    Bear &rb = *pb;

    cout << sizeof(ZooAnimal) << endl;
    cout << sizeof(Bear) << endl;

    cout << sizeof(b) << endl;
    cout << sizeof(pz) << endl;
    cout << sizeof(pb) << endl;
    cout << sizeof(rb) << endl;
    return 0;
}
