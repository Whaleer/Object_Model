---
description: 关于对象（Object Lessons）
layout:
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

> **将 struct Point3d 转换到 class Point3d 后，会有这样一个疑问：加上了封装之后，布局成本增加了多少？**

**答案是：并没有增加成本**

* 三个 data members 直接内含在每一个 class object 之中，和 C struct 的情况一样
* member functions 虽然含在 class 的声明之中，却不出现在 object 之中。每一个 non-inline member function 只会诞生一个函数实例

{% hint style="info" %}
C++在布局以及存取时间上的主要的额外负担是由 virtual 引起的，包括：

* **virtual function 机制：**用以支持一个有效率的“执行期绑定” （也就是我们熟知的多态）
* **virtual base class ：**用以实现“多次出现在继承体系中的 base class，有一个单一而被共享的实例”。（虚继承）
{% endhint %}

### 1.3 C++ 对象模式

**两种 class data members ：**static 和 non-static

**三种 class member functions：**static, non-static, virtual

<mark style="color:blue;">**C++ 对象模型：**</mark>\
<mark style="color:blue;">- non-static data members 被置于每一个 class object 之内</mark>\ <mark style="color:blue;">- static data members 被存放在个别的 class object 之外</mark>\ <mark style="color:blue;">- static 和 non-static function members 也被放在个别的 class object 之外</mark>

这里的 **「存放在个别的 class object 之外」 的意思就是：**不属于任何单个类对象，而是属于整个类，并且所有该类的对象共享这份静态数据

<mark style="color:blue;">**C++ 对象模型中的 virtual function：**</mark>\
<mark style="color:blue;">- 每一个 class 一堆指向 virtual functions 的指针，放在称为</mark> <mark style="color:blue;"></mark><mark style="color:blue;">**virtual table （vtbl）**</mark><mark style="color:blue;">的表格中</mark>\ <mark style="color:blue;">- 每一个 class object 被安插一个指针，指向相关的 virtual table。这个指针被称为</mark> <mark style="color:blue;"></mark><mark style="color:blue;">**vptr。**</mark><mark style="color:blue;">vptr 的设定和重置都由每一个 class 的 constructor、destructor 和 copy assignment 运算符自动完成。 每一个 class 所关联的</mark> <mark style="color:blue;"></mark>_<mark style="color:blue;">type\_info</mark>_ <mark style="color:blue;"></mark><mark style="color:blue;">object (用以支持 runtime type identification， RTTI) 也经由 virtual table 被指出来，</mark><mark style="color:blue;">**通常放在表格的第一个 slot。**</mark>

<details>

<summary> 什么是 type_info object？</summary>

type\_info object 是 type\_info 类的一个实例，用于描述一个特定的类型。是**运行时类型识别（RTTI, Runtime Type Identification）** 的核心部分

</details>



