#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int sockfd;
    int len;
    /**
     * struct sockaddr_in {
     *     short int             sin_family;    AF_INET
     *     unsigned short int    sin_port;      Port number
     *     struct in_addr        sin_addr;      Internet address
     * };用于AF_INET域中的套接字地址结构
     * 
     * struct in_addr {
     *     unsigned long int     s_addr;
     * } 
     */
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    /**
     * #include <sys/types.h>
     * #include <sys/socket.h>
     * int socket(int domain, int type, int protocol)
     * 创建套接字
     * domain: AF_UNIX, AF_INET, AF_ISO, AF_NS, AF_IPX, AF_APPLETALK
     *     AF_UNIX: UNIX和Linux文件系统实现的本地套接字
     *     AF_INET: UNIX网络套接字,用于TCP/IP网络通信程序
     * type: SOCK_STREAM, SOCK_DGRAM
     *     SOCK_STREAM: 建立TCP连接
     *     SOCK_DGRAM: 建立UDP连接
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

    /**
     * #include <sys/socket.h>
     * int connect(int socket, const struct sockaddr *address, size_t address_len)
     * 参数socket指定的套接字连接到参数address指定的套接字.
     * 成功返回0,失败返回-1
     */
    result = connect(sockfd, (struct sockaddr *)&address, len);
    
    if (result == -1) {
        perror("oops: client");
        exit(1);
    }

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);

}