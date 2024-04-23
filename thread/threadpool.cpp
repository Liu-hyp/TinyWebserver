#include "threadpool.h"

void Threadpool::Run(Task task)
{
    if(m_threads.empty())
    {
        task();
    }
    else
    {
        MutexLockGuard guard(m_mutex);
        while(QueueIsFull() && running)
        {           
        }

    }
}