//
// Created by Nidhogg on 2024/5/28.
//

#ifndef TINYWEBSERVER_TIMER_H
#define TINYWEBSERVER_TIMER_H


#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <queue>
#include <time.h>
#include <memory>
#include "../log/log.h"

class util_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    util_timer *timer;
};
class util_timer
{
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    //超时时间
    time_t expire;
    //回调函数：从内核事件表删除事件，关闭文件描述符，释放连接资源
    void (* cb_func)(client_data *);
    //连接资源
    client_data *user_data;
    //前后向定时器
    util_timer *prev;
    util_timer *next;
};
class TimeCmp
{
public:
    bool operator()(util_timer* a, util_timer* b)
    {
        return a->expire > b->expire;
    }
};
class sort_timer_heap
{
public:   
    void push(util_timer* x){Q.push(x);}
    void erase(util_timer* x){D.push(x);}
    bool empty()
    {
        return Q.empty();
    }
    util_timer* top()
    {
        while(!D.empty() && (Q.top() == D.top()))
        {
            Q.pop();
            D.pop();
        }
        return Q.empty() ? nullptr : Q.top();
    }
    int size() const
    {
        return Q.size() - D.size();
    }
private:
    std::priority_queue<util_timer*, vector<util_timer*>, TimeCmp>Q,D;
};
class sort_timer_lst
{
public:
    sort_timer_lst();
    ~sort_timer_lst();

    void add_timer(util_timer *timer);
    void adjust_timer(util_timer *timer);
    void del_timer(util_timer *timer);
    void tick();

private:
    void add_timer(util_timer *timer, util_timer *lst_head);
    //优先队列维护定时器,小顶堆
    sort_timer_heap priority_timer_queue;
    //util_timer *head;
    //util_timer *tail;
};

class Utils
{
public:
    Utils() {}
    ~Utils() {}

    void init(int timeslot);

    //对文件描述符设置非阻塞
    int setnonblocking(int fd);

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

    //信号处理函数
    static void sig_handler(int sig);

    //设置信号函数
    void addsig(int sig, void(handler)(int), bool restart = true);

    //定时处理任务，重新定时以不断触发SIGALRM信号
    void timer_handler();

    void show_error(int connfd, const char *info);

public:
    static int *u_pipefd;//管道id
    sort_timer_lst m_timer_lst;
    static int u_epollfd;
    int m_TIMESLOT;
};

void cb_func(client_data *user_data);

#endif //TINYWEBSERVER_TIMER_H
