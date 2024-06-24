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
    m_CurConn = 0;
	m_FreeConn = 0;
}

connection_pool *connection_pool::GetInstance()
{
    //TODO::单例模式
    static connection_pool connPool;
	return &connPool;
}

void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log)
{
    //TODO::初始化
    m_url = url;
	m_Port = std::to_string(Port);
	m_User = User;
	m_PassWord = PassWord;
	m_DatabaseName = DBName;
	m_close_log = close_log;
    for (int i = 0; i < MaxConn; i++)
	{
		MYSQL *con = NULL;
		con = mysql_init(con);

		if (con == NULL)
		{
			LOG_ERROR("MySQL Error");
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, NULL, 0);

		if (con == NULL)
		{
			LOG_ERROR("MySQL Error");
			exit(1);
		}
		connList.push_back(con);
		++m_FreeConn;
	}
	reserve = sem(m_FreeConn);
    m_MaxConn = m_FreeConn;

}


MYSQL *connection_pool::GetConnection()
{
    //当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
    MYSQL *con = NULL;

	if (connList.size() == 0)
		return NULL;
	reserve.wait();
	std::unique_lock<std::mutex> locker(lock);   
	con = connList.front();
	connList.pop_front();
	--m_FreeConn;
	++m_CurConn;
	locker.unlock();
	return con;
}
//释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con)
{
    //TODO::释放连接
    if (NULL == con)
		return false;

	std::unique_lock<std::mutex> locker(lock);
	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;
	locker.unlock();
	reserve.post();
	return true;
}

//销毁数据库连接池
void connection_pool::DestroyPool()
{
	std::unique_lock<std::mutex> locker(lock);
	if (connList.size() > 0)
	{
		list<MYSQL *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			MYSQL *con = *it;
			mysql_close(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}
}

int connection_pool::GetFreeConn()
{
    //TODO::当前空闲的连接数
    return this->m_FreeConn;;
}

connection_pool::~connection_pool()
{
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool){
   //TODO::初始化
   	*SQL = connPool->GetConnection();
	
	conRAII = *SQL;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII(){
    //TODO::释放
	poolRAII->ReleaseConnection(conRAII);
}