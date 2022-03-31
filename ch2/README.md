# ch2: SSH与socket通信

## SSH
通过`ssh name@host`命令对局域网内电脑进行ssh连接（默认22端口）。连接成功后可以看到终端名称变为远程端。

## 使用Socket和多线程编程实现消息收发

- Server：
  - `g++ Server.cpp -o Server -lpthread`编译
  - `./Server`运行服务端程序。
- Client：
  - `g++ Client.cpp -o Client -lpthread`编译
  - `./Client ${connect host}$`运行客户端程序，其中`{connect host}`为连接到服务端的IP地址。（如果是本地测试即为`127.0.0.1`）。

代码说明：

- send_func为发送消息线程的传入函数指针
- recv_func为接收消息线程的传入函数指针
- Server.cpp的流程为：
  1. 在Server端本地IP创建套接字
  2. bind()将套接字地址与所创建的套接字号联系
  3. listen()指定请求连接队列的最大长度
  4. accept()用于使服务器等待来自客户端的实际连接
  5. 与Client端建立连接后通过send_thread和recv_thread两线程进行消息收发
- Client.cpp的流程为：
  1. 通过运行Client程序传入的形参确定Server端的IP地址
  2. 建立套接字，并通过connect和Server端建立连接
  3. 与Server端建立连接后通过send_thread和recv_thread两线程进行消息收发
