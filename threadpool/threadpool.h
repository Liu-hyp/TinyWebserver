#ifndef TINYWEBSERVER_THREADPOOL_H
#define TINYWEBSERVER_THREADPOOL_H
#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <exception>
#include <pthread.h>
#include <vector>
#include <functional>
#include "../Imysql/sql_connection_pool.h"
#include "../log/block_queue.h"
class threadpool
{
public:
    /*thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量*/
    //threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_request = 10000);
    threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests) : m_actor_model(actor_model),m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL),m_connPool(connPool),stop(false)
    {
        //TODO: 确定线程数和请求队列的大小
        if(thread_number <= 0 || max_requests <= 0)
            throw std::exception();
        for(int i = 0; i < thread_number; i++)
        {
            m_threads.emplace_back([this]{
                while(1)
                {
                    std::unique_lock<std::mutex>locker(m_queuelocker);
                    condition.wait(locker, [this]{
                        return !tasks.empty() || stop;
                    });
                    if(stop && tasks.empty())return;
                    std::function<void()>task(std::move(tasks.front()));
                    tasks.pop();
                    locker.unlock();
                    task();               
                }
            });
            //m_threads.back().detach(); //分离线程
        }
    }   
    ~threadpool()
    {
        {
            std::unique_lock<std::mutex> locker(m_queuelocker);
            stop = true;
        }
        condition.notify_all(); //通知所有线程工作，把任务队列把所有任务取完
        for(auto& t : m_threads)
        {
            t.join();
        }
    }
    //bool append(T *request, int state);
    //bool append_p(T *request);
    template<class F, class... Args>
    bool enqueue(F&& f, Args&&...args)
    {
        std::function<void()>task = 
            std::bind(std::forward<F>(f), std::forward<Args>(args)...); //bind把函数和函数参数绑定在一起，forward实现完美转发，左值就转换成左值引用，右值就转换成右值引用       
        {
            std::unique_lock<std::mutex>locker(m_queuelocker);
            if(tasks.size() >= m_max_requests)return false;
            tasks.emplace(std::move(task));       
        }
        condition.notify_one();
        return true;
    }

//private:
    /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    //static void *worker(void *arg);
    //void run();

private:
    
    int m_thread_number;        //线程池中的线程数
    int m_max_requests;         //请求队列中允许的最大请求数
    std::vector<std::thread>m_threads;  //描述线程池的数组，其大小为m_thread_number
    std::queue<std::function<void()>>tasks; //任务队列
    std::mutex m_queuelocker;       //保护请求队列的互斥锁
    std::condition_variable condition;
    connection_pool *m_connPool;  //数据库
    int m_actor_model;          //模型切换
    bool stop;
};


//template <typename T>
//reactor模式下的请求入队
// bool threadpool<T>::append(T *request, int state)
// {
//     //TODO: 加锁，防止请求队列被修改
//     {
//         std::lock_guard<std::mutex>locker(m_queuelocker);
//         if(m_workqueue.size() >= m_max_requests)return false;
//         //读写事件
//         request->m_state = state;
//         m_workqueue.push_back(request);
//     }
    
    
// }
// template <typename T>
// bool threadpool<T>::append_p(T *request)
// {
//     //TODO: 加锁
// }
// template <typename T>
// void *threadpool<T>::worker(void *arg)
// {
//     //TODO: 运行
// }

//1.进入一个无限循环，这个循环会一直运行，直到线程被外部中断。
//2.调用函数阻塞当前线程，直到有新的任务被添加到工作队列中。
//3.调用函数来加锁，防止其他线程同时修改工作队列。
//4.检查工作队列是否为空，如果为空，则解锁并继续下一次循环。
//5.从工作队列的前端取出一个任务，并从队列中移除。
//6.解锁，允许其他线程修改工作队列。
//7.检查取出的任务是否为空，如果为空，则继续下一次循环。
//8.决定如何处理任务
//9.在Reactor模式下，根据任务来决定是读取数据还是写入数据。
//10.在Proactor模式下，直接处理任务
// template <typename T>
// void threadpool<T>::run()
// {
//     //TODO: 实现
// }

#endif //TINYWEBSERVER_THREADPOOL_H
