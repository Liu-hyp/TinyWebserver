#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H
/*
基本思想是： 把构造函数和析构函数设置为protected权限，这样子类可以调用，外部类不可以
*/
class uncopyable
{
public:
    uncopyable(const uncopyable&) = delete; //禁止类的拷贝构造
    void operator=(const uncopyable&) = delete;
protected:
    uncopyable() = default;
    ~uncopyable() = default;
};


#endif