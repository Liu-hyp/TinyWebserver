#include <iostream>
#include "config.h"
//#include "mysql_conn.h"
//#include "connpool.h"
using namespace std;

int main(int argc, char *argv[]) {
    /*            test                          */
    /*
    // 1.单线程：使用/不适用连接池
// 2.多线程：使用/不适用连接池
 
void op1(int begin, int end) {
    for (int i = begin; i < end; ++i) {
        MysqlConn conn;
        conn.connect("heheda", "123456", "test", "127.0.0.1", 3306);
        char sql[1024] = { 0 };
        sprintf(sql, "insert into person (id,age,sex,name) values(%d,%d,'%s','%s')",
            i, 6, "女", "胧月大魔王");
        conn.update(sql);
    }
}
 
void op2(ConnPool* pool, int begin, int end) {
    for (int i = begin; i < end; ++i) {
        shared_ptr<MysqlConn> conn = pool->getConn();
        char sql[1024] = { 0 };
        sprintf(sql, "insert into person (id,age,sex,name) values(%d,%d,'%s','%s')",
            i, 19, "女", "安陵容");
        conn->update(sql);
    }
}
 
// 压力测试
void test1() {
#if 1
    // 非连接池，单线程，用时：26276567700 纳秒, 26276 毫秒
    steady_clock::time_point begin = steady_clock::now();
    op1(0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin; // 计算时间差，得到操作耗时
    cout << "非连接池，单线程，用时：" << length.count() << " 纳秒,"
        << length.count() / 1000000 << " 毫秒" << endl;
#else
    //连接池，单线程，用时：6100671100 纳秒, 6100 毫秒
    ConnPool* pool = ConnPool::getConnPool();
    steady_clock::time_point begin = steady_clock::now();
    op2(pool, 0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin; // 计算时间差，得到操作耗时
    cout << "连接池，单线程，用时：" << length.count() << " 纳秒,"
        << length.count() / 1000000 << " 毫秒" << endl;
#endif
}
 
void test2() {
#if 0
    // 非连接池，多线程，用时：10107791500 纳秒,10107 毫秒
    MysqlConn conn;
    conn.connect("heheda", "123456", "test", "127.0.0.1", 3306);
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin; // 计算时间差，得到操作耗时
    cout << "非连接池，多线程，用时：" << length.count() << " 纳秒,"
        << length.count() / 1000000 << " 毫秒" << endl;
#else 
    // 连接池，多线程，用时：2411691800 纳秒,2411 毫秒
    ConnPool* pool = ConnPool::getConnPool();
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op2, pool, 0, 1000);
    thread t2(op2, pool, 1000, 2000);
    thread t3(op2, pool, 2000, 3000);
    thread t4(op2, pool, 3000, 4000);
    thread t5(op2, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin; // 计算时间差，得到操作耗时
    cout << "连接池，多线程，用时：" << length.count() << " 纳秒,"
        << length.count() / 1000000 << " 毫秒" << endl;
#endif
}
 
int query() {
    MysqlConn conn;
    conn.connect("heheda", "123456", "test", "127.0.0.1", 3306);
    string sql = "insert into person values(6,35,'女','乌拉那拉皇后')";
    bool flag = conn.update(sql);
    if (flag) cout << "插入数据成功了！！！" << endl;
    else cout << "插入数据失败,耐心检查一下哪里有出差错喔！" << endl;
 
    sql = "select * from person";
    conn.query(sql);
    while (conn.next()) {
        cout << conn.value(0) << ", "
            << conn.value(1) << ", "
            << conn.value(2) << ", "
            << conn.value(3) << endl;
    }
    return 0;
}
 
int main() {
    //SetConsoleOutputCP(CP_UTF8);
    //query();
    //test1();
    test2();
    return 0;
}*/
    // threadpool pool(4, nullptr, 4, 50);
    // for(int i = 0; i < 10; i++)
    // {
    //     pool.enqueue([i]{
    //         std::cout << "task: " << i << "is running" << std::endl;
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //         std::cout << "task: " << i << "is done" << std::endl;
    //     });
    // }

    //需要修改的数据库信息,登录名,密码,库名 
    std::string user = "root";
    std::string passwd = "18861817269lwj";
    std::string databasename = "webserDb";
    //命令行解析
    config m_config;
    m_config.parse_arg(argc, argv);
    webserver server;
    //初始化
    server.init(m_config.PORT, user, passwd, databasename, m_config.LOGWrite, 
                m_config.OPT_LINGER, m_config.TRIGMode,  m_config.sql_num,  m_config.thread_num, 
                m_config.close_log, m_config.actor_model);
    //日志
    server.log_write();
    //数据库
    server.sql_pool();
    //线程池
    server.thread_pool();
    //触发模式
    server.trig_mode();

    server.eventListen();
    server.eventLoop();   
    // //命令行解析
    // config config;
    // config.parse_arg(argc, argv);

    // webserver server;

    // //初始化
    // server.init(config.PORT, user, passwd, databasename, config.LOGWrite,
    //             config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num,
    //             config.close_log, config.actor_model);


    // //日志
    // server.log_write();

    // //数据库
    // server.sql_pool();

    // //线程池
    // server.thread_pool();

    // //触发模式
    // server.trig_mode();

    // //监听
    // server.eventListen();

    // //运行
    // server.eventLoop();

    return 0;
}
