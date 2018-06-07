#include <stdio.h>
#include <term.h>
#include <curses.h>
#include <stdlib.h>

// 编译: gcc -o badterm badterm.c -lncurses
// 使用curses函数库的ncurses实现,并且使用位于标准位置的头文件时,
// 可以直接在程序中包含curses.h头文件,并在编译时为库文件指定-lncurses选项.

int main() {
    /**
     * #include <term.h>
     * int setupterm(char *term, int fd, int *errret)
     * 将当前终端类型设置为参数term指向的值,如果term为空,就使用环境变量TERM的值.
     * 参数fd为一个打开的文件描述符,它用于向终端写数据.
     * 如果errret不是一个空指针,则函数返回值保存在该参数指向的整型变量中,可能值如下:
     *     -1: terminfo数据库不存在.
     *     0: terminfo数据库中没有匹配的数据项.
     *     1: 成功.
     * setupterm成功时返回常量OK,失败时返回ERR.如果errret被设置为空指针,
     * setupterm函数会在失败时输出一条诊断信息并导致程序直接退出.
     */
    setupterm("unlisted", fileno(stdout), (int *)0); // 输出 'unlisted': unknown terminal type. 并退出.
    printf("Done.\n");  // 不会输出.
    exit(0);
}