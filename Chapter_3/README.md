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

<figure><img src="../.gitbook/assets/image (3).png" alt=""><figcaption><p>书上展现的 X, Y, Z 的布局</p></figcaption></figure>

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

这一节的内容暂且略过。



## 3.2 Data Member 的布局

来看下面这组 data members:

```cpp
class Point3d{
public:
...
private:
    float x;
    static List<Point3d*> * freeList;
    float y;
    static const int chunkSize = 250;
    float z;
};
```

non-static data members 在 class object 中的排列顺序将和其被声明的顺序一样，任何中间介入的 static data members 都不会被放进对象布局之中。

C++ Standard 要求，在同一个 access section（也就是 private、public、protected 等区段）中，members 的排列只需符合 <mark style="background-color:blue;">**“较晚出现的 members 在 class object 中有较高的地址”**</mark> 这一条件即可。

编译器还可能会合成一些内部使用的 data members，以支持整个对象模型。vptr 就是这样的东西，目前所有的编译器都把它安插在每一个 “内含 virtual function 之class” 的object内。

**vptr会被放在什么位置呢？**

* 传统上它被放在所有显式声明的members 的最后。
* **如今也有一些编译器把 vptr 放在一个 class object 的最前端。**&#x20;

***

## 3.3 Data Member 的存取

```cpp
Point3d origin, *pt = &origin;
origin.x = 0.0;
pt->x = 0.0;
```

下面会分析这两种不同的存取方式的差异。

### Static Data Members

每一个 static data member 只有一个实例，存放在程序的 data segment 之中。每次程序参阅（取用）static member 时，就会被内部转化为对该唯一extern 实例的直接参考操作。

若取一个 static data member 的地址，会得到一个指向其数据类型的指针，而不是一个指向其 class member 指针，因为 static member 并不内含在一个 class object 之中。

### Non-static Data Members

Non-static Data Members 直接存放在每一个 class object 之中。

除非经由 **显式的(explicit)** 或 **隐式的(implicit)** class object，否则没有办法直接存取它们。

只要程序员在一个 member function 中直接处理一个 non-static data member, 所谓 “implicit class object” 就会发生：

```cpp
Point3d Point3d::translate(const Point3d &pt){
    x += pt.x;
    y += pt.y;
    z += pt.z;
}
```

表面上所看到的对于 x、y、z 的直接存取，事实上是经由一个 <mark style="background-color:blue;">**“implicit class object”（由this 指针表达）**</mark>完成的。事实上这个函数的参数是：

```cpp
// member function 的内部转化 
Point3d Point3d::translate( Point3d *const this, const Point3d &pt){
    this->x += pt.x; 
    this->y += pt.y; 
    this->z += pt.z; 
}
```

欲对一个 non-static data member 进程存取操作，编译器需要把 class object 的起始地址加上 data member 的偏移位置(offset)。 举个例子，如果：

```cpp
origin._y = 0.0;
```

那么地址 `&origin._y` 将等于：

```cpp
&origin + (&Point3d::_y - 1);
```

指向 data member 的指针，**其 offset 总是加上 1，**&#x8FD9;样可以使编译器区分出下面两种情况：

* 一个指向 data member 的指针，用于指出 class 的第一个 member
* 一个指向 data member 的指针，没有指出任何 member

***

## 3.4 "继承" 与 Data Member

在 C++继承模型中，一个 derived class object 所表现出来的东西，是其自己的 members 加上其 base class（es） members 的总和。

derived class members 和 base class（es） members 的排列顺序，则并未在 C++ Standard 中强制指定；理论上编译器可以自由安排之。**在大部分编译器上头，base class members 总是先出现，但属于virtual base class 的除外（一般而言，任何一条通则一旦碰上 virtual base class 就没辙了，这里亦不例外）。**

来看下面两个抽象类

```cpp
class Point2d{
public:
    // constructors
    // operations
    // access functions
private:
    float x, y;
};

class Point3d{
public:
    // constructors
    // operations
    // access functions
private:
    float x, y, z;
};
```

**下面将讨论四种不同的情况：**

### 1. 单一继承且不含 virtual function

我们或许希望，无论是2D还是3D坐标点，都能够共享同一个实例，但又能够继续使用 “与类型性质相关” 的实例。

一个设计策略：就是从 Point2 派生出一个 Point3d

一般而言，**具体继承**相对于**虚拟继承**并不会增加空间或存取时间上的额外负担

```cpp
class Point2d{
public:
    Point2d(float x = 0.0, float y = 0.0):_x(x),_y(y){}
    float x() {return _x;}
    float y() {return _y;}

    void x(float newX) {_x = newX;}
    void y(float newY) {_y = newY;}

    void operator+=(const Point2d & rhs){
        _x += rhs._x;
        _y += rhs._y;
    }
protected:
    float _x,_y;
};

class Point3d: public Point2d{
public:
    Point3d(float x = 0.0, float y = 0.0, float z = 0.0):Point2d(x,y),_z(z){}

    float z() {return _z;}
    void z(float newZ) {_z = newZ;}

    void operator+=(const Point3d & rhs){
        Point2d::operator+=(rhs);
        _z += rhs._z;
    }
protected:
    float _z;
};
```

**这样设计的两个好处：**

* 可以把管理 x 和 y 坐标的程序代码局部化
* 可以明显表现出两个抽象类之间的紧密关系

<figure><img src="../.gitbook/assets/image (1) (1).png" alt=""><figcaption><p>单一继承，没有 virtual function 的布局</p></figcaption></figure>

**把两个原本独立不相干的 classes 凑成一对 “type/subtype”，并带有继承关系，会有什么易犯的错误呢**

* 经验不足的人可能会重复设计一些相同操作的函数。以我们例子中的 constructor 和 operator+=为例，它们并没有被做成 inline 函数（也可能是编译器为了某些理由没有支持 inline member functions）。Point3d object 的初始化操作或加法操作，将需要部分的 Point2d object 和部分的 Point3d object 作为成本。一般而言，选择某些函数做成inline 函数，是设计 class 时的一个重要课题。
* &#x20;第二个易犯的错误是，把一个 class 分解为两层或更多层，有可能会为了 “表现 class 体系之抽象化” 而膨胀所需的空间。C++语言保证 **“出现在 derived class中的 base class subobjeet 有其完整原样性”**，正是重点所在。

来看一个具体的例子：

```cpp
class Concrete {
public:
private:
    int val;
    char c1;
    char c2;
    char c3;
};

cout << sizeof(Concrete) << endl; // 8
```

每一个 Concrete class object 的大小都是 8 bytes：

* val 占用 4 bytes；
* c1, c2, c3 分别占用 1 byte；
* alignment 需要 1 byte；

现在把 Concrete 分裂成三层结构：

```cpp
class Concrete1{
public:
private:
    int val;
    char bit1;
};

class Concrete2: public Concrete1{
public:
private:
    char bit2;
};

class Concrete3: public Concrete2{
public:
private:
    char bit3;
};

cout << sizeof(Concrete1) << endl; // 8
cout << sizeof(Concrete2) << endl; // 8
cout << sizeof(Concrete3) << endl; // 8
```

**书上 Concrete1，Concrete2，Concrete3 大小分别为 8, 12, 16 bytes**

**Concrete1 很简单：**&#x76;al 和 bit1 加起来是 5 bytes，padding 为 3 bytes。

**Concrete2：**&#x62;it2 实际上是被放在 padding 之后的，于是大小为 12 bytes。

**Concrete3：**&#x4EA6;是如此，大小为 16 bytes。

<figure><img src="../.gitbook/assets/image (1).png" alt=""><figcaption><p><strong>Concrete1，Concrete2，Concrete3 对象布局</strong></p></figcaption></figure>

现在声明以下一组指针：

```cpp
Concrete2 *pc2;
Concrete1 *pc1_1, *pc1_2;

*pc1_2 = *pc1_1;
```

`*pc1_2 = *pc1_1;`应该执行一个**默认的 memberwise 复制操作**。如果 pc1\_1 实际指向一个 Concrete2 object 或 Concrete3 object, 则上述操作应该将复制内容指定给其 Concrete1 subobject。

<figure><img src="../.gitbook/assets/image (2).png" alt=""><figcaption><p><strong>如果不维持 base class subobject 的原样性，会发生什么？</strong></p></figcaption></figure>

**接下来，我们分析为什么本地三个类的大小都是8？**

```bash
(lldb) print &temp.val
(int *) 0x000000016fdff158
(lldb) print &temp.bit1
(char *) 0x000000016fdff15c "\U00000001"
(lldb) print &temp.bit2
(char *) 0x000000016fdff160 ""
(lldb) print &temp.bit3
(char *) 0x000000016fdff15e ""
```

`temp.val` 的地址是 0x000000016fdff158

输出的 (int \*) 表示调试器将该地址视为一个整数类型的指针：这意味着从这个地址开始，接下来的4个字节都属于 `temp.val`

我们将 `temp.bit1` 的地址与 `temp.val` 的地址相减：结果为 4。正好是 int 所占的大小



### 2. 单一继承并含 virtual function









































[^1]: &#x20;这里存在遗留问题，为什么要减去？
