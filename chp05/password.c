#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

#define PASSWORD_LEN 8

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
    struct termios initialrsettings, newrsettings;
    char password[PASSWORD_LEN + 1];
    /**
     * int tcgetattr(int fd, struct termios *termios_p);
     * 把当前终端接口变量的值写入termios_p参数指向的结构.
     */
    tcgetattr(fileno(stdin), &initialrsettings);
    newrsettings = initialrsettings;
    newrsettings.c_lflag &= ~ECHO;    // 清除ECHO标志位,关闭回显

    printf("Enter password: ");

    /**
     * int tcsetattr(int fd, int actions, const struct termios *termios_p);
     * 重新配置终端接口.
     * actions控制修改方式,共有三种:
     * TCSANOW: 立刻对值进行修改.
     * TCSADRAIN: 等当前的输出完成后再对值进行修改.
     * TCSAFLUSH: 等当前的输出完成后再对值进行修改,但丢弃还未从read调用返回的当前可用的任何输入.
     */
    // TCSAFLUSH丢弃用户在程序准备好读取数据之前输入的任何内容.这样做是为了培养用户的一个好习惯,
    // 即在回显功能关闭之前不要试图输入自己的密码.
    if (tcsetattr(fileno(stdin), TCSAFLUSH, &newrsettings) != 0) {
        fprintf(stderr, "Could not set attributes\n");
    }
    else {
        fgets(password, PASSWORD_LEN, stdin);
        tcsetattr(fileno(stdin), TCSANOW, &initialrsettings);
        fprintf(stdout, "\nYou entered %s\n", password);
    }

    return 0;
}
