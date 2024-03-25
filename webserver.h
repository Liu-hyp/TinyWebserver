#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
using namespace std;
#define ERRLOG(errmsg) do{\
    perror("errmsg");\
    exit(1);\
}while(0)

class WebServer
{
public:
    WebServer();
    ~WebServer();
    //创建网络编程：socket通信
    void eventListen();
public:
    //基础信息
    int m_port;//端口
    //epoll_event相关
    int m_listenfd;//监听套接字
    int m_OPT_LINGER;//是否下线

};