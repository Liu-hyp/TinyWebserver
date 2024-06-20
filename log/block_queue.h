//
// Created by Nidhogg on 2024/5/28.
//
/*************************************************************
*deque实现的阻塞队列
*线程安全，每个操作前都要先加互斥锁，操作完后，再解锁
**************************************************************/
#ifndef TINYWEBSERVER_BLOCK_QUEUE_H
#define TINYWEBSERVER_BLOCK_QUEUE_H
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

//TODO::阻塞队列
template <class T>
class block_queue
{
public:
    explicit block_queue(size_t maxCapacity = 1024);
    ~block_queue();
    void close();
    void flush();
    void clear();
    bool front(T &value);
    bool back(T &value);
    int get_size() const;
    int get_capacity() const;
    void push_back(const T& item);
    void push_front(const T& item);
    bool empty();
    bool full();
    bool pop(T& item, int timeout); //增加超时处理
private:
    std::deque<T> deq;
    size_t m_capacity;
    std::mutex m_mutex;
    bool isClose; 
    std::condition_variable Consumer;
    std::condition_variable Producer;
      
};
template<typename T>
block_queue<T>::block_queue(size_t maxCapacity):m_capacity(maxCapacity)
{
    assert(maxCapacity > 0);
    isClose = false;
}
template<typename T>
block_queue<T>::~block_queue()
{
    close();
}
template<typename T>
void block_queue<T>::close()
{
    //清理队列中的所有成员，唤醒所有阻塞中的生产者、消费者线程
    {
        std::lock_guard<std::mutex>locker(m_mutex);
        deq.clear();
        isClose = true;
    }
    Producer.notify_all();
    Consumer.notify_all();
}
template<typename T>
void block_queue<T>::flush()
{
    Consumer.notify_one();
}
template<typename T>
void block_queue<T>::clear()
{
    std::lock_guard<std::mutex>locker(m_mutex);
    deq.clear();
}
template<typename T>
bool block_queue<T>::front(T &value)
{
    std::lock_guard<std::mutex>locker(m_mutex);
    if(deq.size() == 0)
        return false;
    value = deq.front();
    return true;
}
template<typename T>
bool block_queue<T>::back(T &value)
{
    std::lock_guard<std::mutex>locker(m_mutex);
    if(deq.size() == 0)
        return false;
    value = deq.back();
    return true;
}
template<typename T>
int block_queue<T>::get_size() const
{
    int tmp = 0;
    std::lock_guard<std::mutex>locker(m_mutex);
    tmp = deq.size();
    return tmp;
}
template<typename T>
int block_queue<T>::get_capacity() const
{
    int tmp = 0;
    std::lock_guard<std::mutex>locker(m_mutex);
    tmp = m_capacity;
    return tmp;
}
template<typename T>
void block_queue<T>::push_back(const T& item)
{
    std::unique_lock<std::mutex>locker(m_mutex);
    while(deq.size() >= m_capacity)
    {
        Producer.wait(locker);
    }
    deq.push_back(item);
    Consumer.notify_one();
}
template<typename T>
void block_queue<T>::push_front(const T& item)
{
    std::unique_lock<std::mutex>locker(m_mutex);
    while(deq.size() >= m_capacity)
    {
        Producer.wait(locker);
    }
    deq.push_front(item);
    Consumer.notify_one();
}
template<typename T>
bool block_queue<T>::empty()
{
    std::unique_lock<std::mutex>locker(m_mutex);
    return deq.emty();
}
template<typename T>
bool block_queue<T>::full()
{
    std::unique_lock<std::mutex>locker(m_mutex);
    return deq.size() >= m_capacity;
}
template<typename T>
bool block_queue<T>::pop(T& item, int timeout)
{
    std::unique_lock<std::mutex>locker(m_mutex);
    while(deq.empty())
    {
        if(Consumer.wait_for(locker, std::chrono::microseconds(timeout)) == std::cv_status::timeout)
            return false;
        if(isClose)
            return false;
    }
    item = deq.front();
    deq.pop_front();
    Producer.notify_one();
    return true;
}
#endif //TINYWEBSERVER_BLOCK_QUEUE_H
