/** 
 * 使用信号并挂起程序的执行是Linux程序设计中的一个重要部分.
 * 这意味着程序不需要总是在执行着.程序不必在一个循环中无休止
 * 地检查某个事件是否已发生,相反,它可以等待事件的发生.这在只
 * 有一个CPU的多用户环境中尤其重要,进程共享着一个处理器,繁忙
 * 的等待将会对系统的性能造成极大的影响.
 * 
 * 程序中信号的使用将带来一个特殊的问题:"如果信号出现在系统调
 * 用的执行过程中会发生什么情况?"--"视情况而定".一般来说,只需
 * 考虑慢系统调用,例如从终端读数据,如果在这个系统调用等待数据
 * 时出现一个信号,它就会返回一个错误.如果在自己的程序中使用信
 * 号,需要注意一些系统调用会因为接收到了一个信号而失败,而这种
 * 错误情况可能是你在添加信号处理函数之前没有考虑到的.
 * 
 * 在编写程序中处理信号部分的代码时必须非常小心,因为在使用信号
 * 的程序中会出现各种各样的"竞态条件".例如,如果想调用pause等
 * 待一个信号,可信号却出现在调用pause之前,就会使程序无限期地
 * 等待一个不会发生的事件.竞态条件都是一些对时间要求很苛刻的问
 * 题,检查和信号相关的代码时要非常小心.
 */
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig) {
    alarm_fired = 1;
}

int main(int argc, char const *argv[])
{
    pid_t pid;

    printf("alarm application starting\n");

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            // sleep(5);
            kill(getppid(), SIGALRM);
            
            exit(0);
    }

    printf("waiting for alarm to go off\n");
    (void) signal(SIGALRM, ding);
    
    /**
     * int pause(void)
     * pause函数用于把程序的执行挂起直到有一个信号出现为止.
     * 当程序接收到一个信号时,预设好的信号处理函数将开始运行,程序也将恢复正常的执行.
     */
    pause();
    if (alarm_fired)
        printf("Ding!\n");

    printf("done\n");
    exit(0);
}
