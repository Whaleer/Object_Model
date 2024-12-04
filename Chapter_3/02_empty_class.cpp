#include <iostream>
using namespace std;

class Base{};
class Derived: public Base{
public:
    int a;
};

int main(){
    cout << sizeof(Base) << endl;       // 1
    cout << sizeof(Derived) << endl;    // 4
    return 0;
}