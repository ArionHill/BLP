#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <curses.h>

static FILE *output_stream = (FILE *)0;

char *menu[] = {
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);
int char_to_terminal(int char_to_write);

int main(int argc, char const *argv[])
{
    /**
     * #include <termios.h>
     * struct termios {
     *     tcflag_t c_iflag;    // 输入模式
     *     tcflag_t c_oflag;    // 输出模式
     *     tcflag_t c_cflag;    // 控制模式
     *     tcflag_t c_lflag;    // 本地模式
     *     cc_t     c_cc[NCCS]; // 特殊控制字符
     * }
     */
    int choice = 0;
    FILE *input;
    FILE *output;
    struct termios initial_settings, new_settings;

    /**
     * #include <unistd.h>
     * int isatty(int fd);
     * 如果打开的文件描述符fd链接到一个终端,则系统调用isatty返回1,否则返回0.
     */
    if (!isatty(fileno(stdout))) {                               // 
        fprintf(stderr, "You are not a terminal, OK.\n");
    }

    input = fopen("/dev/tty", "r");                              // fopen 失败返回NULL,
    output = fopen("/dev/tty", "w");                             // 成功返回非空 FILE *指针.
    if (!input || !output) {
        fprintf(stderr, "Unable to open /dev/tty\n");
        exit(1);
    }

    tcgetattr(fileno(input), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;    // 关闭标准输入处理, 该设置影响特殊控制字符c_cc的设置.
    new_settings.c_lflag &= ~ECHO;      // 关闭回显
    new_settings.c_cc[VMIN] = 1;        // MIN = 1
    new_settings.c_cc[VTIME] = 0;       // TIME = 0
    new_settings.c_lflag &= ~ISIG;      // 关闭信号, 按下Ctrl + C, Ctrl + D等均被直接传递给程序,并被程序认为是一个不正确的菜单选择.
    /**
     * TIME 和 MIN 值
     * MIN = 0 TIME = 0: read总是立即返回.有等待处理的字符,它们就会被返回;
     *                   没有等待处理的字符,read调用返回0,且不读取任何字符.
     * 
     * MIN = 0 TIME > 0: 只要有字符可以处理或者经过TIME个十分之一秒的时间间隔,read调用就返回.
     *                   如果因超时而未读到任何字符,read返回0,否则read返回读取的字符数目.
     *
     * MIN > 0 TIME = 0: read调用一直等待,直到有MIN个字符可以读取时才返回,返回值是读取的字符数量.
     *                   到达文件尾时返回0.
     * 
     * MIN > 0 TIME > 0: 当有MIN个字符或两个字符之间的时间间隔超过十分之一秒时,read调用返回.
     *                   这个功能可用于区分是单独按下了Escape键还是按下一个以Escape键开始的功能组合键.
     *                   但要注意的是,网络通信或处理器的高负载将使得类似这样的定时器失去作用.
     */

    if (tcsetattr(fileno(input), TCSANOW, &new_settings) != 0) {
        fprintf(stderr, "could not set attributes\n");
    }

    do {
        choice = getchoice("Please select an action", menu, input, output);
        printf("You have chosen: %c\n", choice);
        sleep(1);
    } while (choice != 'q');
    tcsetattr(fileno(input), TCSANOW, &initial_settings);
        
    return 0;

}

int getchoice(char *greet, char *choices[], FILE *in, FILE *out) {
    int chosen = 0;
    int selected;
    int screenrow, screencol = 10;
    char **option;
    char *cursor, *clear;

    output_stream = out;
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
    setupterm(NULL, fileno(out), (int *)0);
    cursor = tigetstr("cup");                                    // 返回字符串功能标志的值
    clear = tigetstr("clear");

    screenrow = 4;
    /**
     * #include <term.h>
     * int tputs(char *const str, int affcnt, int (*putfunc)(int))
     * 指定一个用于输出字符的函数.面对不能通过标准输出stdout访问终端的情况.
     * 返回值为用户指定函数putfunc的返回结果.参数affcnt表明受这一变化影响的
     * 行数,一般设为1.真正用于输出控制字符串的函数的参数和返回值类型必须与
     * putchar相同.调用putp(string)等同于函数调用tputs(string, 1, putchar)
     */
    tputs(clear, 1, char_to_terminal);                   // 
    /**
     * #include <term.h>
     * char *tparm(char *cap, long p1, long p2, ..., long p9)
     * 用于构造终端的escape转义序列.
     */
    tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);  // 构造escape转义序列并将其发送到终端.
    fprintf(out, "Choice: %s", greet);
    screenrow += 2;
    option = choices;
    while(*option) {
        tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
        fprintf(out, "%s", *option);
        screenrow ++;
        option ++;
    }
    fprintf(out, "\n");

    do {
        fflush(out);
        selected = fgetc(in);
        option = choices;
        while(*option) {
            if (selected == *option[0]) {
                chosen = 1;
                break;
            }
            option ++;
        }
        if (!chosen) {
            tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
            fprintf(out, "Incorrect choice, select again\n");
        }
    } while(!chosen);
    tputs(clear, 1, char_to_terminal);
    return selected;
}

int char_to_terminal(int char_to_write) {
    if (output_stream) putc(char_to_write, output_stream);
    return 0;
}