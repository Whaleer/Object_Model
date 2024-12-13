#include <iostream>
using namespace std;

class Point2d{
public:
    Point2d(float x = 0.0, float y = 0.0): _x(x), _y(y){}
    virtual float z(){ return 0.0; }
    virtual void z(float ) {}

    virtual void operator+=(const Point2d &rhs){
        _x += rhs.x();
        _y += rhs.y();
    }
protected:
    float _x, _y;
};

class Point3d: public Point2d{
public:
    Point3d(float x = 0.0, float y = 0.0, float z = 0.0): Point2d(x, y), _z(z){}
    float z() { return _z; }
    void z(float newZ) { _z = newZ; }
    void operator+=(const Point2d &rhs){
        Point2d::operator+=(rhs);
        _z += rhs.z();
    }
private:
    float _z;
};