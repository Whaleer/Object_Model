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
Point global;

Point foobar()
{
    Point local;
    Point * heap = new Point;
    *heap = local;
    // ...
    delete heap;
    return local;
}
```
{% endcode %}

#### Plain Old Data（POD）声明形式

Plain Old Data（POD）声明形式是 C++ 中的一种数据结构或类的设计形式，强调数据的简单性和可预测性。它具有 C 语言风格的特性，并与 C 的数据模型保持兼容。POD 数据类型主要用于高效的数据存储、操作和跨语言的兼容性场景。

比如下面 Point 的声明

```cpp
typedef struct{
    float x, y, z;
}Point;
```

> 编译器什么时候会为一个 class 声明所有相关的 trivial constructor ， 什么时候会为其贴上 POD 的标签呢？





当遇到这个定义时：`Point global;`

* C语言中：global 被视为一个 “临时性定义”，因为它没有显式的初始化操作。留下的实例存放在 BSS（Block Started By Symbol）
* C++：并不支持“临时性的定义”，这是因为 class 构造行为的隐式应用之固

<mark style="background-color:red;">**C 和 C++ 的一个差异就在于：BSS data segment 在 C++ 中相对不重要**</mark>

C++ 的所有全局对象都被以 “初始化过的数据” 来对待







```cpp
// L6 的初始化操作
Point *heap = new Point;
// 转化为
Point *heap = __new (sizeof(Point));
```

### 抽象数据类型



















































