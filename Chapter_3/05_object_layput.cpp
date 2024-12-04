#include <iostream>
#include <list>
#include <cstddef>
using namespace std;

class Point3d{
public:
    float x;
    static list<Point3d*> *freeList;
    float y;
    static const int chunkSize = 250;
    float z;
};

int main(){
    cout << sizeof(Point3d) << endl; // 12
    cout << "Offset of x: " << offsetof(Point3d, x) << endl;
    cout << "Offset of y: " << offsetof(Point3d, y) << endl;
    cout << "Offset of z: " << offsetof(Point3d, z) << endl;
    return 0;
}