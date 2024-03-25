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
    int m_listenfd = 0;
    if((m_listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERRLOG("fail to socket");
    }
}