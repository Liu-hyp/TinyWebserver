#include <iostream>
#include "config.h"

int main(int argc, char *argv[]) {
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
    std::string passwd = "root";
    std::string databasename = "qgydb";
    webserver server;
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
