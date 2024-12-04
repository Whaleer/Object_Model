#include <iostream>
using namespace std;

class Concrete {
public:
private:
    int val;
    char c1;
    char c2;
    char c3;
};

int main() {
    cout << sizeof(Concrete) << endl; // 8
    return 0;
}