#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int main() {
    WINDOW *new_window_ptr;
    WINDOW *popup_window_ptr;
    int x_loop;
    int y_loop;
    char a_letter = 'a';

    initscr();
    move(5, 5);
    printw("%s", "Testing multiple windows");
    refresh();

    for (y_loop = 0; y_loop < LINES - 1; y_loop ++) {
        for (x_loop = 0; x_loop < COLS - 1; x_loop ++) {
            mvwaddch(stdscr, y_loop, x_loop, a_letter);
            a_letter ++;
            if (a_letter > 'z') a_letter = 'a';
        }
    }

    refresh();
    sleep(2);

    /**
     * #include <curses.h>
     * WINDOW *newwin(int num_of_lines, int num_of_cols, int start_y, int start_x);
     * int delwin(WINDOW *window_to_delete);
     * newwin创建一个新窗口,该窗口从屏幕位置(start_y, start_x)开始,行数和列数分别由参数
     * num_of_lines和num_of_cols指定.返回一个指向新窗口的指针,如果新窗口创建失败则返回NULL.
     * 如果想让新窗口的右下角落在屏幕的右下角,可以将该函数的行,列参数设为0.所有的窗口范围都必须在
     * 当前屏幕范围之内,如果新窗口的任何部分落在当前屏幕范围之外,则newwin函数调用将失败.通过函数
     * newwin创建的新窗口完全独立于所有已存在的窗口.默认情况下,它被放置在任何已有窗口之上,覆盖它
     * 们的内容.
     * delwin删除一个先前通过newwin函数创建的窗口.因为调用newwin函数可能会给新窗口分配内存,当
     * 不需要这些窗口时,不要忘记通过delwin函数将其删除.
     */
    new_window_ptr = newwin(10, 20, 5, 5);
    /**
     * #include <curses.h>
     * mvwprintw(WINDOW *window_pointer, int y, int x, char *format, ...);
     * 由函数int printw(char *format, ...)加前缀mvw组成,前缀w用于窗口,前缀mv用于光标移动,
     * 加前缀w,就必须在函数参数表的最前面增加一个WINDOW指针
     * 加mv前缀,则需在函数参数表前面增加纵坐标参数y和横坐标参数x.
     * 坐标值y和x是相对于窗口而不是相对于屏幕,坐标(0,0)表示窗口左上角.
     */
    mvwprintw(new_window_ptr, 2, 2, "%s", "Hello World");
    mvwprintw(new_window_ptr, 5, 2, "%s",
             "Notice how very long lines wrap inside the window");
    wrefresh(new_window_ptr);
    sleep(2);

    a_letter = '0';
    for (y_loop = 0; y_loop < LINES - 1; y_loop ++) {
        for (x_loop = 0; x_loop < COLS - 1; x_loop ++) {
            mvwaddch(stdscr, y_loop, x_loop, a_letter);
            a_letter ++;
            if (a_letter > '9') a_letter = '0';
        }
    }
    refresh();
    sleep(2);

    wrefresh(new_window_ptr);
    sleep(2);
    /**
     * #include <curses.h>
     * int touchwin(WINDOW *window_ptr);
     * 通知curses函数库其指针参数指向的窗口内容已发生改变.这意味着,下次调用wrefresh函数时,
     * curses必须必须重新绘制该窗口,即使用户实际上并未修改该窗口中的内容.当屏幕上重叠着多个
     * 窗口时,可以通过该函数来安排要显示的窗口.
     * 
     */
    touchwin(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);

    popup_window_ptr = newwin(10, 20, 8, 8);
    /**
     * #include <curses.h>
     * int box(WINDOW *win_ptr, chtype vertical_char, chtype horizotal_char);
     * chtype:curses自己的字符类型,比标准的char类型包含更多的二进制位.
     * 围绕一个窗口绘制方框.
     */
    box(popup_window_ptr, '|', '-');
    mvwprintw(popup_window_ptr, 5, 2, "%s", "Pop Up Window!");
    wrefresh(popup_window_ptr);
    sleep(2);

    touchwin(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);
    wclear(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);
    delwin(new_window_ptr);
    touchwin(popup_window_ptr);
    wrefresh(popup_window_ptr);
    sleep(2);
    delwin(popup_window_ptr);
    touchwin(stdscr);
    refresh();
    sleep(2);
    endwin();
    exit(EXIT_SUCCESS);
}