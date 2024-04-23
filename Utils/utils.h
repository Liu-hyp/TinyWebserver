#include "timer.h"

class Utils
{
public:
    Utils();
    ~Utils();

    void init(int timeslot);
    //对文件描述符设置非阻塞模式，如果要用epoll的ET模式，必须要用非阻塞
    int setfd_nonblock(int fd);   
    //epoll实例添加节点
    int addfd_onepoll(int epfd, int fd, bool oneshot, int TRIGMODE);
    //任务发生改变时，调整定时器时间
    void adjust_timer(util_timer *timer);


public:
    int m_TIMESLOT; //最小时间间隙
    sort_timer_lst m_timer_lst; //升序链表定时器
};