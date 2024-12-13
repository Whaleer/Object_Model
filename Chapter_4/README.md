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

### Non-static Member Functions

C++ 设计准则之一： **non-static member function 至少必须和一般的 nonmember function 有相同的效率。**

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

`normalize()` 函数将被转化为：

```cpp
void normalize_7Point3dFv(register const Point3d *const this, Point3d & _result){
    register float mag = this->magnitude();
    
    _result.Point3d::Point3d();
    
    _result._x = this->_x/mag;
    _result._y = this->_y/mag;
    _result._z = this->_z/mag;
    
    return;
}
```



### Virtual Member Functions

如果 normalize() 是一个 virtual member function, 那么以下的调用：

```cpp
ptr -> normaliza();
```

将被内部转化为

```cpp
(* ptr -> vptr[1])(ptr)
```

* vptr 是编译器产生的指针，指向 virtual table
* 1 是 virtual table slot 的索引值，关联到 normaliza() 函数
* 第二个 ptr 表示 this 指针

类似的，如果 magnitude() 也是虚函数，转化如下

```cpp
// register float mag = magnitude();
register float mag = (*this->vptr[2])(this);
```



### Static Member Functions

如果 normalize() 是一个 static member function, 以下两个调用操作，将被转化为：

```cpp
obj.normalize()
// 转化为
normalize_7Point3dSFv();

ptr->normalize();
// 转化为
normalize_7Point3dSFv();
```

Static Member Functions 主要特性：

* 不能够直接存取其 class 中的 non-static members
* 不能够被声明为 const, volatile 或 virtual
* 不需要经由 class object 才被调用

## 4.2 Virtual Member Functions

为了支持 virtual function 机制，必须首先能够对于多态对象有某种形式的 “执行期类型判断算法”。

也就是说，下面的调用操作将需要 ptr 在执行期的某些相关信息:

```cpp
ptr -> z();
```

这样才能够找到并调用 `z()`的适当实例。这个额外的信息可以放在对象本身。

{% hint style="info" %}
C++ 中，多态表示：以一个 public base class 的指针或者引用寻址出一个 derived class object 的意思。
{% endhint %}

例如：

```cpp
Point *ptr;
```

我们可以指定 ptr 以寻址出一个 Point2d 对象，或者一个 Point3d 对象：

```cpp
ptr = new Point2d;
ptr = new Point3d;
```

<mark style="background-color:blue;">**识别一个 class 知否支持多态，唯一适当的方式就是看看它是否有任何 virtual function。只要 class 拥有一个 virtual function, 它就需要这份额外的执行期信息。**</mark>



什么样的额外信息是需要存储起来的？

```cpp
ptr -> z()
```

`z()`是一个 virtual function，什么信息才能让我们在执行期调用正确的 `z()` 实例？我们需要知道：

* ptr 所指对象的真实类型。这可使我们选择正确的 `z()` 实例。
* `z()` 实例的位置，以便我们可以调用它。

在实现上，首先我们可以在每一个多态的 class object 身上增加两个 members：

1. 一个字符串或数字，表示 class 的类型
2. 一个指针，指向某 table，table 中持有程序的 virtual functions 的执行地址

table 中的 virtual functions 地址是如何被构建起来的？

virtual functions 可以在编译时期获知。

* 为了找到 table，每一个 class object 被安插了一个由编译器内部产生的指针，指向该表格（vptr）
* 为了找到函数地址，每一个 virtual function 被指派一个表格索引值

这些工作都是由编译器完成的。**执行期间要做的，只是在特定的 virtual table slot 中激活 virtual function**

一个class 只会有一个 virtual table。**每一个 table 内含其对应之 class object 中所有 active virtual functions 函数实例的地址。这些 active virtual funtions 包括：**

* 这一 class 所定义的函数实例。它会 overriding 一个可能存在的 base class virtual function 函数实例
* 继承自 base class 的函数实例。这是在 derived class 决定不 override virtual function 时才会出现的情况
* 一个 `pure_virtual_called()` 函数实例，它既可以扮演 pure virtual function 的空间保卫者角色，也可以当作执行期异常处理函数。

**每一个 virtual function 都被指派一个固定的索引值，这个索引在整个继承体系中保持与特定的 virtual function 的关系。在 Point class 体系中：**

```cpp
class Point{
public:
    virtual ~Point();
    
    virtual Point& mult(float) = 0;
    
    float x() const {return _x;}
    virtual float y() const {return 0;}
    virtual float z() const {return 0;}
    
protected:
    Point(float x = 0.0);
    float _x;
};


class Point2d: public Point{
public:
    Point2d(float x = 0.0, float y = 0.0):Point(x),_y(y){}
    ~Point2d();
    
    // override base virtual functions
    Point2d& mult(float);
    float y() const {return _y;}
    
protected:
    float _y;
};

class Point3d: public Point2d{
public:
    Point3d(float x = 0.0, float y = 0.0, float z = 0.0):Point2d(x,y), _z(z){}
    ~Point3d();
    
    // override
    Point3d & mult(float);
    float z() const {return _z;}
    
protected:
    float _z;
};
```

* virtual destructor ：被指派 slot 1
* `mult()` ：被指派 slot 2 （它是纯虚函数：pure\_virtual\_function()）
* `y()`：被指派 slot 3
* `z()`：被指派 slot 4

`x()` 没有 slot，因为不是虚函数

**class Point2d 继承 Point 之后：**

1. 它可以继承 base class 所声明的 virtual functions 的函数实例。正确地说是，该函数的实例的地址会被拷贝到 derived class 的 virtual table 的相对应 slot 之中。
2. 它可以使用自己的函数实例，这表示它自己的函数实例地址必须放在对应的 slot 之中。
3. 它可以加入一个新的 virtual function。这时候 virtual table 的尺寸会增大一个 slot，而新的函数实例地址会被放进该 slot 之中。

Point2d 的 virtual table 在 slot 1 中指出 destructor, 而在 slot 2 中指出 `mult()`。它自己的 `y()` 函数实例地址放在 slot 3中，继承自 Point 的 `z()` 函数实例地址则被放在 slot 4 中。

**class Point3d 以此类推**

<figure><img src="../.gitbook/assets/image (11).png" alt=""><figcaption><p>4.1 Virtual table 布局</p></figcaption></figure>

那么现在，执行

```cpp
ptr -> z();
```

我如何有足够的知识在编译时期设定 virtual function 的调用呢？

* 一般而言，每次调用 `z()` 时，我并不知道 ptr 所指对象的真正类型。然而，我知道，经由 ptr 可以存取到该对象的 virtual table。
* 虽然不知道哪一个 `z()` 函数实例会被调用，但我知道每一个 `z()` 函数地址都被放在 slot 4 中。

a这些信息可以使得编译器可以将该调用转化为：

<pre class="language-cpp"><code class="lang-cpp"><strong>(*ptr -> vptr[4])(ptr)
</strong></code></pre>

唯一一个在执行期才能知道的东西是：slot 4 所指的到底是哪一个 `z()` 函数实例？

### 多重继承下的 Virtual Functions

```cpp
class Base1{
public:
    Base1();
    virtual ~Base1();
    virtual void speakClearly();
    virtual Base1 * clone() const;
protected:
    float data_Base1;
};

class Base2{
public:
    Base2();
    virtual ~Base2();
    virtual void mumble();
    virtual Base2 * clone() const;
protected:
    float data_Base2;
};

class Derived:public Base1,public Base2{
public:
    Derived();
    virtual ~Derived();
    virtual Derived * clone() const;
protected:
    char data_Derived;
};
```









































