#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char computer[256];
    struct utsname uts;

    /**
     * #include <unistd.h>
     * int gethostname(char *name, size_t namelen);
     * 该函数把机器的网络名写入name字符串.该字符串至少有namelen个字符长.
     * 成功时,gethostname返回0, 否则返回-1.
     * 
     * #include <sys/utsname.h>
     * int uname(struct utsname *name);
     * uname函数把主机信息写入name参数指向的结构.utsname结构定义在头文件sys/utsname.h中,
     * 至少包含以下成员:
     * char sysname[]: 操作系统名
     * char nodename[]: 主机名
     * char release[]: 系统发行级别
     * char version[]: 系统版本号
     * char machine[]: 硬件类型
     */
    if (gethostname(computer, 255) != 0 || uname(&uts) < 0) {
        fprintf(stderr, "Could not get host information\n");
        exit(1);
    }

    printf("Computer host name is %s\n", computer);
    printf("System is %s on %s hardware\n", uts.sysname, uts.machine);
    printf("Nodename is %s\n", uts.nodename);
    printf("Version is %s, %s\n", uts.release, uts.version);

    /**
     * #include <unistd.h>
     * long gethostid(void);
     * 返回与主机对应的一个唯一值.
     * 许可证管理者利用它来确保软件程序只能运行在拥有合法许可证的机器上.
     */
    long hostid;
    hostid = gethostid();
    printf("Host ID: %ld\n", hostid);
    return 0;
}
