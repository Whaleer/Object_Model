#include <iostream>
using namespace std;

class ZooAnimal {
public:
    ZooAnimal() {};
    virtual ~ZooAnimal() {};
    virtual void animate();
    virtual void draw();
private:
    // ZooAnimal 的 animate() 和 draw() 函数所需要的数据
};

class Bear : public ZooAnimal {
public:
    Bear() {};
    ~Bear() {};
    void animate();
    void draw();
    virtual void dance();
private:
    // Bear 的 animate() 和 draw() 和 dance() 函数所需要的数据
};

class Raccoon: public virtual ZooAnimal{
public:
    Raccoon() {};
    Raccoon(int x) {};
};
