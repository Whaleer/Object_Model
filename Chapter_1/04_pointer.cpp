#include <iostream>

using namespace std;

class ZooAnimal {
public:
    ZooAnimal(string name, int loc) : name(name), loc(0) {}
    virtual ~ZooAnimal() {}
    virtual void rotate() {};
protected:
    int loc;
    string name;
};

int main() {
    ZooAnimal za("Zoey", 3);
    ZooAnimal *pza = &za;
    cout << sizeof(za) << endl;
    cout << sizeof(pza) << endl;

    int a=1;
    int *pa = &a;
    cout << sizeof(pa) << endl;

    char b= 'b';
    char *pb = &b;
    cout << sizeof(pb) << endl;

    void *ptr = nullptr;
    cout << sizeof(ptr) << endl;
}