#include <iostream>
using namespace std;

class Basic {
public:
    int a;
    double b;

    void setB(double value){
        b = value;
    }
};

int main() {
    Basic temp;
    temp.a = 10;
    temp.setB(10.1);
    return 0;
}