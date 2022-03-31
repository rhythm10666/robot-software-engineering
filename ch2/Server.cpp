#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

void* send_func(void* arg)
{
    int sockfd = (int)(long)arg;
    char send_buf[MAX_LINE];
    while (true) {
        if (fgets(send_buf, MAX_LINE, stdin) == NULL) {
            puts("未收到消息\n");
            continue;
        } else {
            if (send(sockfd, send_buf, sizeof(send_buf), 0) == -1) {
                printf("发送失败\n");
                continue;
            } else
                printf("Server: %s\n", send_buf);
        }
    }
    pthread_exit(0);
}

void* recv_func(void* arg)
{
    int sockfd = (int)(long)arg;
    char recv_buf[MAX_LINE];
    while (true) {
        if (recv(sockfd, recv_buf, sizeof(recv_buf), 0) > 0)
            printf("Client: %s\n", recv_buf);
    }
    pthread_exit(0);
}

int main()
{
    int s, new_s;
    // sin用来表示网络通信地址
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    bzero((char*)&sin, sizeof(sin)); // 将sin置零
    sin.sin_family = AF_INET; // 声明sin的通信地址为IPv4
    sin.sin_addr.s_addr = INADDR_ANY; // 指定服务端IP
    sin.sin_port = htons(SERVER_PORT); // 指定端口
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // 创建socket
        perror("simplex-talk: socket");
        exit(1);
    }
    if ((bind(s, (struct sockaddr*)&sin, sizeof(sin))) < 0) {
        perror("simplex-talk: bind");
        exit(1);
    }
    listen(s, MAX_PENDING);
    if ((new_s = accept(s, (struct sockaddr*)&sin, &len)) < 0) {
        perror("simplex-talk: accept");
        exit(1);
    } else {
        printf("Successfully connected!\n");
        printf("-----------------------------------\n");
    }
    int send_result, recv_result;
    // 声明两个线程
    pthread_t send_thread, recv_thread;
    send_result = pthread_create(&send_thread, NULL, send_func, (void*)(long)new_s);
    recv_result = pthread_create(&recv_thread, NULL, recv_func, (void*)(long)new_s);
    if (send_result != 0) {
        printf("发送消息线程创建失败！\n");
        exit(1);
    }
    if (recv_result != 0) {
        printf("接收消息线程创建失败！\n");
        exit(1);
    }
    send_result = pthread_join(send_thread, NULL);
    recv_result = pthread_join(recv_thread, NULL);
    close(s);
    close(new_s);
    return 0;
}