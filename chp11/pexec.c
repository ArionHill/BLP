#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /**
     * exec函数把当前进程替换为一个新进程,新进程由path或file参数指定.
     * 可以使用exec函数将程序的执行从一个程序替换为另一个程序.
     * 新的程序启动后,原来的程序就不再运行了.
     * #include <unistd.h>
     * char **environ
     * int execl(const char *path, const char *arg0, ..., (char *)0);
     * int execlp(const char *file, const char *arg0, ..., (char *)0);
     * int execle(const char *path, const char *arg0, ..., (char *)0, char *const envp[]);
     * 
     * int execv(const char *path, const char *const argv[]);
     * int execvp(const char *file, const char *const argv[]);
     * int execle(const char *path, const char *const argv[], char *const envp[]);
     * exec函数用法:
     * char *const ps_argv[] = {"ps", "ax", 0};
     * char *const ps_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", 0};
     * 
     * execl("/bin/ps", "ps", "ax", 0);
     * execlp("ps", "ps", "ax", 0);
     * execle("/bin/ps", "ps", "ax", 0, ps_envp);
     * 
     * execv("/bin/ps", ps_argv);
     * execvp("ps", ps_argv);
     * execve("/bin/ps", ps_argv, ps_envp);
     */

    // char *const ps_argv[] = {"ps", "ax", 0};
    // char *const ps_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", 0};
     
    // execl("/bin/ps", "ps", "ax", 0);
    // execlp("ps", "ps", "ax", 0);
    // execle("/bin/ps", "ps", "ax", 0, ps_envp);
     
    // execv("/bin/ps", ps_argv);
    // execvp("ps", ps_argv);
    // execve("/bin/ps", ps_argv, ps_envp);
    printf("Running ps with execlp\n");
    execlp("ps", "ps", "ax", 0);
    printf("Done.\n");
    exit(0);
}
