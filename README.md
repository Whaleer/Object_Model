---
description: 关于对象（Object Lessons）
cover: >-
  .gitbook/assets/menitrust_painting_of_a_flatland_with_less_vegetation_at_sunse_b8b95c0c-9e95-4aa6-997d-13c6263db7db.png
coverY: 67.16463414634143
layout:
  cover:
    visible: true
    size: full
  title:
    visible: true
  description:
    visible: true
  tableOfContents:
    visible: true
  outline:
    visible: true
  pagination:
    visible: true
---

# 🌷 Chapter One：关于对象

> **C++ 的抽象数据类型（Abstract Data Type，ADT）是什么？**

```cpp
// class Point3d
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
```

Point3d 是一个抽象数据类型，主要体现在：

* **封装性：**用户只能通过公开的成员函数（如 `x()`, `y()`, 和 `z()`）访问坐标值，而无法直接操作私有成员变量
* **明确的接口：**Point3d 提供了易用的接口（`x()`, `y()`, `z()`），用于访问坐标值。用户不需要知道内部 \_x, \_y, 和 \_z 的存储方式，接口抽象了数据的细节
* **抽象性：**Point3d 的设计将 “点” 作为一个抽象的数学概念，通过类封装具体的实现（如使用浮点数表示坐标）

> 将 struct Point3d 转换到 class Point3d 后，会有这样一个疑问：加上了封装之后，布局成本增加了多少？

**答案是：并没有增加成本**

* 三个 data members 直接内含在每一个 class object 之中，和 C struct 的情况一样
* member functions 虽然含在 class 的声明之中，却不出现在 object 之中。每一个 non-inline member function 只会诞生一个函数实例

{% hint style="info" %}
C++在布局以及存取时间上的主要的额外负担是由 virtual 引起的，包括：

* **virtual function 机制：**用以支持一个有效率的“执行期绑定” （也就是我们熟知的多态）
* **virtual base class ：**用以实现“多次出现在继承体系中的 base class，有一个单一而被共享的实例”。**（虚继承）**
{% endhint %}

