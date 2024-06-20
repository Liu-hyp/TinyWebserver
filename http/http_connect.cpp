//
// Created by Nidhogg on 2024/5/28.
//

#include "http_connect.h"

#include <mysql/mysql.h>
#include <fstream>

//定义http响应的一些状态信息
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

locker m_lock;
map<string, string> users;
//TODO::初始化连接池
void http_conn::initmysql_result(connection_pool *connPool)
{
    //先从连接池中取一个连接


    //从表中检索完整的结果集


    //返回结果集中的列数


    //返回所有字段结构的数组


    //从结果集中获取下一行，将对应的用户名和密码，存入map中
}

//TODO::对文件描述符设置非阻塞
int setnonblocking(int fd)
{

}

//TODO::将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (TRIGMode == 1)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

//TODO::从内核时间表删除描述符
void removefd(int epollfd, int fd)
{

}

//TODO::将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev, int TRIGMode)
{

}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

//TODO::关闭连接，关闭一个连接，客户总量减一
void http_conn::close_conn(bool real_close)
{

}

//TODO::初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in &addr, char *root, int TRIGMode,
                     int close_log, string user, string passwd, string sqlname)
{
    m_sockfd = sockfd;
    m_address = addr;
    addfd(m_epollfd, sockfd, true, m_TRIGMode);
    m_user_count++;
    //当浏览器出现连接重置时，可能是网站根目录出错或http响应格式出错或者访问的文件中内容完全为空
    // doc_root = root;
    // m_TRIGMode = TRIGMode;
    // m_close_log = close_log;

    // strcpy(sql_user, user.c_str());
    // strcpy(sql_passwd, passwd.c_str());
    // strcpy(sql_name, sqlname.c_str());
}

//TODO::初始化新接受的连接
//check_state默认为分析请求行状态
//TODO::从状态机，用于分析出一行内容
//返回值为行的读取状态，有LINE_OK,LINE_BAD,LINE_OPEN
http_conn::LINE_STATUS http_conn::parse_line()
{

}

//TODO::循环读取客户数据，直到无数据可读或对方关闭连接
//非阻塞ET工作模式下，需要一次性将数据读完
bool http_conn::read_once()
{

    //LT读取数据

    //ET读数据
}

//TODO::解析http请求行，获得请求方法，目标url及http版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text)
{

    //当url为/时，显示判断界面

}

//TODO::解析http请求的一个头部信息
http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{

}

//TODO::判断http请求是否被完整读入
http_conn::HTTP_CODE http_conn::parse_content(char *text)
{

}

//TODO::处理http请求
http_conn::HTTP_CODE http_conn::process_read()
{

}

//TODO::生成响应
http_conn::HTTP_CODE http_conn::do_request()
{

    //处理cgi

        //根据标志判断是登录检测还是注册检测


        //将用户名和密码提取出来
        //user=123&passwd=123

            //如果是注册，先检测数据库中是否有重名的
            //没有重名的，进行增加数据

            //如果是登录，直接判断
            //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0


}
//TODO::将内存映射区释放
void http_conn::unmap()
{

}
//TODO::写http响应
bool http_conn::write()
{

}

//TODO::往写缓冲中写入待发送的数据
bool http_conn::add_response(const char *format, ...)
{

}


bool http_conn::add_status_line(int status, const char *title)
{

}

bool http_conn::add_headers(int content_len)
{

}

bool http_conn::add_content_length(int content_len)
{

}

bool http_conn::add_content_type()
{

}
bool http_conn::add_linger()
{

}
bool http_conn::add_blank_line()
{

}
bool http_conn::add_content(const char *content)
{

}
//TODO::根据服务器处理HTTP请求的结果，决定返回给客户端的内容
bool http_conn::process_write(HTTP_CODE ret)
{

}
//TODO::由线程池中的工作线程调用，这是处理http请求的入口函数
void http_conn::process()
{

}
