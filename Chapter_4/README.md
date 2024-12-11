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

**会发生什么？**

C++ 支持三种不同类型的 member function: **static, non-static, virtual**

**static member function 得特点：**

* 不能直接存取 non-static 数据
* **不能被声明为 const ：这句话其实是不准确的。**&#x9759;态成员函数与对象实例无关，因此谈论const修饰符（用于限制修改对象的状态）是无意义的。

## 4.1 Member 的各种调用方式

### Non-static Member Functions: 非静态成员函数

C++ 设计准则之一： non-static member function 至少必须和一般的 nonmember function 有相同的效率。

编译器内部会将 **“member 函数实例”** 转换为对等的 **“nonmember函数实例”**

```cpp
float Point3d::magnitude3d() const {...}
```

看上面这个函数，转化步骤如下：

1. 改写函数的 signature 以安插一个额外的参数到 member function 中，用以提供一个存取管道，使 class object 得以将此函数调用，该额外参数被称为 this 指针：

```cpp
// non-const nonstatic member 得扩张过程
Point3d Point3d::magnitude(Point3d * const this)

// 如果 member function 是 const，则变成：
Point3d Point3d::magnitude(const Point3d * const this)
```

2. 将每一个对 “non-static data member 的存取操作” 改为经由 this 指针来存取：

```cpp
{
    return sqrt(this->_x * this->_x +
                this->_y * this->_y + 
                this->_z * this->_z );
}
```

3. 将 member function 重新写成一个外部函数，将函数名称经过 **“mangling” 处理**，使他在程序中成为独一无二的词汇：

```cpp
extern magnitude_7Point3dFv(register Point3d * const this);
```

现在函数已经转换好了，于是

```cpp
obj.magnitude();
ptr->magnitude();

// 分别转换为
magnitude_7Point3dFv(&obj);
magnitude_7Point3dFv(ptr);
```





















