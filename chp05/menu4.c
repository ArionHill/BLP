#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

char *menu[] = {
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);

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
    if (!isatty(fileno(stdout))) {
        fprintf(stderr, "You are not a terminal, OK.\n");
    }

    input = fopen("/dev/tty", "r");
    output = fopen("/dev/tty", "w");
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
    } while (choice != 'q');
    tcsetattr(fileno(input), TCSANOW, &initial_settings);
        
    return 0;

}

int getchoice(char *greet, char *choices[], FILE *in, FILE *out)
{
    int chosen = 0;
    int selected;
    char **option;
    
    do {
        fprintf(out, "Choice: %s\n", greet);
        option = choices;
        while (*option) {
            fprintf(out, "Choice: %s\n", *option);
            option ++;
        }
        do {
            selected = fgetc(in);
        } while (selected == '\n' || selected == '\r');
        
        option = choices;
        while (*option) {
            if (selected == *option[0]) {
                chosen = 1;
                break;
            }
            option ++;
        }
        if (!chosen) {
            fprintf(out, "Incorrect choice, select again\n");
        }
    } while (!chosen);
    return selected;
}
