#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define PW_LEN 256
#define NAME_LEN 256

int main(int argc, char const *argv[])
{
    char name[NAME_LEN];
    char password[PW_LEN];
    const char *real_password = "xyzzy";
    int i = 0;

    initscr();

    move(5, 10);
    printw("%s", "Please login:");

    move(7, 10);
    printw("%s", "User name: ");
    getstr(name);

    move(8, 10);
    printw("%s", "Password: ");
    refresh();

    cbreak();               // 设为cbreak模式,该模式下字符一经键入就被立刻传递给程序.
                            // 而不像在cooked模式中那样首先缓存字符,直到用户按下回车键
                            // 后才将用户输入的字符传递给程序.

    noecho();               // 关闭字符串回显功能.

    memset(password, '\0', sizeof(password));
    while (i < PW_LEN) {
        password[i] = getch();
        if (password[i] == '\n') break;
        move(8, 20 + i);
        addch('*');
        refresh();
        i ++;
    }

    echo();                 // 开启回显功能.
    nocbreak();             // 将输入模式重新设置为cooked模式.

    move(11, 10);
    if (strncmp(real_password, password, strlen(real_password)) == 0)
       printw("%s", "Correct");
    else printw("%s", "Wrong");
    printw("%s", " password");
    refresh();
    sleep(2);

    endwin();
    exit(EXIT_SUCCESS);
    return 0;
}
