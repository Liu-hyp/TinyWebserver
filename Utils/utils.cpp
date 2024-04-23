#include"utils.h"

void Utils::init(int timeslot)
{
    m_TIMESLOT = timeslot;
}
int Utils::setfd_nonblock(int fd)
{
    int old_flag = fcntl(fd, F_GETFL);
    assert(old_flag >= 0);
    int new_flag = fcntl(fd, F_SETFL, old_flag | O_NONBLOCK);   
    return new_flag;
}
int Utils::addfd_onepoll(int epfd, int fd, bool oneshot, int TRIGMODE)
{
    //为描述符注册事件
    epoll_event ev;
    ev.data.fd = fd;
    if(TRIGMODE == 1)
    {
        //EPOLLRDHUP 搭配epoll ET模式，使得检测对方是否断开连接少调用一次recv
        ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
    }
    else
    {
        ev.events = EPOLLIN | EPOLLRDHUP;
    }
    //采用oneshot事件，防止多线程处理同一个socket的事件
    if(oneshot)
    {
        ev.events |= EPOLLONESHOT;
    }
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}
//重置描述符的oneshot事件
void reset_oneshot(int epfd, int fd, int TRIGMODE)
{
    epoll_event ev;
    ev.data.fd = fd;
    if(TRIGMODE == 1)
    {
        //EPOLLRDHUP 搭配epoll ET模式，使得检测对方是否断开连接少调用一次recv
        ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
    }
    else
    {
        ev.events = EPOLLIN | EPOLLRDHUP;
    }
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void Utils::adjust_timer(util_timer *timer)
{
    time_t cur = time(NULL);
    timer->expire = cur + 3 * m_TIMESLOT;
    m_timer_lst.adjust_timer(timer);
}
