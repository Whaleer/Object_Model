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

Virtual base class X subobject 的1 bytes 大小也出现在 class Y 和 Z 身上。传统上它被放在 derived class的固定（不变动）部分的尾端。

**某些编译器会对 empty virtual base class 提供特珠支持。**&#x4E66;上的计算并没有这个支持。

所以这里会计算 1 byte 的大小

3. **Alignment 的限制**

class Y 和 Z 的大小截至目前为5 bytes。在大部分 机器上，聚合的结构体大小会受到 alignment 的限制，使它们能够更有效率地在内存中被存取。在来信读者的机器上，alignment是 4 bytes，所以class Y 和Z必须填补3 bytes。最终得到的结果就是8bytes。

<figure><img src="../.gitbook/assets/image.png" alt=""><figcaption><p>书上展现的 X, Y, Z 的布局</p></figcaption></figure>

#### Empty virtual base class

在空基类被继承后，子类会优化掉基类的1字节的大小。

> **书上 A 的大小为什么为12？ 本地为什么为 16？**

**class A 的大小组成（书上）：**

* 共享的唯一一个 class X 实例，大小为 1 byte；
* Base class Y 的大小，[减去 <mark style="color:blue;">**“因 virtual base class X 而配置”**</mark> 的大小](#user-content-fn-1)[^1]，结果是 4 bytes；
* class A 自己的大小：0 byte；
* alignment。前面一共是 9 bytes， padding 3 bytes。最后就是 12 bytes。

**class A 的大小组成（本地）：**

本地 64 位机器，指针大小为 8 byte

class Y 的 vbptr + class Z 的 vbptr = 16 bytes

#### 一些总结：

1. static data members 被放置在程序的一个 global data segment 中，不会影响个别的 class object 的大小。
2. 每一个 class object 必须拥有足够大的大小以容纳它所有的 non-static data members, 它可能比想象的更大，原因是：
   1. 由编译器自动加上的额外的 data members（用于支持 virtual 特性）
   2. alignment 的需要

***

## 3.1 Data Member 的绑定



































[^1]: &#x20;这里存在遗留问题，为什么要减去？
