#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    char *message;
    int n;

    printf("fork program starting\n");
    /*
     * #include <sys/types.h>
     * #include <unistd.h>
     * 
     * pid_t fork(void)
     * fork系统调用复制当前进程,在进程表中创建一个新的表项,新表项中的许多属性与当前进程是相同的.
     * 新进程几乎与原进程一模一样,执行的代码也完全相同,但新进程有自己的数据空间,环境和文件描述符.
     * fork和exec函数结合在一起使用就是创建新进程所需要的一切了.
     */
    pid = fork();
    switch(pid) {
        case -1:
            fprintf(stderr, "fork error!");
            break;
        case 0:
            n = 50;
            message = "this is child process.";
            break;        
        default:
            n = 30;
            message = "this is parent process";
            break;
    }
    
    int i = 0;
    for (; i < n; ++ i) {
        puts(message);
    }
    return 0;
}
