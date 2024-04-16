#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<assert.h>
#include<string.h>
#include<cassert>
#include"Utils.h"
#include "./http/http_conn.h"
using namespace std;
#define ERRLOG(errmsg) do{\
    perror("errmsg");\
    exit(1);\
}while(0)

const int TIMESLOT = 5;  //最小超时单位
const int MAX_FD = 65536; //最大文件描述符数目
const int MAX_EVENT_NUMBER = 1000; //最大事件数
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
    int m_OPT_LINGER;//是否优雅下线
    http_conn *user;//单个http连接

    //定时器相关
    util_timer *user_timer;
    //工具类
    Utils utils;
   

};