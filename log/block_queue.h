//
// Created by Nidhogg on 2024/5/28.
//

#ifndef TINYWEBSERVER_BLOCK_QUEUE_H
#define TINYWEBSERVER_BLOCK_QUEUE_H
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "../lock/locker.h"
using namespace std;

//TODO::阻塞队列
template <class T>
class block_queue
{
public:
    block_queue(int max_size = 1000)
    {
        //TODO::初始化
    }

    void clear()
    {
        //TODO::清空队列
    }

    ~block_queue()
    {
        //TODO::销毁队列
    }

    bool full()
    {
        //TODO::判断队列是否满
    }

    bool empty()
    {
        //TODO::判断队列是否空
    }
    bool front(T &value)
    {
       //TODO::返回队头元素
    }
    //返回队尾元素
    bool back(T &value)
    {
       //TODO::返回队尾元素
    }

    int size()
    {
        //TODO::返回队列大小
    }

    int max_size()
    {
        //TODO::返回队列最大容量
    }


    //往队列添加元素，需要将所有使用队列的线程先唤醒
    //当有元素push进队列,相当于生产者生产了一个元素
    //若当前没有线程等待条件变量,则唤醒无意义
    bool push(const T &item)
    {

    }


    //pop时,如果当前队列没有元素,将会等待条件变量
    bool pop(T &item)
    {

    }

    //增加了超时处理
    bool pop(T &item, int ms_timeout)
    {

    }

private:
    locker m_mutex;
    cond m_cond;

    T *m_array;
    int m_size;
    int m_max_size;
    int m_front;
    int m_back;
};
#endif //TINYWEBSERVER_BLOCK_QUEUE_H
