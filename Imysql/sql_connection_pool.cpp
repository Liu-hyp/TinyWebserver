//
// Created by Nidhogg on 2024/5/28.
//

#include "sql_connection_pool.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool()
{
    //TODO::初始化
}

connection_pool *connection_pool::GetInstance()
{
    //TODO::单例模式
}

void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log)
{
    //TODO::初始化
}


MYSQL *connection_pool::GetConnection()
{
    //当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
    return new MYSQL;
}

bool connection_pool::ReleaseConnection(MYSQL *con)
{
    //TODO::释放连接
    return false;
}

//销毁数据库连接池
void connection_pool::DestroyPool()
{

    //TODO::销毁数据库连接池
}

int connection_pool::GetFreeConn()
{
    //TODO::当前空闲的连接数
    return 0;
}

connection_pool::~connection_pool()
{
    //TODO::销毁
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool){
   //TODO::初始化
}

connectionRAII::~connectionRAII(){
    //TODO::释放
}