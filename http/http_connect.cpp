//
// Created by Nidhogg on 2024/5/28.
//

#include "http_connect.h"

#include <mysql/mysql.h>
#include <mutex>
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

std::mutex m_lock;
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
    this->m_sockfd = sockfd;
    this->m_address = addr;
    addfd(m_epollfd, sockfd, true, m_TRIGMode);
    http_conn::m_user_count++;
    //当浏览器出现连接重置时，可能是网站根目录出错或http响应格式出错或者访问的文件中内容完全为空
    this->doc_root = root;
    this->m_TRIGMode = TRIGMode;
    this->m_close_log = close_log;

    strcpy(this->sql_user, user.c_str());
    strcpy(this->sql_passwd, passwd.c_str());
    strcpy(this->sql_name, sqlname.c_str());
    this->init();
    
}

//TODO::初始化新接受的连接
//check_state默认为分析请求行状态
void http_conn::init()
{
    this->mysql = NULL;
    this->bytes_to_send = 0;
    this->bytes_have_send = 0;
    this->m_check_state = CHECK_STATE::CHECK_STATE_REQUESTLINE;
    this->m_linger = false;
    this->m_method = METHOD::GET;
    this->m_url = 0;
    this->m_version = 0;
    this->m_content_length = 0;
    this->m_host = 0;
    this->m_start_line = 0;
    this->m_checked_idx = 0;
    this->m_read_idx = 0;
    this->m_write_idx = 0;
    this->cgi = 0;
    this->m_state = 0;
    this->timer_flag = 0;
    this->improv = 0;
    memset(this->m_read_buf, '\0', http_conn::READ_BUFFER_SIZE);
    memset(this->m_write_buf, '\0', http_conn::WRITE_BUFFER_SIZE);
    memset(this->m_real_file, '\0', http_conn::FILENAME_LEN);
}
//check_state默认为分析请求行状态
//TODO::从状态机，用于分析出一行内容
//返回值为行的读取状态，有LINE_OK,LINE_BAD,LINE_OPEN
http_conn::LINE_STATUS http_conn::parse_line()
{
    char temp;
    for (; m_checked_idx < m_read_idx; ++m_checked_idx)
    {
        temp = m_read_buf[m_checked_idx];
        if (temp == '\r')
        {
            if ((m_checked_idx + 1) == m_read_idx)
                return LINE_STATUS::LINE_OPEN;
            else if (m_read_buf[m_checked_idx + 1] == '\n')
            {
                m_read_buf[m_checked_idx++] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_STATUS::LINE_OK;
            }
            return LINE_STATUS::LINE_BAD;
        }
        else if (temp == '\n')
        {
            if (m_checked_idx > 1 && m_read_buf[m_checked_idx - 1] == '\r')
            {
                m_read_buf[m_checked_idx - 1] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_STATUS::LINE_OK;
            }
            return LINE_STATUS::LINE_BAD;
        }
    }
    return LINE_STATUS::LINE_OPEN;
}

//TODO::循环读取客户数据，直到无数据可读或对方关闭连接
//非阻塞ET工作模式下，需要一次性将数据读完
bool http_conn::read_once()
{
    if (m_read_idx >= READ_BUFFER_SIZE)
    {
        return false;
    }
    int bytes_read = 0;
    /*读取数据*/
    //LT读取数据
    if(m_TRIGMode == 0)
    {
        bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
        m_read_idx += bytes_read;

        if (bytes_read <= 0) //<0出错 =0连接关闭
        {
            return false;
        }
        return true;
    }
    //ET读数据
    else
    {
        while (true)
        {
            bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
            if (bytes_read == -1)
            {
                //非阻塞ET模式下，需要一次性将数据读完，无错
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                return false;
            }
            else if (bytes_read == 0)
            {
                return false;
            }
            m_read_idx += bytes_read;
        }
        return true;
    }
    return false;
    
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

//TODO::有限状态机处理请求报文
http_conn::HTTP_CODE http_conn::process_read()
{
    LINE_STATUS line_status = LINE_STATUS::LINE_OK;
    HTTP_CODE ret = HTTP_CODE::NO_REQUEST;
    char *text = 0;

    while ((m_check_state == CHECK_STATE::CHECK_STATE_CONTENT && line_status == LINE_STATUS::LINE_OK) || ((line_status = parse_line()) == LINE_STATUS::LINE_OK))
    {
        text = get_line();
        m_start_line = m_checked_idx;
        //LOG_INFO("%s", text);
        switch (m_check_state)
        {
        case CHECK_STATE::CHECK_STATE_REQUESTLINE:
        {
            ret = parse_request_line(text);
            if (ret == HTTP_CODE::BAD_REQUEST)
                return HTTP_CODE::BAD_REQUEST;
            break;
        }
        case CHECK_STATE::CHECK_STATE_HEADER:
        {
            ret = parse_headers(text);
            if (ret == HTTP_CODE::BAD_REQUEST)
                return HTTP_CODE::BAD_REQUEST;
            else if (ret == HTTP_CODE::GET_REQUEST)
            {
                return do_request();
            }
            break;
        }
        case CHECK_STATE::CHECK_STATE_CONTENT:
        {
            ret = parse_content(text);
            if (ret == HTTP_CODE::GET_REQUEST)
                return do_request();
            line_status = LINE_STATUS::LINE_OPEN;
            break;
        }
        default:
            return HTTP_CODE::INTERNAL_ERROR;
        }
    }
    return HTTP_CODE::NO_REQUEST;

}

//TODO::生成响应
http_conn::HTTP_CODE http_conn::do_request()
{
    strcpy(m_real_file, doc_root);
    int len = strlen(doc_root);
    //printf("m_url:%s\n", m_url);
    const char *p = strrchr(m_url, '/');

    //处理cgi
    if (cgi == 1 && (*(p + 1) == '2' || *(p + 1) == '3'))
    {

        //根据标志判断是登录检测还是注册检测
        char flag = m_url[1];

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/");
        strcat(m_url_real, m_url + 2);
        strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
        free(m_url_real);

        //将用户名和密码提取出来
        //eg:user=123&passwd=123
        char name[100], password[100];
        int i;
        for (i = 5; m_string[i] != '&'; ++i)
            name[i - 5] = m_string[i];
        name[i - 5] = '\0';

        int j = 0;
        for (i = i + 10; m_string[i] != '\0'; ++i, ++j)
            password[j] = m_string[i];
        password[j] = '\0';

        if (*(p + 1) == '3')
        {
            //如果是注册，先检测数据库中是否有重名的
            //没有重名的，进行增加数据
            char *sql_insert = (char *)malloc(sizeof(char) * 200);
            strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
            strcat(sql_insert, "'");
            strcat(sql_insert, name);
            strcat(sql_insert, "', '");
            strcat(sql_insert, password);
            strcat(sql_insert, "')");

            if (users.find(name) == users.end())
            {
                m_lock.lock();
                int res = mysql_query(mysql, sql_insert);
                users.insert(pair<string, string>(name, password));
                m_lock.unlock();

                if (!res)
                    strcpy(m_url, "/log.html");
                else
                    strcpy(m_url, "/registerError.html");
            }
            else
                strcpy(m_url, "/registerError.html");
        }
        //如果是登录，直接判断
        //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
        else if (*(p + 1) == '2')
        {
            if (users.find(name) != users.end() && users[name] == password)
                strcpy(m_url, "/welcome.html");
            else
                strcpy(m_url, "/logError.html");
        }
    }
    //如果请求资源为/0，表示跳转注册界面
    if (*(p + 1) == '0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/register.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    //如果请求资源为/1，表示跳转登录界面
    else if (*(p + 1) == '1')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/log.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    //如果请求资源为/5，表示跳转pic
    else if (*(p + 1) == '5')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/picture.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    //如果请求资源为/6，表示跳转video
    else if (*(p + 1) == '6')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/video.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    //如果请求资源为/7，表示跳转weixin
    else if (*(p + 1) == '7')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/fans.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else
        //如果以上均不符合，即不是登录和注册，直接将url与网站目录拼接
        //这里的情况是welcome界面，请求服务器上的一个图片
        strncpy(m_real_file + len, m_url, FILENAME_LEN - len - 1);
    //通过stat获取请求资源文件信息，成功则将信息更新到m_file_stat结构体
    //失败返回NO_RESOURCE状态，表示资源不存在
    if (stat(m_real_file, &m_file_stat) < 0)
        return HTTP_CODE::NO_RESOURCE;

    //判断文件的权限，是否可读，不可读则返回FORBIDDEN_REQUEST状态
    if (!(m_file_stat.st_mode & S_IROTH))
        return HTTP_CODE::FORBIDDEN_REQUEST;

    //判断文件类型，如果是目录，则返回BAD_REQUEST，表示请求报文有误
    if (S_ISDIR(m_file_stat.st_mode))
        return HTTP_CODE::BAD_REQUEST;

    //以只读方式获取文件描述符，通过mmap将该文件映射到内存中
    int fd = open(m_real_file, O_RDONLY);
    m_file_address = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    //避免文件描述符的浪费和占用
    close(fd);

    //表示请求文件存在，且可以访问
    return HTTP_CODE::FILE_REQUEST;

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
void http_conn::process(http_conn& user)
{
    //NO_REQUEST，表示请求不完整，需要继续接收请求数据
    HTTP_CODE read_ret = user.process_read();
    if (read_ret == http_conn::HTTP_CODE::NO_REQUEST)
    {
        //注册并监听读事件
        modfd(user.m_epollfd, user.m_sockfd, EPOLLIN, user.m_TRIGMode);
        return;
    }
    //调用process_write完成报文响应
    bool write_ret = user.process_write(read_ret);
    if (!write_ret)
    {
        user.close_conn();
    }
    //注册并监听写事件
    modfd(user.m_epollfd, user.m_sockfd, EPOLLOUT, user.m_TRIGMode);
}
