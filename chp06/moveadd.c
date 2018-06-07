#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

int main(int argc, char const *argv[])
{
    const char witch_one[] = " First Witch ";
    const char witch_two[] = " Second Witch ";
    const char *scan_ptr;

    initscr();
    move(5, 15);
    /**
     * attron和attroff函数在不影响其他属性的前提下启用或关闭指定的属性.
     */
    attron(A_BOLD);
    printw("%s", "Macbeth");
    attroff(A_BOLD);
    refresh();
    sleep(1);

    move(8, 15);
    attron(A_STANDOUT);
    printw("%s", "Thunder and Lightning");
    attroff(A_STANDOUT);
    refresh();
    sleep(1);

    move(10, 10);
    printw("%s", "When shall we three meet again");
    move(11, 23);
    printw("%s", "In thunder, lightning, or in rain ?");
    move(13, 10);
    printw("%s", "When the hurlyburly's done,");
    move(14, 23);
    printw("%s", "When the battle's lost and win.");
    refresh();
    sleep(1);

    attron(A_DIM);
    scan_ptr = witch_one + strlen(witch_one) - 1;
    
    while(scan_ptr != witch_one) {
        move(10, 10);
        /**
         * int insch(chtype char_to_insert);
         * 插入一个字符,将已有字符右移,但此操作对行尾的影响并未定义,具体情况取决于你所使用的终端.
         */
        insch(*scan_ptr --);
    }
    scan_ptr = witch_two + strlen(witch_two) - 1;
    while(scan_ptr != witch_two) {
        move(13, 10);
        insch(*scan_ptr --);
    }
    attroff(A_DIM);
    refresh();
    sleep(5);

    move(LINES - 1, COLS - 1);

    refresh();
    sleep(5);
    endwin();
    exit(EXIT_SUCCESS);
    return 0;
}
