//
// Created by Nidhogg on 2024/5/28.
//

#ifndef TINYWEBSERVER_LOCK_H
#define TINYWEBSERVER_LOCK_H
#include <exception>
#include <pthread.h>
#include <semaphore.h>

//信号量
class sem
{
public:
    sem()
    {
        //TODO::初始化信号量
    }
    sem(int num)
    {
        //TODO::初始化信号量
    }
    ~sem()
    {
        //TODO::销毁信号量
    }
    bool wait()
    {
        //TODO::等待信号量
        return false;
    }
    bool post()
    {
        //TODO::增加信号量
        return false;
    }

private:
    sem_t m_sem;
};


class locker
{
public:
    locker()
    {
        //TODO::初始化锁
    }
    ~locker()
    {
        //TODO::销毁锁
    }
    bool lock()
    {
        //TODO::加锁
        return false;
    }
    bool unlock()
    {
        //TODO::解锁
        return false;
    }
    pthread_mutex_t *get()
    {
        //TODO::获取锁
        return new pthread_mutex_t;
    }

private:
    pthread_mutex_t m_mutex;
};
class cond
{
public:
    cond()
    {
       //TODO::初始化条件变量
    }
    ~cond()
    {
        //TODO::销毁条件变量
    }
    bool wait(pthread_mutex_t *m_mutex)
    {
        //TODO::等待条件变量
        return false;
    }
    bool timewait(pthread_mutex_t *m_mutex, struct timespec t)
    {
        //TODO::等待条件变量
        return false;
    }
    bool signal()
    {
        //TODO::唤醒等待条件变量的线程
        return false;
    }
    bool broadcast()
    {
        //TODO::唤醒所有等待条件变量的线程
        return false;
    }

private:
    //static pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};
#endif //TINYWEBSERVER_LOCK_H
