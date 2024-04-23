#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<vector>
#include<deque>
#include<pthread.h>
#include<string>
#include<functional>
#include "mutex.h"
#include "thread.h"
#include "./Utils/uncopyable.h"
/*
有了任务队列 在程序运行之初 就预开启多个执行任务的线程 
等待任务队列中传来的元素 对元素进行处理 
执行完毕后 也不销毁线程 而是等待下个元素进行处理 
*/
class Threadpool : public uncopyable
{
public:
    explicit Threadpool(const std::string& name = std::string("Threadpool"));
    ~Threadpool();

    using Task = std::function<void ()>; //传递的元素类型为一个返回类型为void的无输入参数函数
    //每当队列中添加该元素Task时，则有线程从队伍中取出执行

    void Start(int m_thread_number);
    void Stop();
    //Stop()后应销毁，不可再调用Start()

    const std::string& Name() const {return m_name;}
    int QueueSize() const;

    //内部未加锁，必须在Start()之前调用
    void SetInitCallback(const Task& cb);
    //内部未加锁，必须在Start()之前调用
    void SetMaxQueueSize(int size);
    // 以下两种情况时返回false:
    // 1. maxQueueSize_小于等于0, 此时相当于任务队列不设上限;
    // 2. maxQueueSize_大于0, 且队列大小不超过maxQueueSize_.
    bool QueueIsFull() const;
    // 向任务队列中放置任务. 队列满时会阻塞.
    // 如果在Start()之前调用, 会由主线程调用task.
    void Run(Task task);
    
private:
    Task Take(); // 从任务队列中取出任务. 队列空时会阻塞.
    void RunInThread();
    mutable MutexLock m_mutex; //任务队列互斥锁
    std::string m_name;
    Task threadInitCallback;
    int m_thread_number; //线程池中的线程数目
    int m_max_requests; //请求队列中的允许的最大请求数
    std::vector<std::shared_ptr<Thread>>m_threads;//消费者线程
    std::deque<Task>m_queue;
    bool running;

};
#endif

