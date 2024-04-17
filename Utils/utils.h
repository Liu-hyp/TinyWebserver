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


public:
    int m_TIMESLOT; //最小时间间隙
};