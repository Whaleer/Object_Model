#include <iostream>
using namespace std;

class Point3d {
public:
    Point3d(float x = 0.0, float y = 0.0, float z = 0.0) : _x(x), _y(y), _z(z) {}

    float x() const { return _x; }

    float y() const { return _y; }

    float z() const { return _z; }

private:
    float _x;
    float _y;
    float _z;
};

inline ostream& operator<<(ostream& os, const Point3d &pt){
    os << "(" << pt.x() << ", " << pt.y() << ", " << pt.z() << ")";
    return os;
}


int main(){
    Point3d pt1(1.0, 2.0, 3.0);
    cout << pt1 << endl;
    return 0;
}