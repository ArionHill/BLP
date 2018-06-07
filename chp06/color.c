#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>


int main(int argc, char const *argv[])
{
    int i;
    
    initscr();

    /**
     * bool has_colors(void)
     * 终端支持彩色返回true
     */
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Error - no color support on this cerminal\n");
        exit(1);
    }

    /**
     * int start_color(void)
     * 成功初始化颜色显示功能返回OK.
     * 成功后变量COLOR_PAIRS被设置为终端所能支持的颜色组合数目的最大值.
     * COLORS定义可用颜色数目的最大值.
     */
    if (start_color() != OK) {
        endwin();
        fprintf(stderr, "Error - counld not initialize colors\n");
        exit(2);
    }

    clear();
    mvprintw(5, 5, "There are %d COLORS, and %d COLOR_PAIRS available",
            COLORS, COLOR_PAIRS);
    refresh();
    
    /**
     * int init_pair(short pair_number, short foreground, short background);
     * 将颜色作为属性使用之前,必须首先调用init_pair函数对准备使用的颜色组合进行初始化.
     */
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_GREEN);
    init_pair(3, COLOR_GREEN, COLOR_RED);
    init_pair(4, COLOR_YELLOW, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(7, COLOR_CYAN, COLOR_WHITE);

    for (i = 1; i <= 7; i ++) {
        attroff(A_BOLD);
        attrset(COLOR_PAIR(i));
        mvprintw(5 + i, 5, "Color pair %d", i);
        /**
         * int COLOR_PAIR(int pair_number);
         * 访问颜色属性.
         */
        attrset(COLOR_PAIR(i) | A_BOLD);
        mvprintw(5 + i, 25, "Bold color pair %d", i);
        refresh();
        sleep(1);
    }
    sleep(500);

    endwin();
    exit(EXIT_SUCCESS);
}
