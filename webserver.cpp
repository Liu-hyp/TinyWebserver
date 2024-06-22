#include "webserver.h"
webserver::webserver()
{
    //TODO::创建一个http_conn对象
    users = new http_conn[MAX_FD];
    //root文件夹路径
    char server_path[200];
    getcwd(server_path, 200);
    char root[6] = "/root";
    m_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(m_root, server_path);
    strcat(m_root, root);
    m_port = 8080;

    //TODO::创建定时器
    users_timer = new client_data[MAX_FD];
}

webserver::~webserver()
{
    //TODO::释放服务器
}

void init(int port , std::string user, std::string passWord, std::string databaseName,
          int log_write , int opt_linger, int trigmode, int sql_num,
          int thread_num, int close_log, int actor_model)
{
    //TODO::初始化服务器
}

//如果m_TRIGMode的值为0，那么m_LISTENTrigmode和m_CONNTrigmode都被设置为0，表示LT + LT模式。
//如果m_TRIGMode的值为1，那么m_LISTENTrigmode被设置为0，m_CONNTrigmode被设置为1，表示LT + ET模式。
//如果m_TRIGMode的值为2，那么m_LISTENTrigmode被设置为1，m_CONNTrigmode被设置为0，表示ET + LT模式。
//如果m_TRIGMode的值为3，那么m_LISTENTrigmode和m_CONNTrigmode都被设置为1，表示ET + ET模式
void webserver::trig_mode()
{
    //TODO::设置触发模式
    //LT + LT

    //LT + ET

    //ET + LT

    //ET + ET

}

// void webserver::log_write()
// {
//     if (0 == m_close_log)
//     {
//         //初始化日志
//         if (1 == m_log_write)
//             log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 800);
//         else
//             log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 0);
//     }
// }

void webserver::sql_pool()
{
    //TODO::创建数据库连接池与初始化读取表
}

void webserver::thread_pool()
{
    //TODO::创建线程池
}

void webserver::eventListen()
{
    // TODO: 创建监听套接字 m_listenfd
    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd >= 0);
    // TODO: 设置优雅关闭连接选项:设置套接字属性，利用SO_LINGER实现是否优雅下线
    if(m_OPT_LINGER == 0)
    {
        struct linger tmp = {0, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    else if(m_OPT_LINGER == 1)
    {
        struct linger tmp = {1, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    // TODO: 绑定地址和端口号
    struct sockaddr_in webserveraddr;
    bzero(&webserveraddr, sizeof(webserveraddr));
    webserveraddr.sin_family = AF_INET;
    webserveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    webserveraddr.sin_port = htons(m_port);
    // 设置套接字属性：利用SO_REUSEADDR实现地址复用
    int flag = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    // 绑定套接字
    int ret = bind(m_listenfd, (struct sockaddr*)&webserveraddr, sizeof(webserveraddr));
    assert(ret >= 0);
    // TODO: 监听套接字
    //表示已连接和未连接的最大队列数总和为5
    ret = listen(m_listenfd, 5);
    assert(ret >= 0);
    // TODO: 初始化工具类
    utils.init(TIMESLOT);
    // TODO: 创建内核事件表 m_epollfd
    epoll_event events[MAX_EVENT_NUMBER];
    //注册5个文件描述符，并用红黑树进行管理
    m_epollfd = epoll_create(5);
    assert(m_epollfd >= 0);
    // TODO: 将监听套接字添加到内核事件表
    utils.addfd(m_epollfd, m_listenfd, false, m_LISTENTrigmode);
    http_conn::m_epollfd = m_epollfd;
    //socketpair()函数用以创建一对无名的，相互套接的套接字
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, m_pipefd);
    assert(ret >= 0);
    utils.setnonblocking(m_pipefd[1]);
    utils.addfd(m_epollfd, m_pipefd[0], false, 0);
    // TODO: 设置信号处理函数   
    utils.addsig(SIGPIPE, SIG_IGN);//忽略SIGPIPE信号，防止进程退出，服务器终止
    utils.addsig(SIGALRM, utils.sig_handler, false); //进程定时器
    utils.addsig(SIGTERM, utils.sig_handler, false); //捕获kill
    // TODO: 设置定时器
    alarm(TIMESLOT);
    Utils::u_pipefd = m_pipefd;
    Utils::u_epollfd = m_epollfd;
}
//创建一个定时器节点，将连接信息挂载
void webserver::timer(int connfd, struct sockaddr_in client_address)
{
    // TODO: 初始化http_conn对象
    users[connfd].init(connfd, client_address, m_root, m_CONNTrigmode, m_close_log, m_user, m_passWord, m_databaseName);
    cout << "[client connfd：]" << connfd;   
    cout << "   [IP]:" << inet_ntoa(client_address.sin_addr) << endl;
    // TODO: 初始化client_data数据
    // 创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    users_timer[connfd].address = client_address;
    users_timer[connfd].sockfd = connfd;
    util_timer *timer = new util_timer();
    timer->user_data = &users_timer[connfd];
    timer->cb_func = cb_func;
    time_t cur = time(NULL);
    timer->expire = cur + 3 * TIMESLOT;
    users_timer[connfd].timer = timer;
    utils.m_timer_lst.add_timer(timer);
}

void webserver::adjust_timer(util_timer *timer)
{
    // TODO: 调整定时器  
    //若有数据传输，则将定时器往后延迟3个单位
    //并对新的定时器在链表上的位置进行调整 
    utils.m_timer_lst.adjust_timer(timer); 
    cout << "adjust timer once" << endl;
}

void webserver::deal_timer(util_timer *timer, int sockfd)
{
    // TODO: 处理超时定时器
    timer->cb_func(timer->user_data);
    if(timer)
    {
        utils.m_timer_lst.del_timer(timer);
    }
    cout << "close fd:" << users_timer[sockfd].sockfd << endl;
}

bool webserver::dealclientdata()
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    // TODO: 处理客户端数据,根据客户端模式不同怎么更改
    if(m_LISTENTrigmode == 0)
    {
        int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
        if(connfd < 0)
        {
            //LOG_ERROR("%s:errno is : %d", "accept error", errno);
            return false;
        }
        if(http_conn::m_user_count >= MAX_FD)
        {
            utils.show_error(connfd, "Server busy");
            //LOG_ERROR("%s", "Server busy");
            return false;
        }
        timer(connfd, client_address);        
    }
    else
    {
        //边缘触发需要一直accept到空
        while(1)
        {
            int connfd = accept(m_listenfd, (struct sockaddr*)&client_address, &client_addrlength);
            if(connfd < 0)
            {
                //LOG_ERROR("%s:errno is:%d", "accept error", errno);
                break;
            }
            if (http_conn::m_user_count >= MAX_FD)
            {
                utils.show_error(connfd, "Server busy");
                //LOG_ERROR("%s", "Server busy");
                break;
            }
            timer(connfd, client_address);
        }
        return false;       
    }
    return true;
}
//处理定时器信号：set the timeout true
bool webserver::dealwithsignal(bool& timeout, bool& stop_server)
{
    //TODO::处理信号
    int ret = 0;
    int sig = -1;
    char signals[1024];
    //从管道读端读出信号值，成功返回字节数，失败返回-1
    //正常情况下，这里的ret返回值总是1，只有14和15两个ASCII码对应的字符
    ret = recv(m_pipefd[0], signals, sizeof(signals), 0);
    if(ret == -1)
    {
        return false;
    }
    else if(ret == 0)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < ret; ++i)
        {       
            //这里面明明是字符
            switch (signals[i])
            {
            //这里是整型
            case SIGALRM:
            {
                timeout = true;
                break;
            }
            //关闭服务器
            case SIGTERM:
            {
                stop_server = true;
                break;
            }
            default:
                break;
            }
        }
    }
    return true;
}

void webserver::dealwithread(int sockfd)
{
    //TODO::处理读事件
    //创建定时器临时变量，将连接对应的定时器取出
    util_timer* timer = users_timer[sockfd].timer;
    //reactor
    if (m_actormodel == 1)
    {
        if(timer)
        {
            this->adjust_timer(timer);
            //待实现
        }       
    }
    else
    {
        //proactor
        if (users[sockfd].read_once())
        {
            //若监测到读事件，将该事件放入请求队列
        }
        else
        {
        }
    }
}

void webserver::dealwithwrite(int sockfd)
{
    //TODO::处理写事件
    //类似上面的处理读事件结构
}


/**
 * @brief WebServer类的事件循环函数。
 *
 * 该函数用于处理服务器的事件循环，监听并处理客户端连接、信号和数据的到达。
 * 在事件循环中，函数会调用epoll_wait函数等待事件的发生，并根据事件类型进行相应的处理。
 * 如果发生错误或服务器关闭，事件循环将会终止。
 *
 * @note 该函数是WebServer类的成员函数。
 */
//事件回环（即服务器主线程）
void webserver::eventLoop()
{
    //TODO::事件循环
    bool timeout = false;
    bool stop_server = false;
    while(!stop_server)
    {
        //等待所监控文件描述符上有事件的产生
        int number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        //EINTR错误的产生：当阻塞于某个慢系统调用的一个进程捕获某个信号且相应信号处理函数返回时，该系统调用可能返回一个EINTR错误。
        //例如：在socket服务器端，设置了信号捕获机制，有子进程，
        //当在父进程阻塞于慢系统调用时由父进程捕获到了一个有效信号时，内核会致使accept返回一个EINTR错误(被中断的系统调用)。
        //在epoll_wait时，因为设置了alarm定时触发警告，导致每次返回-1，errno为EINTR，对于这种错误返回
        //忽略这种错误，让epoll报错误号为4时，再次做一次epoll_wait
        if(number < 0 && errno != EINTR)
        {
            //LOG_ERROR("%s", "epoll failure");
            break;
        }
        //对所有就绪的事件进行处理
        for(int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            //处理新到的客户机连接
            if(sockfd = m_listenfd)
            {
                bool flag = this->dealclientdata();
                if(flag == false)
                    continue;
            }
            //处理异常事件
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //服务器端关闭连接，移除对应的定时器
                util_timer *timer = users_timer[sockfd].timer;
                this->deal_timer(timer, sockfd);
            }
            //处理定时器信号
            else if ((sockfd == m_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                //接收到SIGALRM信号，timeout设置为True
                bool flag = dealwithsignal(timeout, stop_server);
                if (false == flag)
                    //LOG_ERROR("%s", "dealclientdata failure");
                    cout << "dealclientdata failure" << endl;
            }
            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN)
            {
                dealwithread(sockfd);
            }
            //处理客户连接上send的数据
            else if (events[i].events & EPOLLOUT)
            {
                dealwithwrite(sockfd);
            }

        }
    }
}