#include "datafile.h"

void *server_pthread(void *arg);
void *client_pthread(void *arg);

struct SERVER
{
    pthread_t tid;
    struct sockaddr_in sock;
    int sockfd;
    char ip[14];
};
struct SERVER server;
struct CONNECT
{
    struct sockaddr_in sock;
    int sockfd;
};
struct CONNECT conn;

struct CLIENT
{
    pthread_t tid;
    struct sockaddr_in sock;
    int sockfd;
    char ip[14];

    struct sockaddr_in s_sock;
};
struct CLIENT client;

sem_t wait_sem;
int main(int argc, char **argv)
{
    sem_init(&wait_sem, 0, 0);
    pthread_create(&client.tid, NULL, client_pthread, NULL);
    pthread_create(&server.tid, NULL, server_pthread, NULL);

    pthread_detach(server.tid);
    pthread_detach(client.tid);
    pthread_exit(NULL);
}
void *client_pthread(void *arg)
{
    sem_wait(&wait_sem);
    sem_destroy(&wait_sem);

    printf("\r\n客户端已开启\r\n");
    /* 创建sockfd */
    client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* 设置TCP客户端参数 */
    memset(&client.sock, 0, sizeof(client.sock));
    inet_pton(AF_INET, "192.168.90.235", &client.sock.sin_addr);
    client.sock.sin_family = AF_INET;
    client.sock.sin_port = htons(8888);
    /* 绑定TCP客户端参数 */
    socklen_t len = sizeof(client.sock);
    bind(client.sockfd, (struct sockaddr *)&client.sock, len);
    /* 设置客户端连接的服务端的 sockaddr */
    memset(&client.s_sock, 0, sizeof(client.s_sock));
    inet_pton(AF_INET, "192.168.90.235", &client.s_sock.sin_addr);
    client.s_sock.sin_family = AF_INET;
    client.s_sock.sin_port = htons(10000);
    /* 开始连接 */
    len = sizeof(client.s_sock);
    connect(client.sockfd, (struct sockaddr *)&client.s_sock, len);
    printf("客户端: 连接至 %s:%d\r\n", inet_ntoa(client.s_sock.sin_addr), ntohs(client.s_sock.sin_port));

    while (1)
    {
        send(client.sockfd, "this is send()", strlen("this is send()"), 0);
        sleep(1);
        write(client.sockfd, "this is write()", strlen("this is write()"));
        sleep(1);
    }
}

void *server_pthread(void *arg)
{
    printf("\r\n服务端已开启\r\n");
    /* 创建sockfd */
    server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* 设置TCP服务端参数 */
    memset(&server.sock, 0, sizeof(server.sock));
    inet_pton(AF_INET, "192.168.90.235", &server.sock.sin_addr);
    server.sock.sin_family = AF_INET;
    server.sock.sin_port = htons(10000);
    /* 绑定TCP服务端 */
    socklen_t len = sizeof(server.sock);
    bind(server.sockfd, (struct sockaddr *)&server.sock, len);
    /* 开启监听 */
    listen(server.sockfd, 4);

    sem_post(&wait_sem);
waitconnect:
    len = sizeof(conn.sock);
    conn.sockfd = accept(server.sockfd, (struct sockaddr *)&conn.sock, &len);
    usleep(10000);
    printf("服务端: 已被 %s:%d 连接\r\n", inet_ntoa(conn.sock.sin_addr), ntohs(conn.sock.sin_port));
    send(conn.sockfd, "\r\nhello this is wsl\r\n", strlen("\r\nhello this is wsl\r\n"), 0);
    while (1)
    {
        int ret = 0;
        uint8_t rec_buf[100];
        while (ret = recv(conn.sockfd, rec_buf, sizeof(rec_buf) - 1, 0))
        {
            rec_buf[ret] = '\0';
            printf("rec = %s    ret: %d\r\n", rec_buf, ret);
        }
        if (ret == 0)
        {
            close(conn.sockfd);
            perror("TCP Client off");
            goto waitconnect;
        }
        if (ret < 0)
        {
            close(conn.sockfd);
            perror("TCP_Server");
            goto waitconnect;
        }
        usleep(10000);
    }
    close(server.sockfd);
}