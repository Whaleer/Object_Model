---
icon: leaf
description: The Semantics of Function
---

# Chapter 4：Function 语意学

有一个 Point3d 的指针和对象：

```cpp
Point3d obj;
Point3d * ptr = &obj;

Point3d Point3d::normalize() const{
    register float mag = magnitude();
    Point3d normal;
    
    normal._x = _x/mag;
    normal._y = _y/mag;
    normal._z = _z/mag;
    
    return normal;
};

float Point3d::magnitude() const{
    return sqrt(_x * _x + _y * _y + _z * _z);
}
```

当执行：

```cpp
obj.normalize();
ptr->normalize();
```

会发生什么？























