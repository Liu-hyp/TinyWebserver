# TinyWebserver

Linux下基于C++的轻量级Web服务器

## 特点

- 使用非阻塞socket

## 实现

- 建立服务器监听

  1. 创建socket

     SOCK_STEREAM

  2. 填充服务器网络信息结构体

     函数：inet_addr

  3. bind：将服务器用来通信的地址和端口信息绑定到socket上

  4. 设置套接字属性

     函数：setsockopt

  5. 使用定时器管理非活跃socket，管理系统资源

     