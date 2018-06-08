#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int server_socketfd, client_socketfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int result;
    fd_set readfds, testfds;

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

    FD_ZERO(&readfds);
    FD_SET(server_socketfd, &readfds);

    while (1) {
        char ch;
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL,
                        (struct timeval *)0); // select 调用不会超时.
        if (result < 1) {
            perror("server5");
            exit(1);
        }

        for (fd = 0; fd < FD_SETSIZE; fd ++) {
            if (FD_ISSET(fd, &testfds)) {
                if (fd == server_socketfd) {
                    client_len = sizeof(client_address);
                    /**
                     * #include <sys/socket.h>
                     * int accept(int socket, struct sockaddr *address, size_t *address_len);
                     * 创建一个新的套接字与客户请求连接进行通信,并返回新套接字的描述符.
                     * 新套接字类型与服务器监听套接字类型相同
                     */
                    client_socketfd = accept(server_socketfd,
                        (struct sockaddr *)&client_address, &client_len);
                    FD_SET(client_socketfd, &readfds);
                    printf("adding client on fd %d\n", client_socketfd);
                }
                else {
                     /**
                     * int ioctl(int fildes, int cmd, ...);
                     * 对描述符fildes引用的对象执行cmd参数中给出的操作.
                     * ioctl 是用来设置硬件控制寄存器，或者读取硬件状态寄存器的数值之类的。而read,write 是把数据丢入缓冲区，硬件的驱动从缓冲区读取数据一个个发送或者把接收的数据送入缓冲区。
                     * ioctl(0,FIONREAD,&nread);得到缓冲区里有多少字节要被读取,然后将字节数放入nread里面.
                     * 然後就可以 read 了.
                     * nread = read(0,buffer,nread);
                     */
                    ioctl(fd, FIONREAD, &nread);
                    if (nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    }
                    else {
                        read(fd, &ch, 1);
                        sleep(5);
                        printf("serving client on fd %d\n", fd);
                        ch ++;
                        write(fd, &ch, 1);

                    }
                }
                   
            }
        }
        
    }

}