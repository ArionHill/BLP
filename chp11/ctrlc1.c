#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void ouch(int sig) {
    printf("OUCH! - I got signal %d\n", sig);
    /**
     * void (*signal(int sig, void (*func)(int)))(int);
     * 准备捕获或忽略的信号由参数sig给出,接收到指定的信号后将要调用的函数由参数func给出.
     * 信号处理函数必须有一个int类型的参数(即接收到的信号代码)并且返回类型为void.
     * signal函数本身也返回一个同类型的函数,即先前用来处理这个信号的函数.
     * 或者也可以用如下两个特殊值之一来代替信号处理函数.
     * SIG_IGN           忽略信号
     * SIG_DFL           恢复默认行为
     */
    (void) signal(SIGINT, SIG_DFL);
}


int main(int argc, char const *argv[])
{
    (void) signal(SIGINT, ouch);

    while(1) {
        printf("Hello World!\n");
        sleep(1);
    }
    return 0;
}
