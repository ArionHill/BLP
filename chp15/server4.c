#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int server_socketfd, client_socketfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_socketfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    /**
     * #include <sys/socket.h>
     * int bind(int socket, const struct sockaddr *address, size_t address_len);
     * 服务器程序必须给套接字命名以使其可以被其他进程使用.
     * bind系统调用把address中的地址分配给与文件描述符socket关联的未命名套接字.地址长度由参
     * 数address_len传递.地址长度和格式取决于地址族.bind调用需将特定地址结构指针转换为指向通
     * 用地址类型(struct sockaddr *)
     * bind 调用成功时返回0,识别时返回-1,并设置errno
     */
    bind(server_socketfd, (struct sockaddr *)&server_address, server_len);

    /**
     * #include <sys/socket.h>
     * int listen(int socket, int backlog);
     * 创建队列保存未处理的请求.
     */
    listen(server_socketfd, 5);

    signal(SIGCHLD, SIG_IGN);

    while (1) {
        char ch;
        printf("server waiting\n");
        client_len = sizeof(client_address);
        /**
         * #include <sys/socket.h>
         * int accept(int socket, struct sockaddr *address, size_t *address_len);
         * 创建一个新的套接字与客户请求连接进行通信,并返回新套接字的描述符.
         * 新套接字类型与服务器监听套接字类型相同
         */
        client_socketfd = accept(server_socketfd,
            (struct sockaddr *)&client_address, &client_len);

        if (fork() == 0) {
            read(client_socketfd, &ch, 1);
            sleep(5);
            ch ++;
            write(client_socketfd, &ch, 1);
            close(client_socketfd);
            exit(0);
        }
        else {
            close(client_socketfd);
        }
        
    }

}