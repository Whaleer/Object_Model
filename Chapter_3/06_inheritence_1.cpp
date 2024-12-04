#include <iostream>
using namespace std;

class Point2d{
public:
    Point2d(float x = 0.0, float y = 0.0):_x(x),_y(y){}
    float x() {return _x;}
    float y() {return _y;}

    void x(float newX) {_x = newX;}
    void y(float newY) {_y = newY;}

    void operator+=(const Point2d & rhs){
        _x += rhs._x;
        _y += rhs._y;
    }
protected:
    float _x,_y;
};

class Point3d: public Point2d{
public:
    Point3d(float x = 0.0, float y = 0.0, float z = 0.0):Point2d(x,y),_z(z){}

    float z() {return _z;}
    void z(float newZ) {_z = newZ;}

    void operator+=(const Point3d & rhs){
        Point2d::operator+=(rhs);
        _z += rhs._z;
    }
protected:
    float _z;
};