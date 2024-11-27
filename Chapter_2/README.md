---
description: The Semantics of Constructions
icon: flower
---

# Chapter 2：构造函数语意学

### 2.1 Default Constructor 的构造操作

> **编译器何时会生成默认构造函数？**

* 对于 class X, **如果没有任何 user-declared constructor, 那么会有一个 default constructor 被隐式（implicitly）声明出来。**
* 一个被隐式声明出来的 default constructor 将是一个 trivial（没啥用的）constructor。
* 如果类中存在用户定义的任何构造函数（无论是否带参数），编译器 **不会** 再生成默认构造函数。

下面分别讨论 nontrivial default constructor 的情况：

#### 1. “带有 Default Constructor” 的 Member Class Object

如果一个class没有任何的构造函数，但是它内含一个 **成员对象（member object），**&#x800C;这个成员对象有 default constructor，那么这个class的implicit default constructor 就是“nontrivial”，编译器需要为该class合成出一个default constructor。<mark style="color:blue;">**不过这个合成操作只有在constructor真正需要被调用时才会发生。**</mark>

> **于是出现了一个有趣的问题：在C++各个不同的编译模块中，编译器如何避免合成出多个 default constructor（比如说一个是为A.C文件合成，另一个是为B.C文件合成）呢？**

我们先来理解这个问题本身。看如下代码

{% code title="Animal.h" %}
```cpp
#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
class Animal {
public:
    std::string name; // 成员变量有默认构造函数
};

#endif // ANIMAL_H
```
{% endcode %}

{% code title="main.cpp" %}
```cpp
#include "Animal.h"
#include <iostream>

int main() {
    Animal a; // 使用隐式默认构造函数
    std::cout << a.name << std::endl; // `std::string` 默认构造函数被调用
    return 0;
}
```
{% endcode %}

{% code title="utils.cpp" %}
```cpp
#include "Animal.h"

void createAnimal() {
    Animal b; // 这里也需要隐式默认构造函数
}
```
{% endcode %}

**编译方式：**

```bash
g++ -c main.cpp -o main.o
g++ -c utils.cpp -o utils.o
g++ main.o utils.o -o app
```

当编译 `main.cpp` 和 `utils.cpp` 时，编译器在每个文件中都会尝试为 Animal 生成一个 隐式默认构造函数，但问题在于：

1\. 如果隐式默认构造函数不是 inline，它会在**每个编译单元**中生成一个完整的非内联定义。

2\. 最终链接时会有两个相同的 `Animal::Animal()` 定义，导致 **重复定义错误。**

**编译器的解决方式：**

解决方法是把合成的 default constructor、copy constructor、destructor、assignment copy operator 都以 inline 方式完成。

* **一个 inline函数有静态链接（static linkage），不会被文件以外者看到。**
* 如果函数太复杂，不适合做成 inline，就会合成出一个 explicit non-inline static 实例。

```cpp
class Foo {
public:
    Foo() {};
    Foo(int i) {};
private:
    int val;
    Foo *pnext;
};

class Bar {
public:
    Foo foo;
    char *str;
};

void foo_bar() {
    Bar bar; // Bar::foo 必须在此处初始化
}
```

被合成的 Bar 的 default constructor 内含必要的代码，能够调用 class Foo 的 default constructor 来处理 member object `Bar::Foo`, 但它并不产生任何代码来初始化 `Bar::str`。

* 将 `Bar::foo` 初始化是编译器的责任
* 将 `Bar::str` 初始化是程序员的责任

被合成的 default constructor 看起来可能是这样的：

```cpp
inline Bar::Bar(){
    // 伪代码
    foo.Foo::Foo();
}
```

{% hint style="info" %}
注意：**被合成的 default constructor 只满足编译器的需要，而不是程序的需要。**&#x4E3A;了让这个程序片段能够正确执行，字符指针 str 也需要被初始化。
{% endhint %}

现在，我们假设程序员已经写了构造函数，如下：

```cpp
Bar::Bar(){
    str = 0;
}
```

此时，str 被初始化了，但是编译器还需要初始化 member object foo。但是程序员实现的 constructor 并没有初始化 foo 的操作。此时编译器会怎么做呢？

编译器的行动如下：

<mark style="background-color:blue;">**编译器会扩张已经存在的 constructors, 在其中安插一些代码，使得 user code 执行之前，先调用必要的 default constructors。**</mark>延续前面的例子，扩张后的 constructors 可能是如下的样子：

```cpp
Bar::Bar(){
    foo.Foo::Foo(); // 附加上的 compiler code
    str = 0;        // explicit user code
}
```

> **如果有多个 class member objects 都要求 constructor 初始化操作，将如何？**

C++要求以 member objects 在 class 中的声明顺序来调用各个 constructors。

#### 2. “带有 default constructor” 的Base Class

如果一个**没有任何 constructors 的 class 派生自一个 "带有default constructors" 的 base class**， 那么这个 derived class 的 default constructor 会被视为 nontrivial, 并因此需要被合成出来。它将调用上一层 base classes 的 default constructor 。

#### 3. “带有一个 Virtual Function” 的 Class

另有两种情况，也需要合成出 default constructor:

1. class 声明（或继承）一个 virtual constructor。
2. class 派生自一个















