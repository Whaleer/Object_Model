---
icon: leaf-maple
description: The Semantics of Data
---

# Chapter 3：Data 语意学

先来看棱形继承的情况：

```cpp
class X{};
class Y:public virtual X{};
class Z:public virtual X{};
class A:public Y,public Z{};

int main(){
    cout << sizeof(X) << endl; // 1
    cout << sizeof(Y) << endl; // 8
    cout << sizeof(Z) << endl; // 8
    cout << sizeof(A) << endl; // 16
    return 0;
}
```

> **为什么 X 的大小为 1？**

对象的地址在程序中用于标识不同的实例，即使这些实例没有任何数据成员。**如果类X的大小为0字节，多个对象实例可能会占用同一个地址，导致程序行为不确定**（例如，`&x1 == &x2`，这会违背C++对对象地址唯一性的要求）。

因此，为了避免这种问题，编译器为空类分配1字节的空间。

> **Y 和 Z 的大小和什么有关？**

1. **语言本身所造成的额外负担（overhead）**

当发生了虚继承时，才会出现 <mark style="color:blue;">**「虚基类表指针（vbptr）」**</mark>**&#x20;，它指向:**

* 虚基类表（Virtual Base Table)
* 虚基类表中存放的是 virtual base class subobject 的地址

在64位系统上，指针大小是8字节

除了虚表指针，X 的大小（1 字节）仍然被保留，但它的存储不直接反映在 Y 和 Z 的对象布局中，因为虚基类的内存是由最下层的最终派生类 A 来共享的。

2. **编译器对于特殊情况所提供的优化处理**
3. **Alignment 的限制**

> **A 的大小为什么为16？**

























