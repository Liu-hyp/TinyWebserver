#include<unistd.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<signal.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>

//连接资源结构体成员需要用到定时器
//前向说明定时器类
class util_timer;
//定时器类：基于双向链表
class util_timer
{
public:
    //超时时间
    time_t expire;
    //前向定时器
    util_timer *prev;
    //后向定时器
    util_timer *next;

};

//定时器容器类：基于升序链表
class sort_timer_lst
{
public:
    sort_timer_lst();
    ~sort_timer_lst();
    //添加定时器，内部调用私有成员add_timer
    void add_timer(util_timer *timer);
    //删除定时器
    void del_timer(util_timer *timer);
    //调整定时器，任务发生变化时，调整定时器在链表中的位置
    void adjust_timer(util_timer *timer);
private:
    //私有成员，被公有成员调用
    void add_timer(util_timer *timer, util_timer *lst_head);
    //头尾结点
    util_timer *head;
    util_timer *tail;
};
