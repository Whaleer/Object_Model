#include <iostream>
using namespace std;

class Base{
public:
    Base(){}
private:
    string name;
};

int main(){
    cout << sizeof(Base) << endl; //32
}