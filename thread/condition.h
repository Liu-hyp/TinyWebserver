#ifndef CONDITION_H
#define CONDITION_H
#include <pthread.h>
#include <cstdint>
#include "mutex.h"
#include "./Utils/uncopyable.h"
#include "thread.h"
// 检查返回值是否为0.
#define MCHECK(ret) { decltype(ret) errnum = ret; assert(errnum == 0); }
// UNIX条件变量的封装类. 不可拷贝.
class Condition : public uncopyable {
 public:
  explicit Condition(MutexLock& curlock) : lock(curlock) {
    MCHECK(pthread_cond_init(&cond, nullptr));
  }

  ~Condition() {
    MCHECK(pthread_cond_destroy(&cond));
  }

  // 等待直到条件出现.
  void Wait() {
    assert(lock.LockedByThisThread(cur_tid));
    // 由于pthread_cond_wait会在内部解锁, 所以需要手动修改持有者
    lock.UnassignHolder(cur_tid);
    MCHECK(pthread_cond_wait(&cond, lock.GetPthreadMutex()));
    lock.AssignHolder(cur_tid);
  }

  // 等待microseconds微秒, 如果到期时条件还没有出现就返回true.
  bool WaitForSeconds(double seconds);

  // 至少唤醒一个线程.
  void Notify() {
    MCHECK(pthread_cond_signal(&cond));
  }

  // 唤醒所有线程.
  void NotifyAll() {
    MCHECK(pthread_cond_broadcast(&cond));
  }

 private:
  MutexLock& lock;
  pthread_cond_t cond;
  pid_t cur_tid;
};


#undef MCHECK
#endif