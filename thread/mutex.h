#ifndef SRC_THREAD_MUTEX_H_
#define SRC_THREAD_MUTEX_H_
#include <pthread.h>
#include <cassert>
#include "./Utils/uncopyable.h"

#define MCHECK(ret) { decltype(ret) errnum = ret; assert(errnum == 0);}
//UNIX互斥量的封装，不可拷贝
class MutexLock : public uncopyable
{
public:
    MutexLock() : mutex_holder(0)
    {
        MCHECK(pthread_mutex_init(&mutex_, nullptr));
    }
    ~MutexLock()
    {
        MCHECK(mutex_holder);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }
    void Lock()
    {
        MCHECK(pthread_mutex_lock(&mutex_));
    }
    void Unlock()
    {
        MCHECK(pthread_mutex_unlock(&mutex_));
    }
    bool LockedByThisThread(int &cur_tid) { return mutex_holder == cur_tid; }
    void AssignHolder(int &cur_tid) { mutex_holder = cur_tid;}
    void UnassignHolder(int &cur_tid) { mutex_holder = 0;}
    pthread_mutex_t* GetPthreadMutex() {return &mutex_;}
private:
    pthread_mutex_t mutex_;
    pid_t mutex_holder; //锁持有者的线程id

};
// MutexLock的RAII包装.
class MutexLockGuard : public uncopyable {
 public:
  explicit MutexLockGuard(MutexLock& lock) : lock_(lock) { lock_.Lock(); }
  ~MutexLockGuard() { lock_.Unlock(); }

 private:
  MutexLock& lock_;
};
#undef MCHECK

// 错误用法 MutexLockGuard(lock);
// 正确的用法是 MutexLockGuard guard(lock);
#define MutexLockGuard(x) wrong_usage!
#endif //MUTEX_H