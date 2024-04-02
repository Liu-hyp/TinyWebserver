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

    //填充网络信息结构体
    struct sockaddr_in webserveraddr;
    bzero(&webserveraddr, sizeof(webserveraddr));
    webserveraddr.sin_family = AF_INET;
    webserveraddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    webserveraddr.sin_port = htons(m_port);

    //设置套接字属性
    int flag = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    //绑定套接字
    int ret = bind(m_listenfd, (struct sockaddr*)&webserveraddr, sizeof(webserveraddr));
    assert(ret >= 0);

    //设置服务器的最小时间间隙
    

}