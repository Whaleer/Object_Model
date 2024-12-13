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

#### Plain Old Data（POD）声明形式

Plain Old Data（POD）声明形式是 C++ 中的一种数据结构或类的设计形式，强调数据的简单性和可预测性。它具有 C 语言风格的特性，并与 C 的数据模型保持兼容。POD 数据类型主要用于高效的数据存储、操作和跨语言的兼容性场景。

比如下面 Point 的声明

```cpp
typedef struct{
    float x, y, z;
}Point;
```



























































