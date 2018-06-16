/*
 * 父进程等待子进程结束.
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    char *message;
    int n;
    int exit_code;

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
            exit_code = 37;
            break;        
        default:
            n = 30;
            message = "this is parent process";
            exit_code = 0;
            break;
    }
    
    int i = 0;
    for (; i < n; ++ i) {
        puts(message);
    }

    if (pid != 0) {
        int stat_val;
        pid_t child_pid;

        /**
         * pid_t wait(int *stat_loc)
         * 暂停父进程直到它的子进程结束为止.
         * 返回值为子进程的PID.
         * 如果stat_loc不是空指针,状态信息将写入它所指向的位置.
         * 可以用sys/wait.h文件中定义的宏来解释状态信息.
         * WIFEXITED(stat_val)        如果子进程正常结束,取非0值
         * WEXITSTATUS(stat_val)      如果WIFEXITED非0,返回子进程的退出码
         * WIFSIGNALED(stat_val)      如果子进程因一个未捕获的信号而终止,取非0值
         * WTERMSIG(stat_val)         如果WIFSIGNALED非0,返回一个信号代码  
         * WIFSTOPPED(stat_val)       如果子进程意外终止,取一个非0值
         * WSTOPSIG(stat_val)         如果WIFSTOPPED非0,返回一个信号代码
         */
        child_pid = wait(&stat_val); //等待子进程退出

        printf("Child has finished: PID = %d\n", child_pid);

        if (WIFEXITED(stat_val))
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        else
            printf("Child terminated abnormally\n");
    }
    exit(exit_code); // wait(&stat_val)等待子进程调用exit(exit_code)退出码为37
                     // 父进程调用exit(exit_code)退出码为0
}
