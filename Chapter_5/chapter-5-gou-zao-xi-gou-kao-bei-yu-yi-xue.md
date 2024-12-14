---
icon: cat
description: Semantics of Construction, Destruction, and Copy
---

# Chapter 5：构造, 析构, 拷贝语意学

看下面这个 abstract base class 声明

```cpp
class Abstract_base{
public:
    virtual ~Abstract_base() = 0;
    virtual void interface() const = 0;
    virtual const char * mumble() const {return _mumble;}
protected:
    char * _mumble;
};
```

存在的问题：

* 纯虚析构函数是允许的，但必须提供定义。
* 数据成员 mumble 未被初始化

修改如下：

```cpp
class Abstract_base{
public:
    virtual ~Abstract_base();
    virtual void interface() = 0;
    const char * mumble() const {return _mumble;}
protected:
    Abstract_base(char *pc = 0);
    char * _mumble;
};
```

## 5.1 "无继承" 情况下的对象构造

{% code lineNumbers="true" %}
```cpp
typedef struct {
    float x, y, z;
}Point;

Point global;

Point foobar(){
    Point local;
    Point *heap = new Point;
    *heap = local;

    delete heap;
    return local;
}

int main(){
    Point p = foobar();
    return 0;
}
```
{% endcode %}

先来看这段程序的执行顺序：

global 是全局变量，它的生命和整个程序的生命相同，所以global 会在程序启动时被分配内存并初始化为默认值。

由于 Point 是一个结构体，没有用户定义的构造函数，global 的 x, y, z 值会是未定义的（在某些编译器下可能被默认初始化为 0），我在本地打印看下

```cpp
cout << sizeof(Point) << endl; // 12
cout << global.x << endl;      // 0
cout << global.y << endl;      // 0
cout << global.z << endl;      // 0
```

因为 global 是一个全局变量，并且没有显式初始化，因此它的成员 x, y, z 会被初始化为 0.0

#### Plain Old Data（POD）声明形式

Plain Old Data（POD）声明形式是 C++ 中的一种数据结构或类的设计形式，强调数据的简单性和可预测性。它具有 C 语言风格的特性，并与 C 的数据模型保持兼容。POD 数据类型主要用于高效的数据存储、操作和跨语言的兼容性场景。

比如下面 Point 的声明

```cpp
typedef struct{
    float x, y, z;
}Point;
```

> 编译器什么时候会为一个 class 声明所有相关的 trivial constructor ， 什么时候会为其贴上 POD 的标签呢？

**C++11及更高版本中，POD 的概念被进一步分解为：**

**1. Trivial（平凡性）**

**2. Standard-layout（标准布局）**

一个类型如果既是 Trivial 又是 Standard-layout，则它是一个 POD 类型。

**标准布局 (standard-layout)：**

* 所有非静态成员具有相同的访问控制（如 public 或 private）。
* 没有虚基类。
* 没有包含非标准布局的基类。
* 非静态成员的排列与 C 结构体兼容。

**当遇到这个定义时：`Point global;`**

* C语言中：global 被视为一个 “临时性定义”，因为它没有显式的初始化操作。留下的实例存放在 BSS（Block Started By Symbol）
* C++：并不支持“临时性的定义”，这是因为 class 构造行为的隐式应用之固

<mark style="background-color:red;">**C 和 C++ 的一个差异就在于：BSS data segment 在 C++ 中相对不重要**</mark>

C++ 的所有全局对象都被以 “初始化过的数据” 来对待。

接下来继续看：

```cpp
// L6 的初始化操作
Point *heap = new Point;
// 转化为
Point *heap = __new (sizeof(Point));
```

### 抽象数据类型

下面是 Point 的第二次声明，在 public 接口下多了 private 数据，提供完整的封装性，但没有任何的虚函数

```cpp
class Point{
public:
    Point(float x = 0.0, float y = 0.0, float z = 0.0): _x(x), _y(y), _z(z) {}
private:
    float _x, _y, _z;
}
```

并没有为 Point 定义一个 copy constructor 或 copy operator, 因为默认的位语意(default bitwise semantics) 已经足够。也不需要提供 destructor, 因为默认的内存管理方法已经足够。

现在执行：

```
Point global;
```

**有了 default constructor。**<mark style="background-color:blue;">**由于 global 被定义在全局范围中，其初始化操作将延迟到程序启动时才开始**</mark>















































