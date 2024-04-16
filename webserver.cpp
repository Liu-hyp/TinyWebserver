#include<webserver.h>

WebServer::WebServer()
{

}

WebServer::~WebServer()
{

}

//创建网络通信：socket通信
void WebServer::eventListen()
{
    //创建网络套接字
    //SOCK_STREAM 表示使用面向字节流的TCP协议
    // if((m_listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     ERRLOG("fail to socket");
    // }

    //对于内部非法情况，使用assert断言
    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd >= 0);
    //设置套接字属性：利用SO_LINGER实现是否优雅下线
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
    //填充网络信息结构体
    struct sockaddr_in webserveraddr;
    bzero(&webserveraddr, sizeof(webserveraddr));
    webserveraddr.sin_family = AF_INET;
    webserveraddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    webserveraddr.sin_port = htons(m_port);

    //设置套接字属性：利用SO_REUSEADDR实现地址复用
    int flag = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    //绑定套接字
    int ret = bind(m_listenfd, (struct sockaddr*)&webserveraddr, sizeof(webserveraddr));
    assert(ret >= 0);

    //服务器被动监听,设置记录正在连接未连接的客户端数目为5
    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    //设置服务器的最小时间间隙
    utils.init(TIMESLOT);

    //epoll创建内核事件表
    //现在只有监听的文件描述符
    //所有的文件描述符对应的读写缓冲区状态都是委托内核检测的epoll
    //创建红黑书模型的实例
    int epfd = epoll_create(5);
    assert(epfd >= 0);
    //往epoll实例中添加节点
    struct epoll_event ev;
    ev.events = EPOLLIN; //检测listenfd读缓冲区是否有数据
    ev.data.fd = m_listenfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, m_listenfd, &ev);
    assert(ret >= 0);
    struct epoll_event evs[MAX_EVENT_NUMBER];
    
    

}