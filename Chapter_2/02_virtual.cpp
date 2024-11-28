#include <iostream>
using namespace std;

class Widget {
public:
    virtual void flip() = 0;
};

class Bell : public Widget {
public:
    void flip() {
        cout << "Ding!" << endl;
    }
};

class Whistle : public Widget {
public:
    void flip() {
        cout << "Ta-da!" << endl;
    }
};

void flip( Widget &weight) {
    weight.flip();
}

void foo() {
    Bell b;
    Whistle w;

    flip(b);
    flip(w);
}

int main() {
    foo();
    return 0;
}