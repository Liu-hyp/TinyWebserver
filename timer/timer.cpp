//
// Created by Nidhogg on 2024/5/28.
//

#include "timer.h"
#include "../http/http_connect.h"
//extern const int TIMESLOT;
const int TIMESLOT = 5;             //最小超时单位
sort_timer_lst::sort_timer_lst()
{
    //TODO::初始化定时器
}
sort_timer_lst::~sort_timer_lst()
{
    //TODO::释放定时器
}

void sort_timer_lst::add_timer(util_timer *timer)
{
    //TODO::添加定时器,注意节点存在判断
    this->priority_timer_queue.push(timer);
    cout << "当前定时器有 " << this->priority_timer_queue.size() << endl; 
}
void sort_timer_lst::adjust_timer(util_timer *timer)
{
    util_timer* newtimer = timer;
    time_t cur = time(NULL);
    newtimer->expire = cur + 3 * TIMESLOT;
    this->del_timer(timer);
    this->add_timer(newtimer);
}
void sort_timer_lst::del_timer(util_timer *timer)
{
    //TODO::删除定时器
    this->priority_timer_queue.erase(timer);
}
void sort_timer_lst::tick()
{
    //TODO::处理定时任务
}

void sort_timer_lst::add_timer(util_timer *timer, util_timer *lst_head)
{
    //TODO::添加定时器
}

void Utils::init(int timeslot)
{
    m_TIMESLOT = timeslot;
}

//对文件描述符设置非阻塞
int Utils::setnonblocking(int fd)
{
    //TODO::设置非阻塞
    //fcntl可以获取/设置文件描述符性质
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Utils::addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    //TODO::注册读事件,ET模式，选择开启EPOLLONESHOT
    //LT（水平触发）ET（边沿触发）
    //EPOLLONESHOT：使得同一个socket在使用epoll_ctl重置文件描述符上的事件前，
    //不会再注册事件，保证同一个socket只能被一个线程处理，不会跨越多个线程
    //如果对描述符socket注册了EPOLLONESHOT事件，
    //那么操作系统最多触发其上注册的一个可读、可写或者异常事件，且只触发一次。
    epoll_event event;
    event.data.fd = fd;
    if(TRIGMode == 1)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;
    if(one_shot)
        event.events |= EPOLLONESHOT;
    //添加事件
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    //设置文件描述符非阻塞
    this->setnonblocking(fd);


}

//信号处理函数
void Utils::sig_handler(int sig)
{
    //为保证函数的可重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char*)&msg, 1, 0);
    errno = save_errno;
    //TODO::处理信号
}

void Utils::addsig(int sig, void(handler)(int), bool restart)
{
    //TODO::设置信号函数
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));

    sa.sa_handler = handler;
    if(restart)
        sa.sa_flags |= SA_RESTART;
    //初始化一个信号集，包含所有可接受的信号
    sigset_t signal_set;
    int ret = sigfillset(&signal_set);
    assert(ret != -1);
    sa.sa_mask = signal_set;

    //注册信号处理程序
    ret = sigaction(sig, &sa, NULL);
    assert(ret != -1);
}

void Utils::timer_handler()
{
    //TODO::处理定时任务,重新定时以不断触发SIGALRM信号
}

void Utils::show_error(int connfd, const char *info)
{
    //TODO::显示错误信息
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int *Utils::u_pipefd = 0;
int Utils::u_epollfd = 0;

class Utils;
void cb_func(client_data *user_data)
{
    //回调函数
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
}
