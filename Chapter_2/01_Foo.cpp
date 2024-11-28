#include <iostream>

using namespace std;

class Foo {
public:
    Foo() {};
    Foo(int i) {};
private:
    int val;
    Foo *pnext;
};

class Bar {
public:
    Foo foo;
    char *str;
};

void foo_bar() {
    Bar bar; // Bar::foo 必须在此处初始化
}

int main() {
    foo_bar();
    return 0;
}