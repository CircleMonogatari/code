#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define SERVER_PORT 6666
#define bzero(x, len) (memset((x),'\0', (len)), (void)0)

int main(int argc, char const *argv[])
{
    //监听套接字
    int serverSocket;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    int client_addr_len = sizeof(client_addr);
    int client;

    char buffer[200];
    
    int hello_len = 0;


    //获取套接字
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socker");
        return 1;
    }

    //端口复用
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));

    //设置初始化
    bzero(&server_addr, sizeof(server_addr));    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);    //ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
    //bind三个参数：服务器端的套接字的文件描述符，
    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    //设置服务器上的socket为监听状态

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        return 1;
    }

    while (1)
    {
        
        char recv_buf[4096];
        printf("监听端口: %d\n", SERVER_PORT);
        printf("等待连接...\n");
        client = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if (client < 0) {
            perror("accept");
            continue;
        }

        //inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
        //表达式：char *inet_ntoa (struct in_addr);
        printf("============接到链接=================\n");
        printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Port is %d\n", htons(client_addr.sin_port));

        
        //读取数据
        memset(recv_buf, 0, 4096);
        hello_len = recv(client, recv_buf, 4096, 0);
 
        printf("receive %d\n\n", hello_len);
 
        printf("%s\n", recv_buf);


        //回发数据

        /* 发送响应给客户端 */

        char buf[1024];
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client, buf, strlen(buf), 0);
        // strcpy(buf, SERVER_STRING);
        // send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html\r\n");
        send(client, buf, strlen(buf), 0);
        strcpy(buf, "\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "Hello World\r\n");
        send(client, buf, strlen(buf), 0);


        //回发数据完毕
        printf((char *) "--------------回发数据完毕-------------\n");

        //关闭socket
        close(client);
    }
    



    return 0;
}
