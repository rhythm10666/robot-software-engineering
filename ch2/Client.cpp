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
#define MAX_LINE 256
// thread1:发送消息
void* send_func(void* arg)
{
    int sockfd = (int)(long)arg;
    char send_buf[MAX_LINE];
    while (true) {
        if (fgets(send_buf, MAX_LINE, stdin) == NULL) {
            printf("未收到消息\n");
            continue;
        } else {
            if (send(sockfd, send_buf, sizeof(send_buf), 0) == -1) {
                printf("发送失败\n");
                continue;
            } else
                printf("Client: %s\n", send_buf);
        }
    }
    pthread_exit(0);
}
// thread2:接收消息
void* recv_func(void* arg)
{
    int sockfd = (int)(long)arg;
    char recv_buf[MAX_LINE];
    while (true) {
        if (recv(sockfd, recv_buf, sizeof(recv_buf), 0) > 0)
            printf("Server: %s\n", recv_buf);
    }
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    int s, len;
    char *host, buf[MAX_LINE];
    struct hostent* hp;
    struct sockaddr_in sin;

    if (argc == 2)
        // 程序只接受两个参数，第一个为程序自身，第二个为host参数
        host = argv[1];
    else {
        // 否则报错
        fprintf(stderr, "usage: simple-talk host\n");
        exit(1);
    }

    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }
    bzero((char*)&sin, sizeof(sin)); // 将sin置零
    sin.sin_family = AF_INET; // 声明sin的通信地址为IPv4
    bcopy(hp->h_addr, (char*)&sin.sin_addr, hp->h_length); // 指定服务端IP
    sin.sin_port = htons(SERVER_PORT); // 指定端口
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    } else {
        printf("Successfully connected!\n");
        printf("-----------------------------------\n");
    }
    int send_result, recv_result;
    // 声明两条线程
    pthread_t send_thread, recv_thread;
    send_result = pthread_create(&send_thread, NULL, send_func, (void*)(long)s);
    recv_result = pthread_create(&recv_thread, NULL, recv_func, (void*)(long)s);
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
    return 0;
}