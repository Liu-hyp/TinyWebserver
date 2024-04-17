#ifndef CONFIG_H
#define CONFIG_H
#include "webserver.h"

class Config
{
public:
    Config();
    ~Config();

    int PORT; //端口号
    int OPT_LINGER; //决定是否优雅下线
    int TRIGMODE; //epoll触发组合模式
    //listenfd触发模式，默认LT
    int LISTENTRIGMODE;
    //http

};
#endif