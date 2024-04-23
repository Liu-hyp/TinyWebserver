#ifndef THREAD_H
#define THREAD_H
#include <memory>
class Thread
{
public:
    Thread();
    ~Thread();
    pid_t Tid() const { return tid; }
private:
    pid_t tid;
    std::string name;
    bool started;
    bool joined;
    pthread_t pthreadId;
};
#endif