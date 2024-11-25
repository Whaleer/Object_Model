#include <iostream>
using namespace std;


class Point{
public:
    Point(float xval);
    virtual ~Point();

    float x() const;
    static int point_count();

protected:
    virtual ostream& print(ostream& os) const;
    float _x;
    static int _point_count;
};

int main(){
    cout << sizeof(Point) << endl;
    return 0;
}


